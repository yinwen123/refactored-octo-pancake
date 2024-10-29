
#include "inc.h"

volatile u16 Tim_out = 0;

u8 Tim_write = 0,Nfc_af = 0;

__align(4) u8 Set_card[12];

void get_crc(u8 *buf,u8 len)
{
    u16 wCrc = 0x6363;

    for(u32 i = 0;i < len;i++)
    {
        u16 bt = buf[i];
        bt ^= (wCrc & 0xFF);
        bt ^= (bt << 4);
        bt &= 0xff;
        u16 bt1 = bt << 8;
        u16 bt2 = bt << 3;
        u16 bt3 = bt >> 4;
        wCrc >>= 8;
        wCrc ^= bt1;
        wCrc ^= bt2;
        wCrc ^= bt3;
    }

    buf[len] = wCrc;
    buf[len + 1] = wCrc >> 8;
}

void set_mode(u8 mode)
{
	if(mode > 1)
	{
		Tim_write = 200;
	}
	else
	{
		Tim_write = 100;
	}
	CARD_LMODE = mode;
}

u8 check_buf_word4(u8 *buf0,u8 *buf1)
{
    if(*(u32*)&buf0[0] != *(u32*)&buf1[0])
    {
        return 1;
    }
    if(*(u32*)&buf0[4] != *(u32*)&buf1[4])
    {
        return 1;
    }
    if(*(u32*)&buf0[8] != *(u32*)&buf1[8])
    {
        return 1;
    }
    return 0;
}

u8 checkbak_id(u32 uid)
{
	u8 ret = 0;
    if(CARD_LEVEL > 8)
    {
        CARD_LEVEL = 2;
        return 0;
    }
    if((uid == 0) || (uid == 0xffffffff))
    {
        return 0;
    }
    for(u32 i = 1;i < 9;i ++)
    {
        if(uid == CARD_ID[i])
        {
			if(i < 3)
			{
				ret |= 0x80;
			}
			else
			{
				ret &= 0xf0;
				ret |= i;
			}
        }
    }
    return ret;
}

void spi_send(u8 data)
{
	u8 dat = data;
	for(u32 i = 0;i < 8;i ++)
	{
		if(dat & 0x80) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
		dat <<= 1;
	}
	/*
    if(data & 0x80) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x40) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x20) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x10) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x08) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x04) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x02) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
    if(data & 0x01) SPI_MO_H;else SPI_MO_L;SPI_CK_H;SPI_CK_L;
	*/
}

u8 pcd_read(u8 reg)
{
    SPI_CS_L;
    u8 data = 0;
    spi_send((reg << 1) | 0x80);
    SPI_MO_H;
	for(u32 i = 0;i < 8;i ++)
	{
		data <<= 1;
		SPI_CK_H; if(SPI_MISO) data |= 1; SPI_CK_L;
	}
	/*
    SPI_CK_H; if(SPI_MISO) data |= 0x80; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x40; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x20; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x10; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x08; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x04; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x02; SPI_CK_L;
    SPI_CK_H; if(SPI_MISO) data |= 0x01; SPI_CK_L;
	*/
    SPI_MO_L;
    SPI_CS_H;
    return data;
}

void pcd_write(u8 reg,u8 idata)
{
    SPI_CS_L;
    spi_send((reg << 1) & 0x7e);
    spi_send(idata);
    SPI_MO_L;
    SPI_CS_H;
}

void set_mask(u8 reg,u8 mask)   //设置bit
{
    u8 data = mask;
    data |= pcd_read(reg);
    pcd_write(reg,data);
}

void clr_mask(u8 reg,u8 mask)   //清除bit
{
    u8 data = ~mask;
    data &= pcd_read(reg);
    pcd_write(reg,data);
}

void pcd_reset(void)    //复位
{
    //RF_RST_H;
    //loop_delayms(1);
    pcd_write(1,0xf);   //软复位
    //loop_delayms(1);
    //pcd_write(0x2d,10);
	
	pcd_write(0x0b,0x10);
    set_mask(0x0c,0x10);        //配置为读卡器
	
    pcd_write(0x2c,0);
    pcd_write(0x2a,0x8d);
    pcd_write(0x2b,0x3e);   //设置定时器
    pcd_write(0x15,0x40);   //强制100%ASK
    set_mask(0x14,3);       //开启天线发射
}

