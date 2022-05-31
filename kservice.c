#include "kservice.h"

#define BL_PRINTF_PRECISION
#define BL_KPRINTF_USING_LONGLONG
#define BL_PRINTF_SPECIAL

#define BL_CONSOLEBUF_SIZE 128

/* private function */
#define _ISDIGIT(c)  ((unsigned)((c) - '0') < 10)

bl_inline int skip_atoi(const char **s)
{
    int i = 0;
    while (_ISDIGIT(**s))
        i = i * 10 + *((*s)++) - '0';

    return i;
}

#define ZEROPAD     (1 << 0)    /* pad with zero */
#define SIGN        (1 << 1)    /* unsigned/signed long */
#define PLUS        (1 << 2)    /* show plus */
#define SPACE       (1 << 3)    /* space if plus */
#define LEFT        (1 << 4)    /* left justified */
#define SPECIAL     (1 << 5)    /* 0x */
#define LARGE       (1 << 6)    /* use 'ABCDEF' instead of 'abcdef' */

static char *print_number(char *buf,
                          char *end,
#ifdef BL_KPRINTF_USING_LONGLONG
                          long long  num,
#else
                          long  num,
#endif /* BL_KPRINTF_USING_LONGLONG */
                          int   base,
                          int   s,
#ifdef BL_PRINTF_PRECISION
                          int   precision,
#endif /* BL_PRINTF_PRECISION */
                          int   type)
{
    char c, sign;
#ifdef BL_KPRINTF_USING_LONGLONG
    char tmp[32];
#else
    char tmp[16];
#endif /* BL_KPRINTF_USING_LONGLONG */
    int precision_bak = precision;
    const char *digits;
    static const char small_digits[] = "0123456789abcdef";
    static const char large_digits[] = "0123456789ABCDEF";
    int i, size;

    size = s;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;

    c = (type & ZEROPAD) ? '0' : ' ';

    /* get sign */
    sign = 0;
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
        }
        else if (type & PLUS)
            sign = '+';
        else if (type & SPACE)
            sign = ' ';
    }

#ifdef BL_PRINTF_SPECIAL
    if (type & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
#endif /* BL_PRINTF_SPECIAL */

    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else
    {
        while (num != 0)
            tmp[i++] = digits[divide(&num, base)];
    }

#ifdef BL_PRINTF_PRECISION
    if (i > precision)
        precision = i;
    size -= precision;
#else
    size -= i;
#endif /* BL_PRINTF_PRECISION */

    if (!(type & (ZEROPAD | LEFT)))
    {
        if ((sign) && (size > 0))
            size--;

        while (size-- > 0)
        {
            if (buf < end)
                *buf = ' ';
            ++ buf;
        }
    }

    if (sign)
    {
        if (buf < end)
        {
            *buf = sign;
        }
        -- size;
        ++ buf;
    }

#ifdef BL_PRINTF_SPECIAL
    if (type & SPECIAL)
    {
        if (base == 8)
        {
            if (buf < end)
                *buf = '0';
            ++ buf;
        }
        else if (base == 16)
        {
            if (buf < end)
                *buf = '0';
            ++ buf;
            if (buf < end)
            {
                *buf = type & LARGE ? 'X' : 'x';
            }
            ++ buf;
        }
    }
#endif /* BL_PRINTF_SPECIAL */

    /* no align to the left */
    if (!(type & LEFT))
    {
        while (size-- > 0)
        {
            if (buf < end)
                *buf = c;
            ++ buf;
        }
    }

#ifdef BL_PRINTF_PRECISION
    while (i < precision--)
    {
        if (buf < end)
            *buf = '0';
        ++ buf;
    }
#endif /* BL_PRINTF_PRECISION */

    /* put number in the temporary buffer */
    while (i-- > 0 && (precision_bak != 0))
    {
        if (buf < end)
            *buf = tmp[i];
        ++ buf;
    }

    while (size-- > 0)
    {
        if (buf < end)
            *buf = ' ';
        ++ buf;
    }

    return buf;
}

/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  size is the size of buffer.
 *
 * @param  fmt is the format parameters.
 *
 * @param  args is a list of variable parameters.
 *
 * @return The number of characters actually written to buffer.
 */
