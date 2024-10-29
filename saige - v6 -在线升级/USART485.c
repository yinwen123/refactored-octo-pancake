#include "inc.h"
uint8_t tx_buf[15]={0};
uint8_t rx_buf[48]={0};
uint8_t tx_nub,tx_nub1,rx_nub,rx_nub1,recv_over,over_time=0;
void UART2_INIT(uint16_t baud)
{
  PA01_AFx_UART2TXD();
  PB03_AFx_UART2RXD();
  CW_SYSCTRL->APBEN1|=(0x01<<7);
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
  GPIO_InitStructure.Pins = GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = GPIO_PIN_3;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_Over = USART_Over_16;
  USART_InitStructure.USART_Source = USART_Source_PCLK;
  USART_InitStructure.USART_UclkFreq = 24000000;
  USART_InitStructure.USART_StartBit = USART_StartBit_FE;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(CW_UART2, &USART_InitStructure);
    
    CW_UART2->IER|=0x01<<2;     //开启发送，接收中断
	NVIC->IP[6]|=0x10<<6;
    NVIC->ISER[0]|=0x01<<28;
	 
}
void send_485(uint8_t data)
{
    CW_UART2->TDR=data&0xff;
}
uint8_t recve_485()
{
    return CW_UART2->RDR&0xff;
}


