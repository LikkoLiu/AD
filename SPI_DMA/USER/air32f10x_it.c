/**
 ******************************************************************************
 * @file    GPIO/IOToggle/air32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and peripherals
 *          interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "air32f10x_it.h"
#include "common.h"
#include "timer.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 air32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_air32f10x_xx.s).                                            */
/******************************************************************************/

#if EN_USART_RX //如果使能了接收

// u8  USART_RxDat;        //接收缓冲
// u8  USART_RxFlag=0;     //接收标志位
// void USART1_IRQHandler(void)
// {
// 	if(USART1->SR&(1<<5))//接收到数据
// 	{
// 		USART_RxDat=0;
// 		USART_RxDat=USART1->DR;
// 		USART_RxFlag=1;
// 	}
// }

u8 table_data[9];//这是提前定义一个数组存放接收到的数据
u8 table_cp[9];//这是额外定义一个数组，将接收到的数据复制到这里面
u16 count=0;//接收数据计数

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
		u8 Res,i;
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
		{
			Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据

			if(count==0)//如果是接收的第一个数据
			{
				table_data[count]=Res;//将第一个数据存到数据中第一元素
				if(table_data[0]==0x2c)//判断接收的第一个数据是不是十六进制0X2C
				  count++;//如果第一个数据是0X2C则表示正确计数+1
			}
			else if(count==1)//第一个数据接收正确的情况下，判断第二个数据
			{
				if(Res==0xe4)//如果刚接收的数据是0xE4则表示数据正确
				{
					table_data[count]=Res;//将数据储存到数组第二个元素位置
					count++;//接收数据计数+1
				}
				else//如果第二个字符不是0XE4则计数清零，重新接收
				{
          count=0;
          memset(table_data, 0, 9);
        }
			}
			else if(count==2&&Res==0)//如果前两个数据正确，接收的第三个数据是0，则清零计数，重新接收数据
			{
				count=0;
        memset(table_data, 0, 9);
			}
			else if(count>1&&count<9)//这是可以接收数据的范围，只要count在数据可接收数据范围内即可进行存入数据
			{
				table_data[count]=Res;
				count++;
			}
			else if(count>=9)//如果接收数据超过数组大小，则清零重新接收
			{
				count=0;
        memset(table_data, 0, 9);
			}
   }
  if(count==9)
  {
		// memset(table_cp, 0, sizeof(table_data));//在使用数组table_cp时清空
		for(i=0;i<9;i++)//把接收到的数据复制到table_cp数组中
		{
			 table_cp[i]= table_data[i];
    }
    count=0;
	}
}


#endif
//
// void USART3_IRQHandler(void)
//{
//	if(USART3->SR&(1<<5))//接收到数据
//	{
//
//		USART_RxDat=USART3->DR;
//		 LED1= !LED1_READ;
//
//
//	}
//
//}