BL_WEAK int bl_vsnprintf(char *buf, bl_size_t size, const char *fmt, va_list args)
{
#ifdef BL_KPRINTF_USING_LONGLONG
    unsigned long long num;
#else
    bl_uint32_t num;
#endif /* BL_KPRINTF_USING_LONGLONG */
    int i, len;
    char *str, *end, c;
    const char *s;

    bl_uint8_t base;            /* the base of number */
    bl_uint8_t flags;           /* flags to print number */
    bl_uint8_t qualifier;       /* 'h', 'l', or 'L' for integer fields */
    bl_int32_t field_width;     /* width of output field */

#ifdef BL_PRINTF_PRECISION
    int precision;      /* min. # of digits for integers and max for a string */
#endif /* BL_PRINTF_PRECISION */

    str = buf;
    end = buf + size;

    /* Make sure end is always >= buf */
    if (end < buf)
    {
        end  = ((char *) - 1);
        size = end - buf;
    }

    for (; *fmt ; ++fmt)
    {
        if (*fmt != '%')
        {
            if (str < end)
                *str = *fmt;
            ++ str;
            continue;
        }

        /* process flags */
        flags = 0;

        while (1)
        {
            /* skips the first '%' also */
            ++ fmt;
            if (*fmt == '-') flags |= LEFT;
            else if (*fmt == '+') flags |= PLUS;
            else if (*fmt == ' ') flags |= SPACE;
            else if (*fmt == '#') flags |= SPECIAL;
            else if (*fmt == '0') flags |= ZEROPAD;
            else break;
        }

        /* get field width */
        field_width = -1;
        if (_ISDIGIT(*fmt)) field_width = skip_atoi(&fmt);
        else if (*fmt == '*')
        {
            ++ fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

#ifdef BL_PRINTF_PRECISION
        /* get the precision */
        precision = -1;
        if (*fmt == '.')
        {
            ++ fmt;
            if (_ISDIGIT(*fmt)) precision = skip_atoi(&fmt);
            else if (*fmt == '*')
            {
                ++ fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0) precision = 0;
        }
#endif /* BL_PRINTF_PRECISION */
        /* get the conversion qualifier */
        qualifier = 0;
#ifdef BL_KPRINTF_USING_LONGLONG
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
#else
        if (*fmt == 'h' || *fmt == 'l')
#endif /* BL_KPRINTF_USING_LONGLONG */
        {
            qualifier = *fmt;
            ++ fmt;
#ifdef BL_KPRINTF_USING_LONGLONG
            if (qualifier == 'l' && *fmt == 'l')
            {
                qualifier = 'L';
                ++ fmt;
            }
#endif /* BL_KPRINTF_USING_LONGLONG */
        }

        /* the default base */
        base = 10;

        switch (*fmt)
        {
        case 'c':
            if (!(flags & LEFT))
            {
                while (--field_width > 0)
                {
                    if (str < end) *str = ' ';
                    ++ str;
                }
            }

            /* get character */
            c = (bl_uint8_t)va_arg(args, int);
            if (str < end) *str = c;
            ++ str;

            /* put width */
            while (--field_width > 0)
            {
                if (str < end) *str = ' ';
                ++ str;
            }
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s) s = "(NULL)";

            for (len = 0; (len != field_width) && (s[len] != '\0'); len++);
#ifdef BL_PRINTF_PRECISION
            if (precision > 0 && len > precision) len = precision;
#endif /* BL_PRINTF_PRECISION */

            if (!(flags & LEFT))
            {
                while (len < field_width--)
                {
                    if (str < end) *str = ' ';
                    ++ str;
                }
            }

            for (i = 0; i < len; ++i)
            {
                if (str < end) *str = *s;
                ++ str;
                ++ s;
            }

            while (len < field_width--)
            {
                if (str < end) *str = ' ';
                ++ str;
            }
            continue;

        case 'p':
            if (field_width == -1)
            {
                field_width = sizeof(void *) << 1;
                flags |= ZEROPAD;
            }
#ifdef BL_PRINTF_PRECISION
            str = print_number(str, end,
                               (long)va_arg(args, void *),
                               16, field_width, precision, flags);
#else
            str = print_number(str, end,
                               (long)va_arg(args, void *),
                               16, field_width, flags);
#endif /* BL_PRINTF_PRECISION */
            continue;

        case '%':
            if (str < end) *str = '%';
            ++ str;
            continue;

        /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            if (str < end) *str = '%';
            ++ str;

            if (*fmt)
            {
                if (str < end) *str = *fmt;
                ++ str;
            }
            else
            {
                -- fmt;
            }
            continue;
        }

#ifdef BL_KPRINTF_USING_LONGLONG
        if (qualifier == 'L') num = va_arg(args, long long);
        else if (qualifier == 'l')
#else
        if (qualifier == 'l')
#endif /* BL_KPRINTF_USING_LONGLONG */
        {
            num = va_arg(args, bl_uint32_t);
            if (flags & SIGN) num = (bl_int32_t)num;
        }
        else if (qualifier == 'h')
        {
            num = (bl_uint16_t)va_arg(args, bl_int32_t);
            if (flags & SIGN) num = (bl_int16_t)num;
        }
        else
        {
            num = va_arg(args, bl_uint32_t);
            if (flags & SIGN) num = (bl_int32_t)num;
        }
#ifdef BL_PRINTF_PRECISION
        str = print_number(str, end, num, base, field_width, precision, flags);
#else
        str = print_number(str, end, num, base, field_width, flags);
#endif /* BL_PRINTF_PRECISION */
    }

    if (size > 0)
    {
        if (str < end) *str = '\0';
        else
        {
            end[-1] = '\0';
        }
    }

    /* the trailing null byte doesn't count towards the total
    * ++str;
    */
    return str - buf;
}

