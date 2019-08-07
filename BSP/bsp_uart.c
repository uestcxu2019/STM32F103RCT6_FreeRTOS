#include "bsp_uart.h"

/*�����жϺ���*/

/*
void NVIC_Config(void)
{
	  NVIC_InitTypeDef NVIC_InitStruct;
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannel=DEBUG_USART_IRQ;  //�����ж�Դ
	  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   //�ж�ʹ��
	  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;  //��ռ���ȼ�
	  NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;     //��ռ�����ȼ�
	  NVIC_Init(&NVIC_InitStruct);

}
*/

void UART_Init(void)
{     
	   GPIO_InitTypeDef GPIO_InitStruct;
	   USART_InitTypeDef USART_InitStruct;
	   /* ��һ��������GPIO*/
	   DEBUG_USART_GPIO_APBxCLKCmd(DEBUG_USART_GPIO_CLK,ENABLE);//����GPIOʱ��
	   //����TX
	   GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;  //����TXΪ���츴����� 
	   GPIO_InitStruct.GPIO_Pin= DEBUG_USART_TX_GPIO_PIN;
	   GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	   GPIO_Init(DEBUG_USART_GPIO_PORT, &GPIO_InitStruct);
	   //����RX
	   GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //����RXΪ�������� 
	   GPIO_InitStruct.GPIO_Pin= DEBUG_USART_RX_GPIO_PIN; 
	   GPIO_Init(DEBUG_USART_GPIO_PORT, &GPIO_InitStruct);
	 
	   //�ڶ��������ô���
	   DEBUG_USART_APBxCLKCmd(DEBUG_USART_CLK,ENABLE); //��������ʱ��
	   USART_InitStruct.USART_BaudRate= DEBUG_USART_BAUDRATE;//���ò�����
	   USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	   USART_InitStruct.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;  //���ù���ģʽ���շ�һ��
	   USART_InitStruct.USART_Parity=USART_Parity_No;    //����У��
	   USART_InitStruct.USART_StopBits=USART_StopBits_1;   //����ֹͣλ
	   USART_InitStruct.USART_WordLength=USART_WordLength_8b;//�������ݳ���
		 USART_Init(DEBUG_USARTx, &USART_InitStruct);
		 
     //�����ж�
//     NVIC_Config();
		 
		 //���ж�
		 USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
		 
		 //ʹ�ܴ���
		 USART_Cmd(DEBUG_USARTx, ENABLE);   
}
/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

