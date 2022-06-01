#ifndef _KSERVICE_H
#define _KSERVICE_H


typedef signed   char                   bl_int8_t;      /**<  8bit integer type */
typedef signed   short                  bl_int16_t;     /**< 16bit integer type */
typedef signed   int                    bl_int32_t;     /**< 32bit integer type */
typedef unsigned char                   bl_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  bl_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned int                    bl_uint32_t;    /**< 32bit unsigned integer type */
typedef int                             bl_bool_t;      /**< boolean type */

typedef int                             bl_bool_t;      /**< boolean type */
typedef long                            bl_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   bl_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef bl_base_t                       bl_err_t;       /**< Type for error number */
typedef bl_uint32_t                     bl_time_t;      /**< Type for time stamp */
typedef bl_uint32_t                     bl_tick_t;      /**< Type for tick count */
typedef bl_base_t                       bl_flag_t;      /**< Type for flags */
typedef bl_ubase_t                      bl_size_t;      /**< Type for size number */
typedef bl_ubase_t                      bl_dev_t;       /**< Type for device */
typedef bl_base_t                       bl_off_t;       /**< Type for offset */



/* maximum value of base type */
#define BL_UINT8_MAX                    0xff            /**< Maxium number of UINT8 */
#define BL_UINT16_MAX                   0xffff          /**< Maxium number of UINT16 */
#define BL_UINT32_MAX                   0xffffffff      /**< Maxium number of UINT32 */
#define BL_TICK_MAX                     BL_UINT32_MAX   /**< Maxium number of tick */

/* the version of GNU GCC must be greater than 4.x */
typedef __builtin_va_list       __gnuc_va_list;
typedef __gnuc_va_list          va_list;
#define va_start(v,l)           __builtin_va_start(v,l)
#define va_end(v)               __builtin_va_end(v)
#define va_arg(v,l)             __builtin_va_arg(v,l)

#define SECTION(x)                  __attribute__((section(x)))
#define BL_UNUSED                   __attribute__((unused))
#define BL_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define BL_WEAK                     __attribute__((weak))
#define bl_inline                   static __inline



void bl_hex_dump(char *str, bl_uint8_t *pbuf, bl_uint32_t len);
int bl_kprintf(const char *fmt, ...);
void bl_kputs(const char *str);
int bl_sprintf(char *buf, const char *format, ...);
#endif
