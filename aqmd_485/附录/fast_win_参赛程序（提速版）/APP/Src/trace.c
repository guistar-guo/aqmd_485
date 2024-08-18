#include "trace.h"
extern short DC_encoder[2];
extern osThreadId_t appKernelTaskHandle;

#define trace_get_motor_left_encoder	DC_encoder[1]
#define trace_get_motor_right_encoder	DC_encoder[0]
#define trace_clr_encoder()			do{motorD.clear_encoder();\
																	motorC.clear_encoder();\
															}while(0)

#define DEV_RIGHT		0				//速度右偏差
#define INIT_SPEED  5000
#define BASE_SPEED	3000		//基础速度
#define CORRECT1		1400		//修正速度1
#define CORRECT2		6000		//修正速度2
#define turn				5000		//原地转弯速度

#define trace_motor_turn_right()		trace_set_motor(BASE_SPEED,-BASE_SPEED)
//#define TURN_RIGHT_AM				2600		//右转90度
#define TURN_RIGHT_AM				2000		//右转45度
#define TURN_RIGHT_FOR_AM		317		//右转前的直行调整
															
#define trace_motor_turn_left()			trace_set_motor(-BASE_SPEED,BASE_SPEED)
#define TURN_LEFT_AM				2000		//左转40度
#define TURN_LEFT_FOR_AM		317		//左转前的直行调整

#define TURN_135_AM					3000		//左转135度

#define TURN_45_AM					200		//左转45度的直行调整															
#define TURN_22_AM					650

#define FINAL_CORRECT				1950
#define TURN_xiuzheng_AM					50
															
#define PUSHROD							420  //设置推杆上升的最高位置

#define trace_motor_turn_left_180()			trace_set_motor(-BASE_SPEED,BASE_SPEED)


typedef enum{
	forward_put_ore			//继续前进，放矿石，掉头，再次前进至路口处，左转
}Intersection_action;
Intersection_action intersection_action = forward_put_ore;//初始动作为：继续前进，方矿石

//case后加0b..等二进制常量，需勾选工程的GNU C标准

typedef enum{
	trace_forward,
	trace_left,
	trace_right
}Xunji_state;
Xunji_state xunji_state1 = trace_forward;
Xunji_state xunji_state2 = trace_forward;
Xunji_state xunji_state3 = trace_forward;

static void trace_control(u8 cur)
{
	u8 trace_3 = (0x0E & cur)>>1;
	
	//正常循迹
	switch(trace_3){
		case 0b111:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b110:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b011:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b010:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b001:
			xunji_state1 = trace_right;
			trace_set_motor(BASE_SPEED+CORRECT1, BASE_SPEED-CORRECT1);
//			rtos_printf("小幅度右拐\r\n");
			break;
		case 0b100:
			xunji_state1 = trace_left;
			trace_set_motor(BASE_SPEED-CORRECT1, BASE_SPEED+CORRECT1);
//			rtos_printf("小幅度左拐\r\n");
			break;
		
		case 0b000:
			switch(xunji_state1){
				case trace_forward:
					trace_set_motor(BASE_SPEED, BASE_SPEED);
					break;
				case trace_right:
					trace_set_motor(BASE_SPEED+CORRECT1, BASE_SPEED-CORRECT1);
					break;
				case trace_left:
					trace_set_motor(BASE_SPEED-CORRECT1, BASE_SPEED+CORRECT1);
					break;
			}
	}
}	

