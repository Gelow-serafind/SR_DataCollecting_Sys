/*
 * SR_isCAN.h
 *
 *  Created on: Jul 6, 2023
 *      Author: ad
 */



#ifndef INC_SR_H_
#define INC_SR_H_

#define CAN_M1_DATA_A_ID 0x153
#define CAN_M1_DATA_A_SIX_ID 0x311
#define CAN_M1_DATA_B_ID 0x07

#define CAN_M1_DATA_A_LOC_SpeedL      	0
#define CAN_M1_DATA_A_LOC_SpeedR      	1
#define CAN_M1_DATA_A_LOC_SoC         	2
#define CAN_M1_DATA_A_LOC_BatVoltage  	3
#define CAN_M1_DATA_A_LOC_FLPressLow  	4
#define CAN_M1_DATA_A_LOC_FLPressHigh 	5
#define CAN_M1_DATA_A_LOC_FRPressLow  	6
#define CAN_M1_DATA_A_LOC_FRPressHigh 	7


#endif /* INC_SR_ISCAN_H_ */



float speedSensorL(uint8_t  data[]);
float speedSensorR(uint8_t  data[]);
void UI();
void CAN1_Send_Test(int id,int *data,int data_size);
float Can_data_process(uint8_t *data,int data_size);
void pushDataToCan(osMessageQId *handle,uint8_t *data,uint8_t loc);
void pushDataToCan_six(osMessageQId *handle,uint8_t *data);
float* Six_axis_acceleration(uint8_t *data);
float* Six_axis_ang_acceleration(uint8_t *data);
float* Six_axis_angular(uint8_t *data);

