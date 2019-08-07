#include "bsp_uart.h"

/*配置中断函数*/

/*
void NVIC_Config(void)
{
	  NVIC_InitTypeDef NVIC_InitStruct;
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannel=DEBUG_USART_IRQ;  //配置中断源
	  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   //中断使能
	  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;  //抢占优先级
	  NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;     //抢占子优先级
	  NVIC_Init(&NVIC_InitStruct);

}
*/

void UART_Init(void)
{     
	   GPIO_InitTypeDef GPIO_InitStruct;
	   USART_InitTypeDef USART_InitStruct;
	   /* 第一步：配置GPIO*/
	   DEBUG_USART_GPIO_APBxCLKCmd(DEBUG_USART_GPIO_CLK,ENABLE);//开启GPIO时钟
	   //配置TX
	   GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;  //配置TX为推挽复用输出 
	   GPIO_InitStruct.GPIO_Pin= DEBUG_USART_TX_GPIO_PIN;
	   GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	   GPIO_Init(DEBUG_USART_GPIO_PORT, &GPIO_InitStruct);
	   //配置RX
	   GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //配置RX为浮空输入 
	   GPIO_InitStruct.GPIO_Pin= DEBUG_USART_RX_GPIO_PIN; 
	   GPIO_Init(DEBUG_USART_GPIO_PORT, &GPIO_InitStruct);
	 
	   //第二步：配置串口
	   DEBUG_USART_APBxCLKCmd(DEBUG_USART_CLK,ENABLE); //开启串口时钟
	   USART_InitStruct.USART_BaudRate= DEBUG_USART_BAUDRATE;//配置波特率
	   USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	   USART_InitStruct.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;  //配置工作模式，收发一起
	   USART_InitStruct.USART_Parity=USART_Parity_No;    //配置校验
	   USART_InitStruct.USART_StopBits=USART_StopBits_1;   //配置停止位
	   USART_InitStruct.USART_WordLength=USART_WordLength_8b;//配置数据长度
		 USART_Init(DEBUG_USARTx, &USART_InitStruct);
		 
     //配置中断
//     NVIC_Config();
		 
		 //打开中断
		 USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
		 
		 //使能串口
		 USART_Cmd(DEBUG_USARTx, ENABLE);   
}
/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

