/*
 * DIO_cnfg.c
 *
 *  Created on: Sep 20, 2019
 *      Author: Rafaat
 */

#include "DIO_cnfg.h"


Dio_Config Dio_arr[NUM_OF_PINS] ={
		{PORT_C,PIN0,OUTPUT,LOW,NA,INITI},
		{PORT_C,PIN1,OUTPUT,LOW,NA,INITI},


};

extern Dio_Config Dio_arr[NUM_OF_PINS];