static void trace_control2(u8 cur)
{
	u8 trace_3 = (0x0E & cur)>>1;
	
	//正常循迹
	switch(trace_3){
		case 0b111:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b110:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b011:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b010:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b001:
			xunji_state2 = trace_right;
			trace_set_motor(BASE_SPEED+CORRECT2, BASE_SPEED-CORRECT2);
//			rtos_printf("小幅度右拐\r\n");
			break;
		case 0b100:
			xunji_state2 = trace_left;
			trace_set_motor(BASE_SPEED-CORRECT2, BASE_SPEED+CORRECT2);
//			rtos_printf("小幅度左拐\r\n");
			break;
		
		case 0b000:
			switch(xunji_state2){
				case trace_forward:
					trace_set_motor(BASE_SPEED, BASE_SPEED);
					break;
				case trace_right:
					trace_set_motor(BASE_SPEED+CORRECT2, BASE_SPEED-CORRECT2);
					break;
				case trace_left:
					trace_set_motor(BASE_SPEED-CORRECT2, BASE_SPEED+CORRECT2);
					break;
			}
	}
}	

static void trace_control3(u8 cur)
{
	u8 trace_3 = (0x0E & cur)>>1;
	
	//正常循迹
	switch(trace_3){
		case 0b111:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b110:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b011:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b010:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("走直线\r\n");
			break;
		case 0b001:
			xunji_state2 = trace_right;
			trace_set_motor(INIT_SPEED+CORRECT2, INIT_SPEED-CORRECT2);
//			rtos_printf("小幅度右拐\r\n");
			break;
		case 0b100:
			xunji_state2 = trace_left;
			trace_set_motor(INIT_SPEED-CORRECT2, INIT_SPEED+CORRECT2);
//			rtos_printf("小幅度左拐\r\n");
			break;
		
		case 0b000:
			switch(xunji_state2){
				case trace_forward:
					trace_set_motor(INIT_SPEED, INIT_SPEED);
					break;
				case trace_right:
					trace_set_motor(INIT_SPEED+CORRECT2, INIT_SPEED-CORRECT2);
					break;
				case trace_left:
					trace_set_motor(INIT_SPEED-CORRECT2, INIT_SPEED+CORRECT2);
					break;
			}
	}
}	

//右转（耗时）
void trace_turn_right(void){
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(1000);
		while(trace_get_motor_right_encoder < TURN_LEFT_FOR_AM && trace_get_motor_left_encoder < TURN_RIGHT_FOR_AM){
			trace_set_motor(BASE_SPEED, BASE_SPEED);
			osDelay(1);
		}
		
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(500);
		while(trace_get_motor_left_encoder - trace_get_motor_right_encoder < TURN_RIGHT_AM){
			trace_motor_turn_right();
			osDelay(1);
		}
		while((infrared_get_data() & 0x04) == 0){
			trace_motor_turn_right();
		}
		trace_set_motor(0,0);
}

void trace_turn_left(void){
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(1000);
		while(trace_get_motor_right_encoder < TURN_LEFT_FOR_AM && trace_get_motor_left_encoder < TURN_LEFT_FOR_AM){
			trace_set_motor(BASE_SPEED, BASE_SPEED);
			osDelay(1);
		}
		
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(500);
		while(trace_get_motor_right_encoder - trace_get_motor_left_encoder < TURN_LEFT_AM){
			trace_motor_turn_left();
			osDelay(1);
		}
		
		while((infrared_get_data() & 0x02) == 0){
			trace_motor_turn_left();
		}
		trace_set_motor(0,0);
}

void trace_turn_180(void){
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(500);
		while(trace_get_motor_right_encoder - trace_get_motor_left_encoder < TURN_135_AM){
			trace_motor_turn_left();
			osDelay(1);
		}
		osDelay(500);
		while(HAL_GPIO_ReadPin(TURN_180_CHECK_GPIO_Port, TURN_180_CHECK_Pin) == 0){//中间靠右探头未压到黑线
			trace_motor_turn_left();
		}
		trace_set_motor(0,0);
}

