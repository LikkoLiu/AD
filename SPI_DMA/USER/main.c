#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "air32f10x.h"
#include "systemconfig.h"
#include "common.h"
#include "ads1256.h"

#define LED2 BIT_ADDR(GPIOB_ODR_Addr, 11)

// PA9  -----> Txd1
// PA10 -----> Rxd1

//***************************
//				Pin assign
//			STM32	 	 ADS1256
//		GPIOB_Pin_7 --- DRDY
//		GPIOB_Pin_12 --- CS
//		GPIOB_Pin_13 --- SCK
//		GPIOB_Pin_14 --- DOUT
//		GPIOB_Pin_15 --- DIN
//***************************

int main(void)
{

	SystemConfiguration();		  //ϵͳ��ʼ��
	USART_Config(USART1, 115200); //����1��ʼ���������� 115200
	Init_ADS1256_GPIO();		  //��ʼ��ADS1256 GPIO�ܽ�
	Delay_ms(50);
	ADS1256_Init();
	// printf("%c",0xaa);
	while (1)
	{
		LED2 = !LED2_READ; //ָʾ����˸

		/********************************************************/
		/*************************test***************************/
		/********************************************************/
#if 0
		for (i = 0; i < 8; i++)
		{
			gainChange(0);
			ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // �൱�� ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);

			GAIN=gainChoose(Adc);
			gainChange(GAIN);
			ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			if (Adc > 0x7FFFFF)										   // if MSB=1,
				Voltdisplay = (signed int)(Adc - 0x1000000);
			else
				Voltdisplay = (signed int)(Adc);
			/*��ֲɼ���ʽ*/
			// Adc = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 ��ַ�ʽ*/
			switch (GAIN)
				{
				case 0:
					Volts = Voltdisplay * 0.000000598;
					break;
				case 1:
					Volts = Voltdisplay * 0.000000298;
					break;
				case 2:
					Volts = Voltdisplay * 0.000000149;
					break;
				case 3:
					Volts = Voltdisplay * 0.000000075;
					break;

				default:
					printf("ERROR");
				}
			printf(" %d\b  ",Adc);
			printf(" %.4fV_%d\r\n  ", Volts,GAIN);
			if (i == 7)
				printf("\r\n");
		}

#endif
		/********************************************************/
		/********************************************************/
		/********************************************************/

#if 1
		autoGainread();
#endif
	}
}
