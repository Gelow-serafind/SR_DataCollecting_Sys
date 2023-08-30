/*
 * SR_isCAN.c
 *
 *  Created on: Jul 6, 2023
 *      Author: ad
 */

#include "main.h"
extern osMessageQId speedLQueueHandle;

float speedSensorL(uint8_t  data[])
{
	short AngleSpeedL;
	float RotarySpeedL;
	if(data[6] != 255)
	{
	  AngleSpeedL = (uint32_t) (data[6] << 24) | (data[5] << 16) |
			  (data[4] << 8) | data[3];  //采样时间间隔(100ms)内编码器值变化量
	  RotarySpeedL = AngleSpeedL * 0.58594;  //r/min
	  return RotarySpeedL * 0.0861;  //km/h
	}
}
float speedSensorR(uint8_t  data[])
{
	short AngleSpeedL;
	float RotarySpeedL;
	if(data[6] != 255)
	{
	  AngleSpeedL = (uint32_t) (data[6] << 24) | (data[5] << 16) |
			  (data[4] << 8) | data[3];  //采样时间间隔(100ms)内编码器值变化量
	  RotarySpeedL = AngleSpeedL * 0.14648;  //r/min
	  return RotarySpeedL * 0.0861;  //km/h
	}
}

void UI()
{
	printf("=============SR data collecting system===========");
	printf("\n\n\n");
	printf("=============Welcome my SRer=====================\n");
	printf("please input your command:\n");
	printf("\'f\' is the Foucs Mode");
}

void CAN1_Send_Test(int id,int *data,int data_size)
{
	CAN_TxHeaderTypeDef        TxMessage;
	uint32_t can_milbox0;			//指向函数将返回的变量的pTxMailbox指针
	TxMessage.IDE = CAN_ID_STD;     //设置ID类型
	TxMessage.StdId = id;
	TxMessage.RTR = CAN_RTR_DATA;   //设置传送数据帧
	TxMessage.DLC = data_size;      //设置数据长度
	if(HAL_CAN_AddTxMessage(&hcan1, &TxMessage, data, &can_milbox0) != HAL_OK) 	//发送can数据
	{
        Error_Handler();
	}
}

float Can_data_process(uint8_t *data,int data_size)
{
	if(data[1] == 0x03)//the data of speed sensor of left
	{
		return speedSensor(data);
	}
	else if(data[1] == 0x02)//the data of speed sensor of right
	{

	}
	else
	{
	}
}

void pushDataToCan(osMessageQId *handle,uint8_t *data,uint8_t loc)
{
	float num;
	if(xQueueReceive(*handle, &num, 0))
	{
		data[loc] = (uint8_t)num;
		num = 0;
	}
	else
	{

	}
}

void pushDataToCan_six(osMessageQId *handle,uint8_t *data)
{
	float *num;
	if(xQueueReceive(*handle, &num, 0))
	{
		for(uint8_t i=0;i<8;i++)
		{
			data[i] = (uint8_t)num[i];
		}
		return 1;
	}
	else
		return 0;
}

float* Six_axis_acceleration(uint8_t *data)
{
	float *xyz = (float*)malloc(sizeof(float)*3);
	xyz[0] = (int16_t)((int16_t)data[3]<<8|data[2])/32768.0*16;
	xyz[1] = (int16_t)((int16_t)data[5]<<8|data[4])/32768.0*16;
	xyz[2] = (int16_t)((int16_t)data[7]<<8|data[6])/32768.0*16;
	return xyz;
}
float* Six_axis_ang_acceleration(uint8_t *data)
{
	float *OxOyOz = (float*)malloc(sizeof(float)*3);
	OxOyOz[0] = (int16_t)((int16_t)data[3]<<8|data[2])/32768.0*2000;
	OxOyOz[1] = (int16_t)((int16_t)data[5]<<8|data[4])/32768.0*2000;
	OxOyOz[2] = (int16_t)((int16_t)data[7]<<8|data[6])/32768.0*2000;
	return OxOyOz;
}
float* Six_axis_angular(uint8_t *data)
{
	float *RPY = (float*)malloc(sizeof(float)*3);
	RPY[0] = (int16_t)((int16_t)data[3]<<8|data[2])/32768.0*180;
	RPY[1] = (int16_t)((int16_t)data[5]<<8|data[4])/32768.0*180;
	RPY[2] = (int16_t)((int16_t)data[7]<<8|data[6])/32768.0*180;
	return RPY;
}
