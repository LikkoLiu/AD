#ifndef __COMMON_H
#define __COMMON_H

#include "bitband.h"

#define EN_USART_RX 1         //ʹ�ܴ���1�����ж�
// #define EN_USART3_RX
//#define X0  	PAin(2)
//#define Y0  	PBout(12)

// #define LED1		BIT_ADDR(GPIOB_ODR_Addr,0)
// #define LED1_READ	BIT_ADDR(GPIOB_IDR_Addr,0)

#define LED2		BIT_ADDR(GPIOB_ODR_Addr,11)
#define LED2_READ	BIT_ADDR(GPIOB_IDR_Addr,11)

#define KEY			BIT_ADDR(GPIOB_0DR_Addr,3)
#define KEY_READ	BIT_ADDR(GPIOB_IDR_Addr,3)



extern u8 USART_RxDat;       //��������
extern u8 USART_RxFlag;      //���ձ�־λ
void Delay_Init(void);       //��ʱ��ʼ������
void Delay_ms(u16 ms);       //��ʱX���뺯��
void Delay_us(u32 us);       //��ʱX΢���
void USART_Config(USART_TypeDef* USARTx,u32 Baud);  //���ڳ�ʼ��
void RTC_Configuration(void);
u8 UartSendData(USART_TypeDef* USARTx,u8 ch);
void UartSendString(USART_TypeDef* USARTx,char *str);
#endif




