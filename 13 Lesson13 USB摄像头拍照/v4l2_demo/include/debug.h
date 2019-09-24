#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>

#if defined DEBUG
#define debug(str) do{\
			printf("[%s]file: \033[32;49;1m%s\033[0m func: \033[33;1m%s\033[0m line: \033[31;1m%d\033[0m  : %s \n",\
					"------debug-----", __FILE__, __func__,\
				       	__LINE__, str);\
			}while(0)
#else
#define debug(str) do {}while(0)					
#endif

#endif
