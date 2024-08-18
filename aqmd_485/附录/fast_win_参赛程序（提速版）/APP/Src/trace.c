#include "trace.h"
extern short DC_encoder[2];
extern osThreadId_t appKernelTaskHandle;

#define trace_get_motor_left_encoder	DC_encoder[1]
#define trace_get_motor_right_encoder	DC_encoder[0]
#define trace_clr_encoder()			do{motorD.clear_encoder();\
																	motorC.clear_encoder();\
															}while(0)

#define DEV_RIGHT		0				//�ٶ���ƫ��
#define INIT_SPEED  5000
#define BASE_SPEED	3000		//�����ٶ�
#define CORRECT1		1400		//�����ٶ�1
#define CORRECT2		6000		//�����ٶ�2
#define turn				5000		//ԭ��ת���ٶ�

#define trace_motor_turn_right()		trace_set_motor(BASE_SPEED,-BASE_SPEED)
//#define TURN_RIGHT_AM				2600		//��ת90��
#define TURN_RIGHT_AM				2000		//��ת45��
#define TURN_RIGHT_FOR_AM		317		//��תǰ��ֱ�е���
															
#define trace_motor_turn_left()			trace_set_motor(-BASE_SPEED,BASE_SPEED)
#define TURN_LEFT_AM				2000		//��ת40��
#define TURN_LEFT_FOR_AM		317		//��תǰ��ֱ�е���

#define TURN_135_AM					3000		//��ת135��

#define TURN_45_AM					200		//��ת45�ȵ�ֱ�е���															
#define TURN_22_AM					650

#define FINAL_CORRECT				1950
#define TURN_xiuzheng_AM					50
															
#define PUSHROD							420  //�����Ƹ����������λ��

#define trace_motor_turn_left_180()			trace_set_motor(-BASE_SPEED,BASE_SPEED)


typedef enum{
	forward_put_ore			//����ǰ�����ſ�ʯ����ͷ���ٴ�ǰ����·�ڴ�����ת
}Intersection_action;
Intersection_action intersection_action = forward_put_ore;//��ʼ����Ϊ������ǰ��������ʯ