void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)
{
  int i;
  unsigned char tmp[4]; tmp[0] = 0;
  for(i=0;i< 8;i++)
 {
  if(srcBuf[0]& (1 << i)) tmp[0]|=1<<(7-i);
 }
  dBuf[0] = tmp[0];
}
void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf)
{
  int i;
  unsigned short tmp[4]; tmp[0] = 0;
  for(i=0;i< 16;i++)
  {
     if(srcBuf[0]& (1 << i))
       tmp[0]|=1<<(15 - i);
  }
   dBuf[0] = tmp[0];
}
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
   unsigned short wCRCin = 0xFFFF;
   unsigned short wCPoly = 0x8005;    
   unsigned char wChar = 0;
   while(usDataLen--)
   {
     wChar = *(puchMsg++);
     InvertUint8(&wChar,&wChar);
     wCRCin ^= (wChar << 8);
     for(int i = 0;i < 8;i++)
     {
        if(wCRCin & 0x8000) 
           wCRCin = (wCRCin << 1) ^ wCPoly; 
        else 
           wCRCin = wCRCin << 1; 
     }       
   }
   InvertUint16(&wCRCin,&wCRCin); 
   return (wCRCin) ;

}
void UART2_IRQHandler()
{
   rx_nub1=44;
  if(CW_UART2->ISR&(0x01<<2)) 
  {
    over_time=0;
    CW_UART2->ICR&=~(0x01<<2);
    rx_buf[rx_nub]= recve_485();
    rx_nub++;     
  }
  if(rx_nub>rx_nub1)
  {
      rx_nub=0;
     recv_over=1;
  }
  if(CW_UART2->ISR&(0x01<<1))
  {
    CW_UART2->ICR&=~(0x01<<1);
    if(tx_nub<tx_nub1)
    {
       tx_nub++;
       send_485(tx_buf[tx_nub]);
    }
    else
      RE_485_L;        
  }
}
uint16_t crc;
uint8_t hour,min,temp2,R_mod,GuoBiao_mod,cut_first=0,v485,tcs_sta;
uint16_t soc_485,last_trip;
uint16_t trip_485,odo_485,voltage_485,speed_485;
void recv_deal()
{
   if(recv_over)
   {
      recv_over=0;
      cut485=0;
//      uint8_t cut0,cut1;
      crc=CRC16_MODBUS(rx_buf,43);
      if(rx_buf[0]==0x06&&rx_buf[1]==0x10&&rx_buf[2]==0xB1&&rx_buf[3]==0x00&&rx_buf[6]==0x24&&(crc==((rx_buf[44]<<8)|rx_buf[43])))
      {     
        if(rx_buf[8]&(0x01<<1))
           car_stat.data.XunHang=1;
         else
           car_stat.data.XunHang=0;
         if(rx_buf[8]&(0x01<<6))
         {
           car_stat.data.TCS=1;
         }
         else
           car_stat.data.TCS=0;
         if(rx_buf[8]&0x01)
            car_stat.data.XianSu=1;
         else
             car_stat.data.XianSu=0;
         if(rx_buf[9]&0x01)
           car_stat.data.DaoChe=1;
         else
           car_stat.data.DaoChe=0;
         if(rx_buf[9]&(0x04))
            tcs_sta=1;
         else
            tcs_sta=0;             
          if(rx_buf[10]&(0x01<<7))
           P_STA=1;
          else
           P_STA=0;
         if(rx_buf[30]&0x01<<3)
           car_stat.data.control_error=1;
         else
           car_stat.data.control_error=0;
         if(rx_buf[29]&0x01)
            car_stat.data.zhuanBa_error=1;
         else
            car_stat.data.zhuanBa_error=0;
         if(rx_buf[30]&0x07<<5||rx_buf[29]&(0x03<<1))
         {
            temp.STA.motor=1;
             car_stat.data.QueXiang=1;
         }
         else
         {
             temp.STA.motor=0;
             car_stat.data.QueXiang=0;
         }             
        if(rx_buf[29]&(0x01<<4))    //刹车故障
           car_stat.data.ShaChe=1;
        else
           car_stat.data.ShaChe=0;             
         hour= rx_buf[33];
         min= rx_buf[34];  
        car_stat.data.SanSu= (rx_buf[10]&0x70)>>4;
        if(car_stat.data.SanSu==0x00)
           GuoBiao_mod=1;
        else
           GuoBiao_mod=0;
      if(rx_buf[41]==0x02)
        car_dt=0;
      else
        car_dt=1;        
        temp2=rx_buf[10]&0x0f;
        soc_485=rx_buf[26];
        voltage_485=(rx_buf[23]<<8)|rx_buf[24];
        speed_485=(rx_buf[13]<<8)|rx_buf[14];
        speed_485=speed_485/10;
        trip_485=(rx_buf[15]<<8)|rx_buf[16];
        trip_485=trip_485/10;
        v485=rx_buf[12];
        if(rx_buf[12]==60)
          V60_STA=1;
        else
          V60_STA=0;
        if(rx_buf[12]==72)
         V72_STA =1;
        else
         V72_STA =0;
        if(last_trip!=trip_485)
        {
            if(!cut_first)
            {
              cut_first=1;
              last_trip=trip_485;
            }
           if(trip_485>last_trip)
           {
               ODO_lc+=10;
               flash_odo_updata();
           }
           last_trip=trip_485;
        }
        
        switch(temp2)
        {
            case 0:
            {
              car_stat.data.ChangGui=0;
              car_stat.data.JieNen=1;
              car_stat.data.YunDon=0;  
            }
            break;
            case 1:
            {
             car_stat.data.ChangGui=1;
             car_stat.data.JieNen=0;
             car_stat.data.YunDon=0;   
            }
            break;
            case 2:
            {
             car_stat.data.ChangGui=0;
             car_stat.data.JieNen=0;
             car_stat.data.YunDon=1;   
            }
            break;
            default:
                break;
        }
                        
        for(u8 i=0;i<48;i++)
          rx_buf[i]=0;
        modbus_ack();                
      }
           
    }
  }
void modbus_ack()
{
  tx_nub=0;
  tx_nub1=7;
  uint16_t CRC;
  tx_buf[0]=0x06;
  tx_buf[1]=0x10;
  tx_buf[2]=0xB1;
  tx_buf[3]=0x00;
  tx_buf[4]=0x00;
  tx_buf[5]=0x12;
  CRC=CRC16_MODBUS(tx_buf,6);
  tx_buf[7]=(CRC&0xff00)>>8;
  tx_buf[6]=CRC&0xff;
  CW_UART2->IER|=0x01<<1;
  RE_485_H;
  send_485(tx_buf[tx_nub]);
}














