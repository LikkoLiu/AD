#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "air32f10x.h"
#include "systemconfig.h"
#include "common.h"
#include "ads1256.h"

#define LED2 BIT_ADDR(GPIOB_ODR_Addr, 11)
u8 AUTO_continuous_flag = 0;
u8 AUTO_single_flag = 0;
u8 unAUTO_continuous_flag = 0;
u8 unAUTO_single_flag = 0;

u8 SG_AUTO_continuous_flag = 0;
u8 SG_AUTO_single_flag = 0;
u8 SG_unAUTO_continuous_flag = 0;
u8 SG_unAUTO_single_flag = 0;

u8 unAUTO_gain_sta = 0;
u8 channel_sta = 8;
u8 print_Format;

// PA9  -----> Txd1
// PA10 -----> Rxd1
// PB3 -----> KeyIO
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
	extern u8 table_cp[9];
	extern u8 table_data[9];
	u8 table_sum = 0;
	int i;

	SystemConfiguration();		  //系统初始化
	USART_Config(USART1, 115200); //串口1初始化，波特率 115200
	Init_ADS1256_GPIO();		  //初始化ADS1256 GPIO管脚
	Delay_ms(50);
	ADS1256_Init();
	// printf("%c",0xaa);
	while (1)
	{
		LED2 = !LED2_READ; //指示灯闪烁

		/***************@@@@@ 串口接收校验 @@@@*******************/
		if (table_cp[0] == 0x2c) //如果数组第一个十六进制数据是0X2C则进行
		{
			//原始数据（十六进制数据）是2C E4 04 00 00 AD 01 23 FC
			table_sum = 0;
			for (i = 0; i < 8; i++)
			{
				// USART_SendData(USART1, table_cp[i]);
				table_sum += table_cp[i];
			}
			if (table_sum == table_cp[8])
			{
				for (i = 0; i < 9; i++)
				{
					// USART_SendData(USART1, table_cp[i]);
					printf("%c", table_cp[i]);
				}
				print_Format = table_cp[6];
				if (table_cp[2] == 0x30)
				{
					AUTO_continuous_flag = 0;
					unAUTO_continuous_flag = 0;

					SG_AUTO_continuous_flag = 0;
					SG_unAUTO_continuous_flag = 0;
				}

				else if (table_cp[2] == 0x31)
				{
					switch (table_cp[3])
					{
					case 0x31:
						unAUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 0;
						AUTO_continuous_flag = 1;
						channel_sta = table_cp[4];
						break;
					case 0x32:
						AUTO_continuous_flag = 0;
						unAUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 0;
						AUTO_single_flag = 1;
						channel_sta = table_cp[4];
						break;
					case 0x33:
						AUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 0;
						unAUTO_continuous_flag = 1;
						unAUTO_gain_sta = table_cp[5];
						channel_sta = table_cp[4];
						break;
					case 0x34:
						AUTO_continuous_flag = 0;
						unAUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 0;
						unAUTO_single_flag = 1;
						unAUTO_gain_sta = table_cp[5];
						channel_sta = table_cp[4];
						break;
					default:
						break;
					}
				}

				else if (table_cp[2] == 0x32)
				{
					switch (table_cp[3])
					{
					case 0x31:
						unAUTO_continuous_flag = 0;
						AUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 1;
						SG_unAUTO_continuous_flag = 0;
						channel_sta = table_cp[4];
						break;
					case 0x32:
						AUTO_continuous_flag = 0;
						unAUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 0;
						SG_AUTO_single_flag = 1;
						channel_sta = table_cp[4];
						break;
					case 0x33:
						AUTO_continuous_flag = 0;
						unAUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 1;
						unAUTO_gain_sta = table_cp[5];
						channel_sta = table_cp[4];
						break;
					case 0x34:
						AUTO_continuous_flag = 0;
						unAUTO_continuous_flag = 0;
						SG_AUTO_continuous_flag = 0;
						SG_unAUTO_continuous_flag = 0;
						SG_unAUTO_single_flag = 1;
						unAUTO_gain_sta = table_cp[5];
						channel_sta = table_cp[4];
						break;
					default:
						break;
					}
				}
			}
			memset(table_cp, 0, 9); //在使用数组table_cp时清空
			memset(table_data, 0, 9);
		}
		/********************************************************/

		/********************************************************/
		/*************************test***************************/
		/********************************************************/
#if 0
		for (i = 0; i < 8; i++)
		{
			gainChange(0);
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

#if 1
		if (AUTO_continuous_flag == 1 || AUTO_single_flag == 1)
		{
			autoGainread(channel_sta);
			AUTO_single_flag = 0;
		}
		if (unAUTO_continuous_flag == 1 || unAUTO_single_flag == 1)
		{
			un_autoGainread(unAUTO_gain_sta,channel_sta);
			unAUTO_single_flag = 0;
		}

		if (SG_AUTO_continuous_flag == 1 || SG_AUTO_single_flag == 1)
		{
			single_autoGainread(channel_sta);
			SG_AUTO_single_flag = 0;
		}
		if (SG_unAUTO_continuous_flag == 1 || SG_unAUTO_single_flag == 1)
		{
			single_unautoGainread(channel_sta,unAUTO_gain_sta);
			SG_unAUTO_single_flag = 0;
		}
#endif
	}
}
