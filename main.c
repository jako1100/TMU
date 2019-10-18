


#include "timer.h"
#include "dio.h"
#include "REGISTER.h"
#include "TMU.h"

void Toggle_lED1();
void Toggle_lED2();
void Toggle_lED3();

int main(void){

	Dio_init();
	TMU_Init(&TMU_Config_STR);
	TMU_Start(300,Toggle_lED1,Periodic);
	TMU_Start(600,Toggle_lED2,Periodic);
	TMU_Start(900,Toggle_lED3,One_Shot);

	sei();


	while(1)
	{
		TMU_Dispatcher();


	}
	return 0;
}


void Toggle_lED1()
{
	PORTC_REG ^=1<<PIN0;
}
void Toggle_lED2()
{
	PORTC_REG ^=1<<PIN1;
}

void Toggle_lED3()
{
	PORTC_REG ^=1<<PIN2;

}


