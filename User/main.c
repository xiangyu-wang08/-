#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"


int angle = 999  ;
int power = 0 ;
int leftbj =0;               //左扳机
int rightbj = 0;             //右扳机



int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Serial_Init();		//串口初始化
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "yg");
	OLED_ShowString(3, 1, "bj");
	
	
	
	
	
	
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
				}
			}
			else if  ( Serial_RxPacket[0] =='B'  )				//上浮下沉命令
			{
				if  (   sscanf(Serial_RxPacket,"BJ %d %d",&templeftbj,&temprightbj)==2   )
				{
					leftbj =templeftbj;
					rightbj = temprightbj;
				}
			}
			else
			{
			}
			
			
			
			
			Serial_SendString(Serial_RxPacket);               	//回传上位机并换行
			Serial_SendString("\r\n");
			
			
			OLED_ShowString(2, 1, "                ");
			OLED_ShowSignedNum(2, 1,angle,3);				
			OLED_ShowSignedNum(2, 6,power,3);						//OLED 显示水平方向摇杆数值
			
			OLED_ShowString(4, 1, "                ");
			OLED_ShowSignedNum(4, 1, leftbj,3);	
			OLED_ShowSignedNum(4, 6, rightbj,3);					//OLED 显示上浮下沉扳机数值
			
			
			Serial_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
	}
}