u8 pcd_cmd(u8 cmd,u8 *idata,u8 ilen,u8 *olen)   //数据收发
{
    u8 status = 1;
    u8 irqen = 0,waitfor = 0;
    switch(cmd)
    {
        case 0x0e:  //验证密钥
        {
            irqen = 0x12;
            waitfor = 0x11;
            break;
        }
        case 0x0c:  //发送接收数据
        {
            irqen = 0x77;
            waitfor = 0x31;
            break;
        }
    }
    pcd_write(0x02,irqen | 0x80);   //设置中断
    clr_mask(0x04,0x80);    //置位中断
    pcd_write(0x01,0);      //终止所有命令
    set_mask(0x0a,0x80);    //清除缓存

    for(u32 i = 0;i < ilen;i ++) //加载数据至FIFO
    {
        pcd_write(0x09,idata[i]);
    }

    pcd_write(0x01,cmd);

    if(cmd == 0x0c)
    {
        set_mask(0x0d,0x80);    //开始传输数据
    }

    Tim_out = 0;
    u8 irq = 0;

    do
    {
        irq = pcd_read(0x04);
        if(irq & waitfor)
        {
            break;
        }
    }while(Tim_out < 150);  //一直查询状态，直到收到数据或超时

    clr_mask(0x0d,0xf7);    //清除发送接收最后一个字节bit数量

    if(Tim_out < 150)   //未超时
    {
        u8 r6 = pcd_read(0x06); //读错误标志
        if((r6 & 0x1b) == 0)
        {
            if(cmd == 0x0c) //发收数据
            {
                if((irq & irqen & 1) == 0)
                {
                    status = 0;
                }
            }
            else    //验证密钥
            {
                if(pcd_read(0x08) & 8)
                {
                    status = 0;
                }
            }
        }
        else if(r6 & 0x08)  //冲撞
        {
            status = 2;
        }
        u8 len = pcd_read(0x0a);    //获取FIFO字节数
        u8 lasb = pcd_read(0x0c) & 7;   //获取最后一个字节有效bit数
        if(lasb)
        {
            *olen = ((len - 1) << 3) + lasb;
        }
        else
        {
            *olen = (len << 3);
        }
        if(len > 18)
        {
            len = 18;
        }
        else if(len == 0)
        {
            len = 1;
        }
        for(u32 i = 0;i < len;i ++)
        {
            idata[i] = pcd_read(0x09);  //读取缓存数据
        }
    }
    set_mask(0x0c,0x80);    //停止定时器
    pcd_write(0x01,0);      //终止所有命令
    return status;
}

u8 find_card(u8 *cardtype)  //寻卡
{
    u8 len = 0,buf[18] = {0x26};
    pcd_write(0x2d,2);  //寻卡超时设置
    clr_mask(0x08,0x08);//清除加密状态标志
    pcd_write(0x0d,7);  //发送7个数据位
    //set_mask(0x14,3);
    u8 status = pcd_cmd(0x0c,buf,1,&len);
    if((status == 0) && (len == 16))
    {
        cardtype[0] = buf[0];
        cardtype[1] = buf[1];
        return status;
    }
    return 1;
}

u8 Coll = 0;

u8 pcd_coll(u8 *uid)    //防冲撞
{
    u8 len = 0,buf[18] = {0x93,0x20};
    //u8 cln[18],ubits = 0,collbits = 0,ubyte = 0;
    pcd_write(0x2d,10);//防冲撞超时设置
    clr_mask(0x08,0x08);//清除加密状态标志
    pcd_write(0x0d,0);//最后发送完整数据位
    clr_mask(0x0e,0x80);    //冲突后清除所有接收的位
    u8 status = pcd_cmd(0x0c,buf,2,&len);
    if(status == 0)
    {
        u8 check = 0;
        for(u32 i = 0;i < 4;i ++)
        {
            uid[i] = buf[i];
            check ^= buf[i];
        }
        if(check != buf[4])
        {
            status = 1;
        }
    }
    Coll = 0;   //没有冲突
    set_mask(0x0e,0x80);//不清除所有接收的位
    return status;
}

u8 sel_card(u8 *uid)    //选择卡片
{
    u8 len = 0,buf[18] = {0x93,0x70,0,0,0,0,0};
    for(u32 i = 0;i < 4;i ++)
    {
        buf[2 + i] = uid[i];
        buf[6] ^= uid[i];
    }
    pcd_write(0x2d,10);
    get_crc(buf,7);
    clr_mask(0x08,0x08);
    u8 status = pcd_cmd(0x0c,buf,9,&len);
    if(len != 0x18)
    {
        status = 1;
    }
    return status;
}

