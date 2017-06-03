/**********************
 * test code 
 * *******************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

int main()
{
	if (logger_init()) {
		return -1;	
	}

	log_debug("This is debug, %s %d", "hello", 123);
	log_info(" This is info");
	log_warn(" This is warnning");
	log_error("This is error");
	
	logger_release();

	return 0;
}