void trace_turn_left_45(void){
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(500);
	
		while(trace_get_motor_right_encoder < TURN_45_AM && trace_get_motor_left_encoder < TURN_45_AM){
			trace_set_motor(BASE_SPEED, BASE_SPEED);
			osDelay(1);
		}
	
		trace_clr_encoder();
		while(trace_get_motor_right_encoder - trace_get_motor_left_encoder < TURN_22_AM){
			trace_motor_turn_left();
			osDelay(1);
		}
		while((infrared_get_data() & 0x04) == 0){
			trace_motor_turn_left();
		}
		trace_set_motor(0,0);
}

void trace_turn_right_45(void){
		trace_set_motor(0,0);
		trace_clr_encoder();
		osDelay(500);
	
		while(trace_get_motor_right_encoder < TURN_45_AM && trace_get_motor_left_encoder < TURN_45_AM){
			trace_set_motor(BASE_SPEED, BASE_SPEED);
			osDelay(1);
		}
	
		while(trace_get_motor_left_encoder - trace_get_motor_right_encoder < TURN_22_AM){
			trace_motor_turn_right();
			osDelay(1);
		}
		while((infrared_get_data() & 0x04) == 0){
			trace_motor_turn_right();
		}
		trace_set_motor(0,0);
}

void tace_pushrod(void){
	pushrod_set_postion(PUSHROD);//耗时操作
	osDelay(2000);
}

//继续循迹固定的路程，忽略路口判断
void app_kernel_xunji(int x){
	short cur;
	trace_set_motor(0,0);
					trace_clr_encoder();
					osDelay(500);
				while(trace_get_motor_left_encoder < x && trace_get_motor_right_encoder < x){
					cur = infrared_get_data();
					trace_control2(cur);
					osDelay(1);
				}
				trace_set_motor(0,0);
}

typedef enum{
	trace,		//循迹
	move			//路口的一些动作
}App_state;
App_state app_state = move;

typedef enum{
	state0,
	state1,		//循迹
	state2,			//路口的一些动作
	state3,
	state4
}Movement;
Movement movement = state0;//默认为动作

u32 time_cnt = 0;//计时器

