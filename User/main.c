#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"


int angle = 999  ;
int power = 0 ;
int leftbj =0;               //左扳机	  下降
int rightbj = 0;             //右扳机     上升

int spmove =0;             //判断水平运动状态，0停止，1向前，2向后，3向左，4向右
int szmove =0; 				//判断竖直运动状态，0停止，1上升，2下降

void spmovecmd(void)												//判断水平运动状态函数
{
	
	if ( angle==999 || power <=10 )			//判断水平停止
	{
		spmove =0;
	}
	else if ( angle>-45  && angle<=45 )     	//判断水平向右
	{
		spmove = 4;
	}
	else if  ( angle>45  && angle <=135  )		//判断水平向前
	{
		spmove = 1;
	}
	else if ( (angle>135  && angle <=180)   ||  (angle < -135)   )		//判断水平向左
	{
		spmove = 3;
	}
	else
	{
		spmove = 2;							//判断水平向后
	}
}


void szmovecmd(void)												//判断竖直运动状态函数
{
	if ( rightbj >10 )
	{
		szmove =1;            //上升
	}
	else if (leftbj  >10)
	{
		szmove =2;            //下降
	}
	else
	{
		szmove =0;             //停止
	}
}





int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Serial_Init();		//串口初始化
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "sp");
	OLED_ShowString(3, 1, "sz");
	
	
	
	
	
	
	while (1)
	{
		if (Serial_RxFlag == 1)		//如果接收到数据包
		{
			
			
			int tempangle;
			int temppower;
			int templeftbj;
			int temprightbj;
			
			if (   Serial_RxPacket[0] =='J'    )                  //水平方向控制
			{
				if  (  sscanf(Serial_RxPacket,"JSV1 %d %d",&tempangle,&temppower)==2        )
				{
					angle =tempangle;
					power = temppower;
					
					
					spmovecmd();                                       //判断水平运动状态
					if (spmove==0)  			//停止
					{
						OLED_ShowString(2, 1, "                ");
						OLED_ShowString(2,1,"STOP");                          //OLED显示控制动作
					}
					else if (spmove==1)			//前进
					{
						OLED_ShowString(2, 1, "                ");
						OLED_ShowString(2,1,"GO");
					}
					else if (spmove==2)			//后退
					{
						OLED_ShowString(2, 1, "                ");
						OLED_ShowString(2,1,"BACK");
					}
					else if (spmove==3)			//左转
					{
						OLED_ShowString(2, 1, "                ");
						OLED_ShowString(2,1,"Turn Left");
					}
					else 						//右转
					{
						OLED_ShowString(2, 1, "                ");
						OLED_ShowString(2,1,"Turn Right");
					}
					
					
					
					
				}
			}
			else if  ( Serial_RxPacket[0] =='B'  )				//上浮下沉命令
			{
				if  (   sscanf(Serial_RxPacket,"BJ %d %d",&templeftbj,&temprightbj)==2   )
				{
					leftbj =templeftbj;
					rightbj = temprightbj;
					
					
					
					szmovecmd();					//判断竖直运动状态
					if (  szmove ==1  )				//上升
					{
						OLED_ShowString(4, 1, "                ");
						OLED_ShowString(4,1,"UP");
					}
					else if (szmove ==2  )    		//下降
					{
						OLED_ShowString(4, 1, "                ");
						OLED_ShowString(4,1,"DOWN");
					}
					else 							//停止
					{
						OLED_ShowString(4, 1, "                ");
						OLED_ShowString(4,1,"STOP");
					}
					
				}
			}
			else
			{
			}
			
			
			
			Serial_SendString(Serial_RxPacket);               	//回传上位机并换行,用于测试STM32数据接收
			Serial_SendString("\r\n");
			

			Serial_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
	}
}
