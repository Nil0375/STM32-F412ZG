
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HI229_H
#define __HI229_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


__packed typedef struct
{
    uint8_t tag; /* data packet tag */
    uint8_t id;
    uint8_t rev[6]; /* reserved */
    uint32_t ts; /* timestamp */
    float acc[3];
    float gyr[3];
    float mag[3];
    float eul[3]; /* eular angles: Roll,Pitch,Yaw */
    float quat[4]; /* quaternion */
}id0x91_t;

extern __align(4) id0x91_t dat;

void crc16_update(uint16_t *, const uint8_t *, uint32_t );

void crc_check(uint8_t *buf);

void HI229(uint8_t *buf);

#ifdef __cplusplus
}
#endif
#endif /*__ HI229_H */
