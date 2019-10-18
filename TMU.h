
#ifndef TMU_H_
#define TMU_H_

#include "TIMER.h"
#include "TMU_pconfg.h"
#include "TMU_preconfg.h"
#include <avr/interrupt.h>

#define Periodic    1
#define One_Shot    2

typedef void(*CONSUMER_PTR)(void);

//typedef enum {INVALID_ARGUMENT=-1,FULL_BUFFER=-2,NO_ERROR=1}error_code;
//typedef enum {TMU_INIT=-100,TIMER_START=-200,TMU_STOP=300,TMU_DISPATCHER=400}service_id;
typedef enum {TMU_INIT_INVALID_ARGUMENT,TIMER_START_FULL_BUFFER,TMU_START_INVALID_ARGUMENT,TMU_STOP_INVALID_ARGUMENT,TMU_START_FULL_BUFFER,NO_ERROR}EnumTMUError;



typedef struct{
	uint8 Timer_ID;
	uint8 Res;
}TMU_Conf;

extern TMU_Conf TMU_Config_STR;

typedef struct
{
	volatile uint16 PERIOD;
	volatile uint8 REPETATION;
	CONSUMER_PTR FUNC;

}TMU_Buffer;

extern TMU_Buffer TMU_Buffer_arr[NumOfTasks];

EnumTMUError TMU_Init(TMU_Conf *TMU_Config_STR);

EnumTMUError TMU_Start(uint16 period_val, CONSUMER_PTR FuncName, uint8 repetation_val);

EnumTMUError TMU_Stop(CONSUMER_PTR Func_name);

void TMU_Dispatcher();
void G_Flag_Increase(void);

#endif
