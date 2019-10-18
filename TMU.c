
#include "TMU.h"

static uint8 Resolution=0;  			  /* g_variable for saving resolution */
static uint8 Timer_used=0;  			  /* g_variable for saving timer to use */
static uint8 End_of_active_consumers=0;   /* g_variable for defining end of active  */
static uint16 Period_Flags_Arr[NumOfTasks];      /* g_array for incrementing time of active functions in it instead of one global for all fns  */
TMU_Buffer TMU_Buffer_arr[NumOfTasks];    /*Buffer for tasks required*/

/*
 * TMU Initialization Function
 * address of structure is passed to it which includes time_id and resolution
 * returns error code
 *
 *
 * */

EnumTMUError TMU_Init(TMU_Conf *TMU_Config_STR)
{
	EnumTMUError error =NO_ERROR;
	if(TMU_Config_STR != NULL)
	{
		Resolution= TMU_Config_STR ->Res;
		Timer_used=TMU_Config_STR->Timer_ID;
		Timer_Init(Timer_used);
	}
	else
	{
		error=TMU_INIT_INVALID_ARGUMENT;
	}
	return error;
}

/*
 * TMU start function
 * it aims to put task's information in the buffer
 * returns error code
 *
 * */
EnumTMUError TMU_Start(uint16 period_val, CONSUMER_PTR FuncName, uint8 repetation_val)
{
	EnumTMUError error = NO_ERROR;
	if(FuncName!=NULL)
	{
		if(End_of_active_consumers< NumOfTasks)
		{
			TMU_Buffer_arr[End_of_active_consumers].FUNC=FuncName;
			TMU_Buffer_arr[End_of_active_consumers].PERIOD=period_val;
			TMU_Buffer_arr[End_of_active_consumers].REPETATION=repetation_val;
			End_of_active_consumers++;
			Timer_Start(Timer_used);

		}
		else
		{
			error = TMU_START_FULL_BUFFER;
		}
	}
	else
	{
		error = TMU_START_INVALID_ARGUMENT;

	}

	return  error;
}

/*
 * TMU stop function
 * it aims to delete task's information from the buffer
 * returns error code
 *
 * */

EnumTMUError TMU_Stop(CONSUMER_PTR Func_name)
{EnumTMUError error = NO_ERROR;
uint8 Count ;

if(Func_name!=NULL)
{
	for( Count=0;Count<End_of_active_consumers;Count++)
	{
		if(TMU_Buffer_arr[Count].FUNC == Func_name)
		{
			TMU_Buffer_arr[Count] =TMU_Buffer_arr[End_of_active_consumers-1];
			End_of_active_consumers--;
		}
		else
		{
			error=TMU_STOP_INVALID_ARGUMENT;
		}
	}
}
else
{
	error=TMU_STOP_INVALID_ARGUMENT;
}
return error;
}

/*
 * TMU Dispatcher
 * it aims to execute each task every certain period
 * returns none
 *
 * */

void TMU_Dispatcher(void)
{
	uint8 Disp_Count;
	for(Disp_Count =0 ; Disp_Count<End_of_active_consumers;Disp_Count++)
	{
		if( (Period_Flags_Arr[Disp_Count] >=(TMU_Buffer_arr[Disp_Count].PERIOD) ) && (Period_Flags_Arr[Disp_Count] <(2*(TMU_Buffer_arr[Disp_Count].PERIOD)) ) )
		{

			(*TMU_Buffer_arr[Disp_Count].FUNC)();
			if(	TMU_Buffer_arr[End_of_active_consumers].REPETATION==One_Shot)
			{
				TMU_Stop(TMU_Buffer_arr[Disp_Count].FUNC);
				Disp_Count--;
			}
			else
			{
			}
			Period_Flags_Arr[Disp_Count]=0;
		}
	}



}

/*
 * G_flags increase function
 * it aims to increase each flag related to each task instead of depending on one flag to mitigate errors and overrun as possible
 * returns error code
 *
 * */

void G_Flag_Increase(void)
{
	uint8 Flag_Count;


	for(Flag_Count=0;Flag_Count<End_of_active_consumers;Flag_Count++)
	{
		Period_Flags_Arr[Flag_Count]++;
	}
}

ISR(TIMER0_OVF_vect)
{
	static uint32 counter=0;
	counter++;
	if(counter%(4*Resolution) == 0)
	{
		G_Flag_Increase();
	}

}