u8 password(u8 addr,u8 *key,u8 *uid)    //密钥验证
{
    u8 len = 0,buf[18] = {0x60};
    buf[1] = addr;
    for(u32 i = 0;i < 6;i ++)
    {
        buf[i + 2] = key[i];
        buf[i + 8] = uid[i];
    }
    pcd_write(0x2d,10);
    u8 status = pcd_cmd(0x0e,buf,12,&len);

    return status;
}

u8 pcd_rblock(u8 addr,u8 *obuf) //读取一块
{
    u8 len = 0,buf[18] = {0x30};
    buf[1] = addr;
    pcd_write(0x2d,10);
    get_crc(buf,2);
    u8 status = pcd_cmd(0x0c,buf,4,&len);
    if(status == 0)
    {
        if(len == 0x90)
        {
            for(u32 i = 0;i < 16;i ++)
            {
                obuf[i] = buf[i];
            }
        }
        else
        {
            status = 1;
        }
    }
    return status;
}

u8 pcd_wblock(u8 addr,u8 *ibuf) //写入一块
{
    u8 len = 0,buf[18] = {0xa0};
    buf[1] = addr;
    pcd_write(0x2d,30);
    get_crc(buf,2);
    u8 status = pcd_cmd(0x0c,buf,4,&len);
    if(status) { return status;}
    if((len != 4) || ((buf[0] & 0xf) != 0x0a))
    {
        return 1;
    }
    for(u32 i = 0;i < 16;i ++)
    {
        buf[i] = ibuf[i];
    }
    get_crc(buf,16);
    status = pcd_cmd(0x0c,buf,18,&len);
    if(status) { return status;}
    if((len != 4) || ((buf[0] & 0xf) != 0x0a))
    {
        return 1;
    }
    return status;
}
u8 ALM_s = 0,ALM_c = 0,ZDS_c = 0;
u8 open_buf[4]={0x86,0xAA,0x00,0x2c};
u8 close_buf[4]={0x86,0x55,0x00,0xd3};
#define TOSC32 10

u8* alm_send(u8 len,u8 cmd)
{
    static u8 bit = 0,byte = 0,tims = 0,step2 = 0;
    static u16 tim = 0;
    static u8* p;
    if(cmd==0xAA)
      p=open_buf;
    else if(cmd==0x55)
      p=close_buf;
    switch(step2)
    {
        case 0:
        {
          ACC_OFF;
          tim++;
          if(tim>TOSC32*60)
          {
            tim=0;
            step2++;
          }
        }
        break;
        case 1:
        {
          ACC_ON;
          tim++;
          if(tim>TOSC32)
          {
            tim=0;
            step2++;
          }              
        }
        break;
        case 2:
        {
          if((p[byte]>>bit)&0x01)
          {
            tim++;
            if(tim<TOSC32)
               ACC_OFF;
            else if(tim<TOSC32*3)
               ACC_ON;
            else
            {
              tim=0;
              bit++;
              if(bit>7)
              {
                bit=0;
                byte++;     
              }
            }                
          }
          else
          {
            tim++;
            if(tim<TOSC32*2)
               ACC_OFF;
            else if(tim<TOSC32*3)
               ACC_ON; 
            else
            {
              tim=0;
              bit++;
              if(bit>7)
              {
                bit=0;
                byte++;     
              }
            }   
          }
         if(byte==len)
         {
           tim=0;
           byte=0;
           bit=0;
           step2++;
         }             
        }
        break;
        default :
         break;            
    }
    return &step2;    
}

void alm_cmd(u8 cmd)
{
	if(cmd == 0xff)
	{
		ZDS_c = 1;
	}
	else
	{
		ALM_c = cmd;
	}
	ALM_s = 1;
}
//__align(4) u8 PassWord[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
__align(4) u8 PassWord_new[6] = {0x00,0x00,0x00,0x00,0xff,0xff};
__align(4) u8 Tscard[12] = {0xa5,0x5a,0x12,0x34,0x67,0x89,0xab,0xcd,0xef,0x1e,0x2d,0x3c};

