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
	u8 i = 0;
	u8 GAIN = 0;
	unsigned int Adc;
	int Voltdisplay;
	float Volts;

	SystemConfiguration();		  //ϵͳ��ʼ��
	USART_Config(USART1, 115200); //����1��ʼ���������� 115200
	Init_ADS1256_GPIO();		  //��ʼ��ADS1256 GPIO�ܽ�
	Delay_ms(50);
	ADS1256_Init();
	// printf("%c",0xaa);
	while (1)
	{
		LED2 = !LED2_READ; //ָʾ����˸

		for (i = 0; i < 8; i++)
		{
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // �൱�� ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);

			/*��ֲɼ���ʽ*/
			// Adc = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 ��ַ�ʽ*/
			#if voltDisplay
				GAIN = gainChoose(Adc);
				gainChange(GAIN);
				Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // �൱�� ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);

				if (Adc>0x7FFFFF)           // if MSB=1,
					Voltdisplay = Adc-0x1000000;       // do 2's complement
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
					break;
				}

				printf(" %.4fV  ", Volts);
				if(i==0)printf("\r\n");
			#endif
		}
	}
}
