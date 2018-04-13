/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include "periph/gpio.h"
#include "rtctimers-millis.h"
#include "thread.h"


static kernel_pid_t process_pid;
//static msg_t pracss_msg;

#define MY_PROCESS_SIZE (1024)

#define ENABLE_DEBUG (1)
#include "debug.h"

//#include "shell_commands.h"
//#include "shell.h"
#include "periph/pm.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "opt3001.h"


uint32_t millisecond_last_press=0;

opt3001_t opt3001;
opt3001_measure_t measure_date; 
	

////////////////////////////////////////////////////////////
void interupt_f(void *arg){
	(void)arg;
	
	opt3001_measure(&opt3001, &measure_date);
	printf(" luminocity = %lu\n **** interupt****", measure_date.luminocity);
	 gpio_toggle(GPIO_PIN(PORT_B,0));
	
}
////////////////////////////////////////////////////////////

void *process_treade(void *arg){
	(void)arg; // присвоение если используется конструкция void *arg 
	//msg_t message;
	gpio_set(GPIO_PIN(PORT_B,0));
	while(1){
		 
		//msg_receive(&message); // сообщегне процесса 
		if((rtctimers_millis_now()-millisecond_last_press)>1000){
			millisecond_last_press = rtctimers_millis_now();
			
			opt3001_measure(&opt3001, &measure_date);
			printf(" luminocity = %lu\n", measure_date.luminocity);
			
			if(1020<measure_date.luminocity){
				
				printf("luminocity = %lu lux is to high !\n", measure_date.luminocity);
				
			}else 
			{
				printf("luminocity = %lu lux is to low !\n", measure_date.luminocity);
			}
			
			
		
		}
				
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////
/*
static int print_echo(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i) {
        printf("\"%s\"", argv[i]);
    }
    puts("");

    return 0;
}
*/
//////////////////////////////////////////////////////////////////////////////


int main(void)
{
	rtctimers_millis_init(); // инит таймера 
	
	float lim_high=1000.0;
	float lim_low = 100.0;
	
	pm_init();
	pm_prevent_sleep = 1; // запрет энергосбережения 
	
	

	opt3001.i2c = 1;
	opt3001_init(&opt3001);
	write_sensor_lim(&opt3001,lim_high,lim_low);
	opt3001_measure(&opt3001, &measure_date);
	
	

	    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
		printf("This board features a(n) %s MCU.\n", RIOT_MCU);
		

//	printf(" luminocity = %lu\n", measure_date.luminocity);
	
	gpio_init(GPIO_PIN(PORT_B,0),GPIO_OUT);
	gpio_set(GPIO_PIN(PORT_B,0));
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	gpio_init_int(GPIO_PIN(PORT_A,7),GPIO_IN_PU,GPIO_FALLING,interupt_f,NULL); // gpio portb pin 28 
	////////////////////////////////////////////////////////////////////////////////////////////////////
		char stack[MY_PROCESS_SIZE];
		process_pid = thread_create(stack, MY_PROCESS_SIZE,
									THREAD_PRIORITY_MAIN-1, THREAD_CREATE_STACKTEST,
									process_treade, NULL,"out process");
									
									
	
	// shell/////////////////////////////////
//	char line_buf[SHELL_DEFAULT_BUFSIZE];
//	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	////////////////////////////////////
	
    return 0;
}
