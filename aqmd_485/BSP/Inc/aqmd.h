#ifndef __AQMD_H_
#define __AQMD_H_

#include "main.h"

#define READ_HOLD_REGISTER				0x03			//读保持寄存器
#define WRITE_SINGLE_REGISTER			0x06			//写单个寄存器
#define REG_SET_PWM								0x0042		//占空比设置寄存器
#define IS_RECEIVE_WRITE_RETURN		0         //是否接收写寄存器操作的返回数据

typedef enum{
	undecided, 	//读取数据未定
	dataError, 	//读取数据错误
	correct,			//读取数据正确
}AqmdDataStateTypeDef;

extern AqmdDataStateTypeDef aqmdDataState;

void aqmd_receive_one_byte_callback(uint8_t recByte);
void aqmd_debug_log(void);
void aqmd_test(void);





















#endif
