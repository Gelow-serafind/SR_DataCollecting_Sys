/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern char command_USART1;
extern char isGet;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId GetCommandHandle;
osThreadId CANGetHandle;
osThreadId ADCGetHandle;
osThreadId toComputerHandle;
osThreadId CANSendHandle;
osMessageQId commandHandle;
osMessageQId adc1_ch5_QueueHandle;
osMessageQId adc2_ch4_QueueHandle;
osMessageQId speedLQueueHandle;
osMessageQId speedRQueueHandle;
osMessageQId speedL_owHandle;
osMessageQId speedR_owHandle;
osMessageQId adc4Handle;
osMessageQId adc3Handle;
osMessageQId adc1_ch5_owHandle;
osMessageQId adc2_ch4_owHandle;
osMessageQId ECUdataHandle;
osMessageQId batteryHandle;
osMessageQId sixHandle;
osMessageQId six_owHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartGetCommand(void const * argument);
void StartCANGet(void const * argument);
void StartADCGet(void const * argument);
void Start_toComputer(void const * argument);
void StartCANSend(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of command */
  osMessageQDef(command, 2, char);
  commandHandle = osMessageCreate(osMessageQ(command), NULL);

  /* definition and creation of adc1_ch5_Queue */
  osMessageQDef(adc1_ch5_Queue, 4, float);
  adc1_ch5_QueueHandle = osMessageCreate(osMessageQ(adc1_ch5_Queue), NULL);

  /* definition and creation of adc2_ch4_Queue */
  osMessageQDef(adc2_ch4_Queue, 4, float);
  adc2_ch4_QueueHandle = osMessageCreate(osMessageQ(adc2_ch4_Queue), NULL);

  /* definition and creation of speedLQueue */
  osMessageQDef(speedLQueue, 4, float);
  speedLQueueHandle = osMessageCreate(osMessageQ(speedLQueue), NULL);

  /* definition and creation of speedRQueue */
  osMessageQDef(speedRQueue, 4, float);
  speedRQueueHandle = osMessageCreate(osMessageQ(speedRQueue), NULL);

  /* definition and creation of speedL_ow */
  osMessageQDef(speedL_ow, 1, float);
  speedL_owHandle = osMessageCreate(osMessageQ(speedL_ow), NULL);

  /* definition and creation of speedR_ow */
  osMessageQDef(speedR_ow, 1, float);
  speedR_owHandle = osMessageCreate(osMessageQ(speedR_ow), NULL);

  /* definition and creation of adc4 */
  osMessageQDef(adc4, 1, uint8_t);
  adc4Handle = osMessageCreate(osMessageQ(adc4), NULL);

  /* definition and creation of adc3 */
  osMessageQDef(adc3, 1, uint8_t);
  adc3Handle = osMessageCreate(osMessageQ(adc3), NULL);

  /* definition and creation of adc1_ch5_ow */
  osMessageQDef(adc1_ch5_ow, 1, float);
  adc1_ch5_owHandle = osMessageCreate(osMessageQ(adc1_ch5_ow), NULL);

  /* definition and creation of adc2_ch4_ow */
  osMessageQDef(adc2_ch4_ow, 1, float);
  adc2_ch4_owHandle = osMessageCreate(osMessageQ(adc2_ch4_ow), NULL);

  /* definition and creation of ECUdata */
  osMessageQDef(ECUdata, 1, uint8_t*);
  ECUdataHandle = osMessageCreate(osMessageQ(ECUdata), NULL);

  /* definition and creation of battery */
  osMessageQDef(battery, 1, uint8_t);
  batteryHandle = osMessageCreate(osMessageQ(battery), NULL);

  /* definition and creation of six */
  osMessageQDef(six, 4, float*);
  sixHandle = osMessageCreate(osMessageQ(six), NULL);

  /* definition and creation of six_ow */
  osMessageQDef(six_ow, 1, float*);
  six_owHandle = osMessageCreate(osMessageQ(six_ow), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of GetCommand */
  osThreadDef(GetCommand, StartGetCommand, osPriorityNormal, 0, 128);
  GetCommandHandle = osThreadCreate(osThread(GetCommand), NULL);

  /* definition and creation of CANGet */
  osThreadDef(CANGet, StartCANGet, osPriorityNormal, 0, 4096);
  CANGetHandle = osThreadCreate(osThread(CANGet), NULL);

  /* definition and creation of ADCGet */
  osThreadDef(ADCGet, StartADCGet, osPriorityNormal, 0, 1024);
  ADCGetHandle = osThreadCreate(osThread(ADCGet), NULL);

  /* definition and creation of toComputer */
  osThreadDef(toComputer, Start_toComputer, osPriorityNormal, 0, 2048);
  toComputerHandle = osThreadCreate(osThread(toComputer), NULL);

  /* definition and creation of CANSend */
  osThreadDef(CANSend, StartCANSend, osPriorityNormal, 0, 2048);
  CANSendHandle = osThreadCreate(osThread(CANSend), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartGetCommand */
/**
  * @brief  Function implementing the GetCommand thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGetCommand */
void StartGetCommand(void const * argument)
{
  /* USER CODE BEGIN StartGetCommand */
	char *mode = (char*)malloc(sizeof(char)*10);
  /* Infinite loop */
  for(;;)
  {
	if(isGet == 'y')
	{
		vTaskSuspend(CANGetHandle);
		isGet = 'n';
		UI();
		taskENTER_CRITICAL();
//		printf("%s mode\n",mode);
		taskEXIT_CRITICAL();
		osDelay(1);
	}
	else if(isGet == 'o')
	{
		vTaskResume(CANGetHandle);
		isGet = 'n';
	}
  }
  /* USER CODE END StartGetCommand */
}

/* USER CODE BEGIN Header_StartCANGet */
/**
* @brief Function implementing the CANGet thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCANGet */
void StartCANGet(void const * argument)
{
  /* USER CODE BEGIN StartCANGet */
	CAN_RxHeaderTypeDef RxMessage;
	float tempdata;
//	float ADC_data;
	uint8_t  data[8]={0};
	float SpeedLlast;
	float SpeedRlast;
	SpeedLlast = 1.0;
	SpeedRlast = 1.0;
	float *six_result;
	float *six_data = (float*)malloc(sizeof(float)*8);
	uint8_t *ECU_data = (uint8_t*)malloc(sizeof(uint8_t)*8);
  /* Infinite loop */
  for(;;)
  {
	  if(!HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxMessage, data))
	  {
		  switch (RxMessage.StdId)
		  {
			  case 0x03://Left speed sensor
				tempdata = speedSensorL(data);
				if((int)(tempdata*10) > 1 && (int)(tempdata*10) != (int)(SpeedLlast*10))
				{
				  xQueueSendToBack(speedLQueueHandle,&tempdata,0);
				  xQueueOverwrite(speedL_owHandle,&tempdata);
				}
				SpeedLlast = tempdata;
				break;

			  case 0x02://Right speed sensor
				tempdata = speedSensorR(data);
				if((int)(tempdata*10) > 1 && (int)(tempdata*100) != (int)(SpeedRlast*100))
				{
				  xQueueSendToBack(speedRQueueHandle,&tempdata,0);
				  xQueueOverwrite(speedR_owHandle,&tempdata);
				}
				SpeedRlast = tempdata;
				break;
			  case 0x50://six-aix sensor
				if(data[1] == 0x51)
				{
					six_result = Six_axis_acceleration(data);
					for(uint8_t i=0;i<2;i++)
					{
						six_data[i] = six_result[i];
					}
					xQueueOverwrite(six_owHandle,&six_data);
					for(uint8_t i=0;i<2;i++)
					{
						six_data[i] = six_data[i]*120;
					}
					xQueueSendToBack(sixHandle,&six_data,0);
					free(six_result);
				}
				else if(data[1] == 0x52)
				{
					six_result = Six_axis_ang_acceleration(data);
					for(uint8_t i=2;i<5;i++)
					{
						six_data[i] = six_result[i-2];
					}
					xQueueOverwrite(six_owHandle,&six_data);
					xQueueSendToBack(sixHandle,&six_data,0);

					free(six_result);
				}
				else if(data[1] == 0x53)
				{
					six_result = Six_axis_angular(data);
					for(uint8_t i=5;i<8;i++)
					{
						six_data[i] = six_result[i-5];
					}
					xQueueOverwrite(six_owHandle,&six_data);
					xQueueSendToBack(sixHandle,&six_data,0);

					free(six_result);
				}
				break;

			  case 0x310://ECU data
				if(uxQueueMessagesWaiting(ECUdataHandle))
				{
					free(ECU_data);
				}
				for(uint8_t i=0;i<8;i++)
				{
					ECU_data[i] = data[i];
				}
				xQueueOverwrite(ECUdataHandle,&ECU_data);
				break;
			  case 0x353://ADCM2 data
				xQueueOverwrite(adc3Handle,&data[0]);
				xQueueOverwrite(adc4Handle,&data[2]);
				break;
			  default:
				  break;
		  }
	  }

	  RxMessage.StdId = 0;
	  if(RxMessage.ExtId == 0x186040F3)
	  {
		  printf("battery");
	  }
	  fflush(stdout);
	  RxMessage.ExtId = 0;

  }
  /* USER CODE END StartCANGet */
}

/* USER CODE BEGIN Header_StartADCGet */
/**
* @brief Function implementing the ADCGet thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartADCGet */
void StartADCGet(void const * argument)
{
  /* USER CODE BEGIN StartADCGet */
	float AD_Value;

  /* Infinite loop */
  for(;;)
  {
//	  temp_adc_value = GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_5,20);
	  AD_Value = GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_5,10) * 100  * 3.3/4096;
	  xQueueSendToBack(adc1_ch5_QueueHandle,&AD_Value,0);
	  xQueueOverwrite(adc1_ch5_owHandle,&AD_Value);


//	  temp_adc_value = GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_4,20);
	  AD_Value = GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_4,10) * 100  * 3.3/4096;
	  xQueueSendToBack(adc2_ch4_QueueHandle,&AD_Value,0);
	  xQueueOverwrite(adc2_ch4_owHandle,&AD_Value);


  }
  /* USER CODE END StartADCGet */
}