//__hcode
void nfc_loop(void) //读写卡主循环
{
    static u8 step = 0,leave = 0,cnt_ontim = 0,card_type = 0,s_s = 0;
    __align(4) u8 uid[6];
	
    Nfc_af ++;
    switch(step)
    {
		case 0: //复位
        {
			nfc_afinit(1);
			RF_RST_H;
            step ++;
            break;
        }
        case 1: //复位
        {
            if(leave || (Speed < 2))
            {
                pcd_reset();
                step ++;
            }
            else
            {
                step = 6;
            }
            break;
        }
        case 2: //寻卡
        {
            u8 card[2];
            if(find_card(card) == 0)
            {
                if(pcd_coll(uid) == 0)
                {
                    if(leave)
                    {
						leave = 5;
                        if(cnt_ontim < 100)
                        {
                            cnt_ontim ++;
                            if(cnt_ontim == 100)
                            {
                                if(Start_type && (card_type == 1))
                                {
                                    set_mode(1);
                                    //MUSIC_T(3);
                                }
                            }
							else if(cnt_ontim == 30)
                            {
                                if(Start_type == 0)
                                {
									if(Alm_type == 0x54)
									{
										Alm_type = 0x53;
									}
									else
									{
										Alm_type = 0x54;
									}
									
//									alm_cmd(Alm_type);
                                }
                            }
                        }
                        RF_RST_L;
                        step = 6;
                        break;
                    }
                    if(sel_card(uid) == 0)
                    {
						
                        u32 uid32 = *(u32*)&uid[0];

                        *(u32*)&PassWord_new[0] = uid32 ^ 0x12345678;

                        if(Tim_write)
                        {
                            if(CARD_LMODE == 1)
                            {
                                if(CARD_LEVEL < 8)
                                {
                                    CARD_LEVEL ++;
                                }
                                else
                                {
                                    CARD_LEVEL = 3;
                                }
                                CARD_ID[CARD_LEVEL] = uid32;
                                flash_user_updata();
                                leave = 5;
                                cnt_ontim = 50;
                                //MUSIC_T(3);
                                Tim_write = 0;
                            }
                            else if(password(8,PassWord_new,uid) == 0)
                            {
                                if(pcd_wblock(8,Set_card) == 0)
                                {
                                    if(CARD_INDEX == 0xff)
                                    {
                                        CARD_INDEX = 0;
                                        CARD_ID[1] = uid32;
                                    }
                                    else
                                    {
                                        CARD_INDEX = 0xff;
                                        CARD_ID[2] = uid32;
                                    }
                                    flash_user_updata();
                                    leave = 5;
                                    cnt_ontim = 50;
                                    //MUSIC_T(3);
                                    Tim_write = 200;
                                }
                            }
                        }
                        else
                        {
                            u8 ret = checkbak_id(uid32);
                            if(ret)
                            {
                                if(ret & 0x80)
                                {
                                    if(password(8,PassWord_new,uid) == 0)
                                    {
                                        u8 read[16];
                                        if(pcd_rblock(8,read) == 0)
                                        {
                                            RF_RST_L;
                                            if(check_buf_word4(read,Set_card))    //ID不匹配
                                            {
                                                step = 6;
                                                break;
                                            }
											card_type = 1;
                                        }
										else
										{
											card_type = 2;
										}
                                    }
									else
									{
										card_type = 2;
									}
                                }
                                else
                                {
                                    card_type = 2;
                                }

                                if(Start_type == 0)
                                {
//                                   estart ( START_TYPE_NFC );
								   alm_cmd(0xAA);
                                    //MUSIC_ST;
                                }
                                else
                                {
                                  
                                      alm_cmd(0x55);
//									eclose ();
//									alm_cmd(0xAA);
                                }
                                leave = 3;
								s_s = 0;
                                cnt_ontim = 0;
                            }
							/*
                            else if(Start_type && Led_f.GBT)
                            {
                                if(password(8,PassWord_new,uid) == 0)
                                {
                                    u8 read[16];
                                    if(pcd_rblock(8,read) == 0)
                                    {
                                        if(check_buf_word4(read,Tscard) == 0) //解限速
                                        {
											s_s = 0;
											leave = 3;
											//speedcmd();
											cnt_ontim = 50;
                                        }
                                    }
                                }
                                card_type = 0;
                            }
							*/
							else
							{
								card_type = 0;
							}
                        }
                    }
                }
                else if(leave)
                {
                    leave --;
                }
            }
            else if(leave)
            {
                leave --;
            }
            RF_RST_L;
            step = 6;
            break;
        }
        case 6: //关闭芯片并等待下个周期
        {
            RF_RST_L;
            if(Nfc_af > 9)
            {
                Nfc_af = 0;
                step = 0;
                if(Tim_write)
                {
                    Tim_write --;
                    if(Tim_write == 0)
                    {
                        //MUSIC_T(5);
                    }
                }
            }
			else if(Nfc_af == 4)
			{
				nfc_afinit(0);
			}
            break;
        }
    }
}
