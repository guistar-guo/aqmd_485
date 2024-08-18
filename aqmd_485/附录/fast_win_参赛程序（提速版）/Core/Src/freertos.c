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
	
	osTimerStart(system_timerHandle, 1);//开启系统软件定时器，设置周期为1个系统节拍(1ms)
	
  /* Infinite loop */
  for(;;)
  {
		infrared_oled_show();		//显示灰度传感器的值
		pushrod_encoder_test();	//显示推杆编码器的值
		motot_show_DC_encoder();//显示左右电机的编码器的数值
    osDelay(20);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief 任务2专用于电机pi闭环控制
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
//	motorD.speed = 10;
//	motorC.speed = 10;
//	motor_task();//电机速度闭环核心控制任务，注意，调试极性的时候，要注释此行
//		trace_set_motor(BASE_SPEED, BASE_SPEED);
	//温警提示：在StartTask02这个任务中，这一行以后，写的任何代码都无效(motor_task是死循环)
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
	pushrod_reset();				//复位推杆
	u32 lastWakeTime = systick;
  /* Infinite loop */
  for(;;)
  {
		vTaskDelayUntil(&lastWakeTime, 1);//绝对延时（位运算，switch双管齐下，app_kernel的运行时间很低，因此1个节拍也能驾驭这个任务）
		app_kernel();
  }
  /* USER CODE END app_kernel_task */
}

/* system_timer_callback function */
void system_timer_callback(void *argument)
{
  /* USER CODE BEGIN system_timer_callback */
	if(!(++systick%200))//系统led灯每隔200个节拍闪烁一次
	{
		system_led_heat();
	}
  /* USER CODE END system_timer_callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