/* USER CODE BEGIN Header_Start_toComputer */
/**
* @brief Function implementing the toComputer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_toComputer */
void Start_toComputer(void const * argument)
{
  /* USER CODE BEGIN Start_toComputer */
	float num;
	float num2;
	uint8_t ADC_data;
	uint8_t *ECU_dataArray = (uint8_t *)malloc(sizeof(uint8_t) * 8);//�?要非常注意的是，必须使用动�?�分配函�?
	float *six;
  /* Infinite loop */
  for(;;)
  {
	  if(xQueueReceive(speedR_owHandle, &num, 0))
	  {
		  taskENTER_CRITICAL();
//  		  printf("SR %.2f\n",num);
		  taskEXIT_CRITICAL();
	  }
	  else if(xQueueReceive(speedL_owHandle, &num, 0))
	  {
		  taskENTER_CRITICAL();
//		  printf("SL %.2f\n",num);
		  taskEXIT_CRITICAL();
	  }
	  if(xQueueReceive(adc1_ch5_owHandle, &num, 0) && (xQueueReceive(adc2_ch4_owHandle, &num2, 0)))
	  {
		  taskENTER_CRITICAL();
//		  printf("ADCFL %.2f\nADCFR %.2f\n",num,num2);
		  taskEXIT_CRITICAL();
	  }
	  else
	  {

	  }
	  if(xQueueReceive(six_owHandle, &six, 0))
	  {
		  taskENTER_CRITICAL();
//		  printf("ax %d\n",(uint8_t)six[0]);
		  printf("ax %.2f\n",six[0]);
		  printf("ay %.2f\n",six[1]);
		  printf("Ox %.2f\n",six[2]);
		  printf("Oy %.2f\n",six[3]);
		  printf("Oz %.2f\n",six[4]);
		  printf("R  %.2f\n",six[5]);
		  printf("P  %.2f\n",six[6]);
		  printf("Y  %.2f\n",six[7]);
//		  free(six);//    此注释不可删。为�?么不可以释放six的内存？
		  taskEXIT_CRITICAL();
	  }

	  if(xQueueReceive(adc3Handle, &ADC_data, 0))
	  {
		  taskENTER_CRITICAL();
//		  printf("ADCBL %d\n",ADC_data);
		  taskEXIT_CRITICAL();
	  }
	  else if(xQueueReceive(adc4Handle, &ADC_data, 0))
	  {
		  taskENTER_CRITICAL();
//		  printf("ADCBR %d\n",ADC_data);
		  taskEXIT_CRITICAL();
	  }
	  if(xQueueReceive(ECUdataHandle, &ECU_dataArray, 0))
	  {
		  ECU_dataArray = (uint8_t *)malloc(sizeof(uint8_t) * 8);
		  taskENTER_CRITICAL();
		  printf("E %d",ECU_dataArray[0]);
//		  printf(" B %d",ECU_dataArray[1]);
//		  printf(" SFL %d",ECU_dataArray[2]);
//		  printf(" SFR %d",ECU_dataArray[3]);
//		  printf(" SBL %d",ECU_dataArray[4]);
//		  printf(" SBR %d",ECU_dataArray[5]);
//		  printf(" RL %d",ECU_dataArray[6]);
//		  printf(" RR %d\n",ECU_dataArray[7]);
		  taskEXIT_CRITICAL();
		  free(ECU_dataArray);
	  }
  }
  /* USER CODE END Start_toComputer */
}

