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
static msg_t pracss_msg;

#define MY_PROCESS_SIZE (1024)

#define ENABLE_DEBUG (1)
#include "debug.h"

#include "shell_commands.h"
#include "shell.h"
#include "periph/pm.h"




uint32_t millisecond_last_press=0;

void btn_tolse(void *arg){
	(void)arg;
	
	
	if((rtctimers_millis_now()-millisecond_last_press)>200){
			
		msg_send(&pracss_msg,process_pid); // сообщение процесса , id процесса 
		//gpio_toggle(GPIO_PIN(PORT_B,0));
		millisecond_last_press = rtctimers_millis_now();
		
	}
	
}

void *process_treade(void *arg){
	(void)arg; // присвоение если используется конструкция void *arg 
	msg_t message;
	gpio_set(GPIO_PIN(PORT_B,0));
	while(1){
		 
		msg_receive(&message); // сообщегне процесса 
		gpio_toggle(GPIO_PIN(PORT_B,0));
		DEBUG("GEPIO_READE = %d\n",gpio_read(GPIO_PIN(PORT_B,0)));
				
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
static const shell_command_t shell_commands[] = {
///	{ "echo", "prints the input command", print_echo },
    { NULL, NULL, NULL }
};

int main(void)
{
	rtctimers_millis_init();
	
	pm_init();
	pm_prevent_sleep = 1; // запрет энергосбережения 
	
	puts("Hello World!");
	    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);
	

	
	gpio_init(GPIO_PIN(PORT_B,0),GPIO_OUT);
	gpio_set(GPIO_PIN(PORT_B,0));
	
	gpio_init_int(GPIO_PIN(PORT_B,1),GPIO_IN_PU,GPIO_FALLING,btn_tolse,NULL);
	
		char stack[MY_PROCESS_SIZE];
		process_pid = thread_create(stack, MY_PROCESS_SIZE,
									THREAD_PRIORITY_MAIN-1, THREAD_CREATE_STACKTEST,
									process_treade, NULL,"out process");
									
									
	
	// shell/////////////////////////////////
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	////////////////////////////////////
	
    return 0;
}
