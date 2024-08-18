/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for appKernelTask */
osThreadId_t appKernelTaskHandle;
const osThreadAttr_t appKernelTask_attributes = {
  .name = "appKernelTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for system_timer */
osTimerId_t system_timerHandle;
const osTimerAttr_t system_timer_attributes = {
  .name = "system_timer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void app_kernel_task(void *argument);
void system_timer_callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* Create the timer(s) */
  /* creation of system_timer */
  system_timerHandle = osTimerNew(system_timer_callback, osTimerPeriodic, NULL, &system_timer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of appKernelTask */
  appKernelTaskHandle = osThreadNew(app_kernel_task, NULL, &appKernelTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint8_t str[] = "SuperEgo";
	OLED_ShowString(30, 0, str);
	OLED_ShowString(0, 15, (u8*)"infr:");
	OLED_ShowString(0, 30, (u8*)"pecd:");
	OLED_Refresh_Gram();
	
	osTimerStart(system_timerHandle, 1);//����ϵͳ�����ʱ������������Ϊ1��ϵͳ����(1ms)
	
  /* Infinite loop */
  for(;;)
  {
		infrared_oled_show();		//��ʾ�Ҷȴ�������ֵ
		pushrod_encoder_test();	//��ʾ�Ƹ˱�������ֵ
		motot_show_DC_encoder();//��ʾ���ҵ���ı���������ֵ
    osDelay(20);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief ����2ר���ڵ��pi�ջ�����
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
//	motorD.speed = 10;
//	motorC.speed = 10;
//	motor_task();//����ٶȱջ����Ŀ�������ע�⣬���Լ��Ե�ʱ��Ҫע�ʹ���
//		trace_set_motor(BASE_SPEED, BASE_SPEED);
	//�¾���ʾ����StartTask02��������У���һ���Ժ�д���κδ��붼��Ч(motor_task����ѭ��)
	osDelay(3000);
  /* Infinite loop */
  for(;;)
  {
//		fsi6_control_motorCD();
		osDelay(10);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_app_kernel_task */
/**
* @brief Function implementing the appKernelTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_app_kernel_task */
void app_kernel_task(void *argument)
{
  /* USER CODE BEGIN app_kernel_task */
	pushrod_reset();				//��λ�Ƹ�
	u32 lastWakeTime = systick;
  /* Infinite loop */
  for(;;)
  {
		vTaskDelayUntil(&lastWakeTime, 1);//������ʱ��λ���㣬switch˫�����£�app_kernel������ʱ��ܵͣ����1������Ҳ�ܼ�Ԧ�������
		app_kernel();
  }
  /* USER CODE END app_kernel_task */
}

/* system_timer_callback function */
void system_timer_callback(void *argument)
{
  /* USER CODE BEGIN system_timer_callback */
	if(!(++systick%200))//ϵͳled��ÿ��200��������˸һ��
	{
		system_led_heat();
	}
  /* USER CODE END system_timer_callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