short cur;
void app_kernel(void){
	u8 cur = infrared_get_data();
	u8 intersection = ((0x10 & cur)>>3) | (0x01 & cur);
	
	//动作状态机
	if(app_state == move || intersection){//如果遇到路口则进入动作模式，进行相应的动作
		app_state = move;//设置为动作模式，同时停止循迹
		switch(movement){
			
			case state0://走出启动区，走出之后，注意要恢复为循迹模式
			///////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////
				trace_clr_encoder();
				while(trace_get_motor_left_encoder < 250 && trace_get_motor_right_encoder < 250){//向前直走一小段
					trace_set_motor(BASE_SPEED,BASE_SPEED);
					osDelay(1);
				}
				trace_set_motor(0,0);
				osDelay(500);
				
				trace_clr_encoder();
				while(trace_get_motor_right_encoder - trace_get_motor_left_encoder < 2500){//左转
					trace_motor_turn_left();
					osDelay(1);
				}
				
				trace_set_motor(0,0);
				osDelay(500);
				
				trace_clr_encoder();
				while(trace_get_motor_left_encoder < 1500 && trace_get_motor_right_encoder < 1500){//向前直走一小段
					trace_set_motor(BASE_SPEED,BASE_SPEED);
					osDelay(1);
				}
				
				trace_set_motor(0,0);
				osDelay(500);
				
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					cur = infrared_get_data();
					trace_control3(cur);//提速循迹，直到遇到路口	
				}
				
				movement = state1;//转为下一个状态
				break;
			
			case state1: //放矿石
				
			///////////////////////
			
				trace_set_motor(0,0);
				trace_clr_encoder();
				osDelay(500);
				pushrod_set_postion(50);
				osDelay(500);
				while(trace_get_motor_left_encoder < 1600 && trace_get_motor_right_encoder < 1600){
					cur = infrared_get_data();
					trace_control(cur);
					osDelay(1);
				}
				trace_set_motor(0,0);
				osDelay(500);
				
//				trace_clr_encoder();
//				while(trace_get_motor_left_encoder < 700 && trace_get_motor_right_encoder < 700){//向前直走一小段
//					trace_set_motor(BASE_SPEED,BASE_SPEED);
//					osDelay(1);
//				}
			trace_set_motor(BASE_SPEED,BASE_SPEED);
				osDelay(600);
				trace_set_motor(0,0);
				osDelay(500);
				servos_set_angle(120);
				osDelay(500);
				
				
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					trace_set_motor(-BASE_SPEED, -BASE_SPEED);//倒车，直到遇到路口
				}
				trace_set_motor(0,0);
				osDelay(500);
				trace_turn_right();//右转到下一个路口
				movement = state2;//下次遇到路口的状态
				app_state = trace;//开始正常循迹
				break;
			case state2://遇到放矿石之后的第一个路口，直接走过去忽略它
				trace_set_motor(0,0);
				osDelay(500);
				app_kernel_xunji(500);
				osDelay(500);
				movement = state3;
				app_state = trace;//开始正常循迹
				break;
			case state3://拿矿石
				trace_set_motor(0,0);
				osDelay(500);
			
			pushrod_set_postion(PUSHROD);//耗时操作
			osDelay(2000);
				
				trace_turn_left_45();
				osDelay(500);
				app_kernel_xunji(750);
			
				servos_set_angle(INIT_ANGLE);//爪子夹住矿石
				osDelay(500);
				
				//拿到矿石了，开始返回
				//掉头之前，首先后退一段距离
			
				pushrod_set_postion(570);//拿到矿石后，首先把推杆抬高
			
			
				trace_set_motor(-BASE_SPEED, -BASE_SPEED);
				osDelay(400);
				trace_set_motor(0, 0);
				osDelay(500);
				
			
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					trace_set_motor(-BASE_SPEED, -BASE_SPEED);//倒车，直到遇到路口
				}
				trace_set_motor(0,0);
				
				pushrod_reset();//复位推杆
				
				trace_turn_right();//转弯，进入下一个路口
				osDelay(500);
				
				////////////////////////////////////////////////////////////////////////////////////////
				trace_clr_encoder();//右转调整一小段
				while(trace_get_motor_left_encoder - trace_get_motor_right_encoder < FINAL_CORRECT){
					trace_motor_turn_right();
					osDelay(1);
				}
				osDelay(500);
				
				app_kernel_xunji(1450);//先正常循迹一小段，此时不要检测路口，目的是使检测路口的传感器走过路口路
				osDelay(500);
				
				//继续循迹，直到遇到路口为止
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					cur = infrared_get_data();
					trace_control2(cur);
					osDelay(1);
				}
				trace_set_motor(0,0);
				osDelay(500);
				movement = state4;//进入最后一个状态
				break;
			case state4://遇到第二个放矿石的路口
				trace_set_motor(0,0);
				pushrod_set_postion(200);
				osDelay(500);
				trace_turn_left();
			
				trace_set_motor(0,0);
				trace_clr_encoder();
				osDelay(500);
				while(trace_get_motor_left_encoder < 1300 && trace_get_motor_right_encoder < 1300){
					cur = infrared_get_data();
					trace_control(cur);
					osDelay(1);
				}
				
				trace_set_motor(BASE_SPEED,BASE_SPEED);
				osDelay(600);
			
				trace_set_motor(0,0);
				osDelay(500);
				servos_set_angle(120);
				osDelay(500);
				
				
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					trace_set_motor(-BASE_SPEED, -BASE_SPEED);//倒车，直到遇到路口
				}
				trace_set_motor(0,0);
				osDelay(500);
				trace_turn_right();//右转到下一个路口
				
				app_kernel_xunji(5000);
				
				while(1){
					osDelay(1);
				}
		}
		
	}
	
	//主线任务：循迹
	if(app_state == trace){
		trace_control(cur);
	}
}














