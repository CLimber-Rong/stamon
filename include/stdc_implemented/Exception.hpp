/*
	Name: 
	Copyright: 
	Author: 
	Date: 24/08/23 18:09
	Description: 
*/
#ifndef EXCEPTION
#define EXCEPTION

#include"stmlib.hpp"
//异常处理的实现

char* ExceptionMessage;

/*
	CATCH的使用方法：
	CATCH {
		//在这里写上捕捉异常的代码
	}
	THROW(message)用于抛出异常，异常信息为message
	ERROR是异常信息
*/ 

#define THROW(message) ExceptionMessage = (char*)message;	//THROW当中的message是char*
#define CATCH if(ExceptionMessage!=NULL)
#define ERROR ExceptionMessage

#endif