//case���0b..�ȶ����Ƴ������蹴ѡ���̵�GNU C��׼

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
	
	//����ѭ��
	switch(trace_3){
		case 0b111:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b110:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b011:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b010:
			xunji_state1 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b001:
			xunji_state1 = trace_right;
			trace_set_motor(BASE_SPEED+CORRECT1, BASE_SPEED-CORRECT1);
//			rtos_printf("С�����ҹ�\r\n");
			break;
		case 0b100:
			xunji_state1 = trace_left;
			trace_set_motor(BASE_SPEED-CORRECT1, BASE_SPEED+CORRECT1);
//			rtos_printf("С�������\r\n");
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
	
	//����ѭ��
	switch(trace_3){
		case 0b111:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b110:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b011:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b010:
			xunji_state2 = trace_forward;
			trace_set_motor(BASE_SPEED, BASE_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b001:
			xunji_state2 = trace_right;
			trace_set_motor(BASE_SPEED+CORRECT2, BASE_SPEED-CORRECT2);
//			rtos_printf("С�����ҹ�\r\n");
			break;
		case 0b100:
			xunji_state2 = trace_left;
			trace_set_motor(BASE_SPEED-CORRECT2, BASE_SPEED+CORRECT2);
//			rtos_printf("С�������\r\n");
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
	
	//����ѭ��
	switch(trace_3){
		case 0b111:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b110:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b011:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b010:
			xunji_state2 = trace_forward;
			trace_set_motor(INIT_SPEED, INIT_SPEED);
//			rtos_printf("��ֱ��\r\n");
			break;
		case 0b001:
			xunji_state2 = trace_right;
			trace_set_motor(INIT_SPEED+CORRECT2, INIT_SPEED-CORRECT2);
//			rtos_printf("С�����ҹ�\r\n");
			break;
		case 0b100:
			xunji_state2 = trace_left;
			trace_set_motor(INIT_SPEED-CORRECT2, INIT_SPEED+CORRECT2);
//			rtos_printf("С�������\r\n");
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

//��ת����ʱ��
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
		while(HAL_GPIO_ReadPin(TURN_180_CHECK_GPIO_Port, TURN_180_CHECK_Pin) == 0){//�м俿��̽ͷδѹ������
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
	pushrod_set_postion(PUSHROD);//��ʱ����
	osDelay(2000);
}

//����ѭ���̶���·�̣�����·���ж�
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
	trace,		//ѭ��
	move			//·�ڵ�һЩ����
}App_state;
App_state app_state = move;

typedef enum{
	state0,
	state1,		//ѭ��
	state2,			//·�ڵ�һЩ����
	state3,
	state4
}Movement;
Movement movement = state0;//Ĭ��Ϊ����

u32 time_cnt = 0;//��ʱ��

short cur;
void app_kernel(void){
	u8 cur = infrared_get_data();
	u8 intersection = ((0x10 & cur)>>3) | (0x01 & cur);
	
	//����״̬��
	if(app_state == move || intersection){//�������·������붯��ģʽ��������Ӧ�Ķ���
		app_state = move;//����Ϊ����ģʽ��ͬʱֹͣѭ��
		switch(movement){
			
			case state0://�߳����������߳�֮��ע��Ҫ�ָ�Ϊѭ��ģʽ
			///////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////
				trace_clr_encoder();
				while(trace_get_motor_left_encoder < 250 && trace_get_motor_right_encoder < 250){//��ǰֱ��һС��
					trace_set_motor(BASE_SPEED,BASE_SPEED);
					osDelay(1);
				}
				trace_set_motor(0,0);
				osDelay(500);
				
				trace_clr_encoder();
				while(trace_get_motor_right_encoder - trace_get_motor_left_encoder < 2500){//��ת
					trace_motor_turn_left();
					osDelay(1);
				}
				
				trace_set_motor(0,0);
				osDelay(500);
				
				trace_clr_encoder();
				while(trace_get_motor_left_encoder < 1500 && trace_get_motor_right_encoder < 1500){//��ǰֱ��һС��
					trace_set_motor(BASE_SPEED,BASE_SPEED);
					osDelay(1);
				}
				
				trace_set_motor(0,0);
				osDelay(500);
				
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					cur = infrared_get_data();
					trace_control3(cur);//����ѭ����ֱ������·��	
				}
				
				movement = state1;//תΪ��һ��״̬
				break;
			
			case state1: //�ſ�ʯ
				
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
//				while(trace_get_motor_left_encoder < 700 && trace_get_motor_right_encoder < 700){//��ǰֱ��һС��
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
					trace_set_motor(-BASE_SPEED, -BASE_SPEED);//������ֱ������·��
				}
				trace_set_motor(0,0);
				osDelay(500);
				trace_turn_right();//��ת����һ��·��
				movement = state2;//�´�����·�ڵ�״̬
				app_state = trace;//��ʼ����ѭ��
				break;
			case state2://�����ſ�ʯ֮��ĵ�һ��·�ڣ�ֱ���߹�ȥ������
				trace_set_motor(0,0);
				osDelay(500);
				app_kernel_xunji(500);
				osDelay(500);
				movement = state3;
				app_state = trace;//��ʼ����ѭ��
				break;
			case state3://�ÿ�ʯ
				trace_set_motor(0,0);
				osDelay(500);
			
			pushrod_set_postion(PUSHROD);//��ʱ����
			osDelay(2000);
				
				trace_turn_left_45();
				osDelay(500);
				app_kernel_xunji(750);
			
				servos_set_angle(INIT_ANGLE);//צ�Ӽ�ס��ʯ
				osDelay(500);
				
				//�õ���ʯ�ˣ���ʼ����
				//��ͷ֮ǰ�����Ⱥ���һ�ξ���
			
				pushrod_set_postion(570);//�õ���ʯ�����Ȱ��Ƹ�̧��
			
			
				trace_set_motor(-BASE_SPEED, -BASE_SPEED);
				osDelay(400);
				trace_set_motor(0, 0);
				osDelay(500);
				
			
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					trace_set_motor(-BASE_SPEED, -BASE_SPEED);//������ֱ������·��
				}
				trace_set_motor(0,0);
				
				pushrod_reset();//��λ�Ƹ�
				
				trace_turn_right();//ת�䣬������һ��·��
				osDelay(500);
				
				////////////////////////////////////////////////////////////////////////////////////////
				trace_clr_encoder();//��ת����һС��
				while(trace_get_motor_left_encoder - trace_get_motor_right_encoder < FINAL_CORRECT){
					trace_motor_turn_right();
					osDelay(1);
				}
				osDelay(500);
				
				app_kernel_xunji(1450);//������ѭ��һС�Σ���ʱ��Ҫ���·�ڣ�Ŀ����ʹ���·�ڵĴ������߹�·��·
				osDelay(500);
				
				//����ѭ����ֱ������·��Ϊֹ
				while(!(((0x10 & infrared_get_data())>>3) | (0x01 & infrared_get_data()))){
					cur = infrared_get_data();
					trace_control2(cur);
					osDelay(1);
				}
				trace_set_motor(0,0);
				osDelay(500);
				movement = state4;//�������һ��״̬
				break;
			case state4://�����ڶ����ſ�ʯ��·��
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
					trace_set_motor(-BASE_SPEED, -BASE_SPEED);//������ֱ������·��
				}
				trace_set_motor(0,0);
				osDelay(500);
				trace_turn_right();//��ת����һ��·��
				
				app_kernel_xunji(5000);
				
				while(1){
					osDelay(1);
				}
		}
		
	}
	
	//��������ѭ��
	if(app_state == trace){
		trace_control(cur);
	}
}














