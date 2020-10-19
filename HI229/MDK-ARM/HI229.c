

#include "HI229.h"
#include "string.h"
#include "stdio.h"

__align(4) id0x91_t dat; /* struct must be 4 byte aligned */
/*
currectCrc: previous crc value, set 0 if it's first section
src: source stream data
lengthInBytes: length
*/
void crc16_update(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes)
{
	uint32_t crc = *currectCrc;
	uint32_t j;
	for (j=0; j < lengthInBytes; ++j){
		uint32_t i;
		uint32_t byte = src[j];
		crc ^= byte << 8;
		for (i = 0; i < 8; ++i){
			uint32_t temp = crc << 1;
			if (crc & 0x8000){
				temp ^= 0x1021;
			}
		crc = temp;
		}
	}
	*currectCrc = crc;
}

void crc_check(uint8_t *buf){
	//printf("%x\r\n",buf[0]);
	uint16_t payload_len;
	uint16_t crc;
	crc = 0;
	payload_len = buf[2] + (buf[3] << 8);
	/* calulate 5A A5 and LEN filed crc */
	crc16_update(&crc, buf, 4);
	//printf("crc1=%x\r\n",crc);
	/* calulate payload crc */
	crc16_update(&crc, buf + 6, payload_len);
	printf("calulate_crc=%x\r\ncrc=%x%x\r\n",crc,buf[5],buf[4]);
}

void HI229(uint8_t *buf){
	
	crc_check(buf);
	memcpy(&dat, &buf[6], sizeof(id0x91_t));
}
