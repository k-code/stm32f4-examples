/*
 * dht11.c
 *
 *  Created on: Jan 10, 2014
 *      Author: kvv
 */


#include "dht11.h"
#include "time.h"

uint16_t read_cycle(uint16_t cur_tics, uint8_t neg_tic){
	uint16_t cnt_tics;
 	if (cur_tics < MAX_TICS) cnt_tics = 0;
	if (neg_tic){
		while (!GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)&&(cnt_tics<MAX_TICS)){
			cnt_tics++;
		}
	}
	else {
		while (GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)&&(cnt_tics<MAX_TICS)){
			cnt_tics++;
		}
	}
 	return cnt_tics;
}

uint8_t read_DHT11(uint8_t *buf){
  GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t dt[42];
	uint16_t cnt;
	uint8_t i, check_sum;

  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);

	//reset DHT11
	TIME_delay(500);
 	GPIO_ResetBits(DHT11_PORT,DHT11_PIN);
 	TIME_delay(20);
 	GPIO_SetBits(DHT11_PORT,DHT11_PIN);

  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);

  //start reading
 	cnt = 0;
	for(i=0;i<85 && cnt<MAX_TICS;i++){
		if (i & 1){
			cnt = read_cycle(cnt, 1);
		}
		else {
			cnt = read_cycle(cnt, 0);
			dt[i/2]= cnt;
		}
	}

 	//release line
	GPIO_SetBits(GPIOA,GPIO_Pin_2);

	if (cnt>=MAX_TICS) return DHT11_NO_CONN;

	//convert data
 	for(i=2;i<42;i++){
		(*buf) <<= 1;
  	if (dt[i]>20) {
			(*buf)++;
 		}
		if (!((i-1)%8) && (i>2)) {
			buf++;
		}
 	}

	//calculate checksum
	buf -= 5;
	check_sum = 0;
 	for(i=0;i<4;i++){
		check_sum += *buf;
		buf++;
	}

	if (*buf != check_sum) return DHT11_CS_ERROR;

	return DHT11_OK;
	//return check_sum;
}
