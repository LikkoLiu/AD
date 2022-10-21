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
	u8 GAIN_L = 0;
	unsigned int Adc = 0;
	unsigned int Adc_arr[8] = 0;
	signed int Voltdisplay = 0;
	float Volts;

	SystemConfiguration();		  //系统初始化
	USART_Config(USART1, 115200); //串口1初始化，波特率 115200
	Init_ADS1256_GPIO();		  //初始化ADS1256 GPIO管脚
	Delay_ms(50);
	ADS1256_Init();
	// printf("%c",0xaa);
	while (1)
	{
		LED2 = !LED2_READ; //指示灯闪烁

/********************************************************/
/*************************test***************************/
/********************************************************/
#if 1
		for (i = 0; i < 8; i++)
		{
			GAIN=0;
			gainChange(GAIN);
			ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // 相当于 ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);

			GAIN=gainChoose(Adc);
			gainChange(GAIN);
			ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			if (Adc > 0x7FFFFF)										   // if MSB=1,
				Voltdisplay = (signed int)(Adc - 0x1000000);
			else
				Voltdisplay = (signed int)(Adc);
			/*差分采集方式*/
			// Adc = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 差分方式*/
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

/********************************************************/
/*************************test2***************************/
/********************************************************/
#if 0
		for (i = 0; i < 8; i++)
		{
			gainChange(0);
			ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // 相当于 ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
			GAIN = gainChoose(Adc);
			gainChange(GAIN);
			ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // 相当于 ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);

			if (Adc > 0x7FFFFF)										   // if MSB=1,
				Voltdisplay = (signed int)(Adc - 0x1000000);
			else
				Voltdisplay = (signed int)(Adc);
			/*差分采集方式*/
			// Adc = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 差分方式*/
			switch (GAIN_L)
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

			printf(" %.4fV_%dG  ", Volts,GAIN);
			if (i == 7)
				printf("\r\n");
		}

#endif
		/********************************************************/
		/********************************************************/
		/********************************************************/


#if 0
		for (i = 0; i < 8; i++)
		{
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // 相当于 ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
			if (i == 0)
			{
				if (Adc > 0x7FFFFF) // if MSB=1,
					Voltdisplay = (signed int)(Adc - 0x1000000);
				else
					Voltdisplay = (signed int)(Adc);
				switch (GAIN_L)
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
					;
				}

				printf(" %.4fV_%dG  ", Volts,GAIN_L);
			}
			else
				Adc_arr[i - 1] = Adc;
		}
/*差分采集方式*/
// Adc = ADS1256ReadData( ADS1256_MUXP_AIN0|ADS1256_MUXN_AIN1); //P = AIN0 ,N = AIN1 差分方式*/

		for (i = 0; i < 8; i++)
		{
			GAIN = gainChoose(Adc_arr[i]);
			gainChange(GAIN);
			Adc = ADS1256ReadData_raw((i << 4) | ADS1256_MUXN_AINCOM); // 相当于 ( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
			if (i == 0)
				Adc_arr[7] = Adc;
			else
			{
				if (Adc > 0x7FFFFF) // if MSB=1,
					Voltdisplay = (signed int)(Adc - 0x1000000);
				else
					Voltdisplay = (signed int)(Adc);
				switch (GAIN_L)
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

				printf(" %.4fV_%dG  ", Volts,GAIN_L);

			}
			GAIN_L = GAIN;
			if (i == 0)
					printf("\r\n");

		}
#endif
	}
}