/* USER CODE BEGIN Header_StartCANSend */
/**
* @brief Function implementing the CANSend thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCANSend */
void StartCANSend(void const * argument)
{
  /* USER CODE BEGIN StartCANSend */
	uint8_t data[8];
	uint8_t six_data[8];
  /* Infinite loop */
  for(;;)
  {
	pushDataToCan(&speedRQueueHandle,data,CAN_M1_DATA_A_LOC_SpeedL);
	pushDataToCan(&speedLQueueHandle,data,CAN_M1_DATA_A_LOC_SpeedR);
	pushDataToCan(&adc1_ch5_QueueHandle,data,CAN_M1_DATA_A_LOC_FLPressLow);
	pushDataToCan(&adc2_ch4_QueueHandle,data,CAN_M1_DATA_A_LOC_FRPressLow);
	taskENTER_CRITICAL();
	CAN1_Send_Test(CAN_M1_DATA_A_ID,data,8);
	taskEXIT_CRITICAL();
	vTaskDelay(5);

	pushDataToCan_six(&sixHandle,six_data);
	taskENTER_CRITICAL();
	CAN1_Send_Test(CAN_M1_DATA_A_SIX_ID,six_data,8);
	taskEXIT_CRITICAL();

	vTaskDelay(20);
  }
  /* USER CODE END StartCANSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