/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  size is the size of buffer.
 *
 * @param  fmt is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int bl_snprintf(char *buf, bl_size_t size, const char *fmt, ...)
{
    bl_int32_t n;
    va_list args;

    va_start(args, fmt);
    n = bl_vsnprintf(buf, size, fmt, args);
    va_end(args);

    return n;
}

/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  format is the format parameters.
 *
 * @param  arg_ptr is a list of variable parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int bl_vsprintf(char *buf, const char *format, va_list arg_ptr)
{
    return bl_vsnprintf(buf, (bl_size_t) - 1, format, arg_ptr);
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param  buf the buffer to save formatted string.
 *
 * @param  format is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int bl_sprintf(char *buf, const char *format, ...)
{
    bl_int32_t n;
    va_list arg_ptr;

    va_start(arg_ptr, format);
    n = bl_vsprintf(buf, format, arg_ptr);
    va_end(arg_ptr);

    return n;
}

BL_WEAK void bl_hw_console_output(const char *str)
{
    /* empty console output */
}

/**
 * This function will put string to the console.
 *
 * @param str is the string output to the console.
 */
void bl_kputs(const char *str)
{
    if (!str) return;

    bl_hw_console_output(str);
}

/**
 * This function will print a formatted string on system console.
 *
 * @param fmt is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
BL_WEAK int bl_kprintf(const char *fmt, ...)
{
    va_list args;
    bl_size_t length;
    static char bl_log_buf[BL_CONSOLEBUF_SIZE];

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the bl_log_buf, we have to adjust the output
     * length. */
    length = bl_vsnprintf(bl_log_buf, sizeof(bl_log_buf) - 1, fmt, args);
    if (length > BL_CONSOLEBUF_SIZE - 1)
        length = BL_CONSOLEBUF_SIZE - 1;
    bl_hw_console_output(bl_log_buf);
    va_end(args);

    return length;
}

void bl_hex_dump(char *str, bl_uint8_t *pbuf, bl_uint32_t len)
{
    unsigned char *pt;
    int x;

    pt = pbuf;
    bl_kprintf("%s: %p, len = %d\n\r", str, pbuf, len);
    for (x = 0; x < len; x++) {
        if (x % 16 == 0) {
            bl_kprintf("0x%04x : ", x);
        }
        bl_kprintf("%02x ", ((bl_uint8_t)pt[x]));
        if (x % 16 == 15) {
            bl_kprintf("\n\r");
        }
    }
    bl_kprintf("\n\r");
}
