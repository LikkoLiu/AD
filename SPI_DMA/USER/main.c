#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "air32f10x.h"
#include "systemconfig.h"
#include "common.h"
#include "ads1256.h"

#define LED1 BIT_ADDR(GPIOB_ODR_Addr, 0)
#define LED2 BIT_ADDR(GPIOB_ODR_Addr, 1)
#define KEY BIT_ADDR(GPIOB_IDR_Addr, 2)

// PA9  -----> Txd1
// PA10 -----> Rxd1

//***************************
//				Pin assign
//			STM32	 	 ADS1256
//		GPIOB_Pin_12 --- DRDY
//		GPIOA_Pin_4 --- CS
//		GPIOA_Pin_5 --- SCK
//		GPIOA_Pin_6 --- DOUT
//		GPIOA_Pin_7 --- DIN
//***************************

int main(void)
{

	u8 i = 0;
	int Adc;
	float Volts;

	SystemConfiguration();		  //ϵͳ��ʼ��
	USART_Config(USART1, 115200); //����1��ʼ���������� 115200
	printf("UART_OK\r\n");

	Init_ADS1256_GPIO();		  //��ʼ��ADS1256 GPIO�ܽ�
	printf("ADS1256_GPIO_OK\r\n");
	Delay_ms(50);
	ADS1256_Init();
	printf("ADS1256_OK\r\n");
	while (1)
	{

		LED2 = !LED2_READ; //ָʾ����˸

		for (i = 0; i < 8; i++)
		{

			Adc = ADS1256ReadData((i << 4) | ADS1256_MUXN_AINCOM); // �൱�� ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);

			/*��ֲɼ���ʽ*/
			// Adc = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 ��ַ�ʽ*/
			Volts = Adc * 0.000000598;

			printf(" %.4fV  ", Volts);
		}
		// Delay_ms(100);
		printf("\r\n");
	}
}
