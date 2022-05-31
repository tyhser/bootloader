#ifndef _DRIVER_CONFIG_H
#define _DRIVER_CONFIG_H

typedef signed   char                   INT8;      /**<  8bit integer type */
typedef signed   short                  INT16;     /**< 16bit integer type */
typedef signed   int                    INT32;     /**< 32bit integer type */
typedef unsigned char                   UINT8;     /**<  8bit unsigned integer type */
typedef unsigned short                  UINT16;    /**< 16bit unsigned integer type */
typedef unsigned int                    UINT32;    /**< 32bit unsigned integer type */
typedef int                             BOOL;      /**< boolean type */

/* maximum value of base type */
#define DRV_UINT8_MAX                    0xff            /**< Maxium number of UINT8 */
#define DRV_UINT16_MAX                   0xffff          /**< Maxium number of UINT16 */
#define DRV_UINT32_MAX                   0xffffffff      /**< Maxium number of UINT32 */

#endif
