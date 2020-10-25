

#include "HI229.h"

UART_HandleTypeDef * HI229_uart;
bool HI229_crc_check=false;
uint8_t HI229_RxBuffer[1];
uint8_t HI229_Rx_Data[96];
uint8_t HI229_Rx_Data_len=0;
bool UartStartRx=false;
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

bool crc_check(uint8_t *buf){
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
	if(crc==(buf[4]+(buf[5]<<8))){
		return true;
	}else {
		return false;
	}
}

void HI229(uint8_t *buf){
	if(crc_check(buf)){
		memcpy(&dat, &buf[6], sizeof(id0x91_t));
	}/*else{
		memset(&dat,0,sizeof(id0x91_t));
	}*/
}

void HI229_Init(UART_HandleTypeDef * huart){
	HI229_Rx_Data_len=0;
	HI229_uart = huart;
	HAL_UART_Receive_IT(HI229_uart, (uint8_t *)HI229_RxBuffer, 1);
}


void HI229_CallBack(UART_HandleTypeDef *huart){
	if(huart->Instance==HI229_uart->Instance){
		if(*HI229_RxBuffer==0x5A&&UartStartRx==false&&HI229_crc_check==false){
			UartStartRx=true;
		}
		if(UartStartRx==true){
			HI229_Rx_Data[HI229_Rx_Data_len++]=*HI229_RxBuffer;
			//HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		}
		if(HI229_Rx_Data_len<=82){
			HAL_UART_Receive_IT(HI229_uart, (uint8_t *)HI229_RxBuffer, 1);
		}else{
			UartStartRx=false;
		}
	}
}

void HI229_Update(void){
	if(HI229_Rx_Data_len>=82){
		HI229_crc_check=true;
		//HAL_UART_Transmit(&huart3,(uint8_t *)usart2_rec_buffer,usart2_rcv_len,5000);
		HI229(HI229_Rx_Data);
		memset(HI229_Rx_Data,0,HI229_Rx_Data_len);
		HI229_Rx_Data_len=0;
		HI229_crc_check=false;
		HAL_UART_Receive_IT(HI229_uart, (uint8_t *)HI229_RxBuffer, 1);
	}
}
