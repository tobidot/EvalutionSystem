#include "UsedExceptions.h"
#include <string>

const char * UsedExceptions::ExtendedException::get_message()
{ 
	if (!log_message)
	{
		// errorMessage + filename + funcname + line + extraText + \0
		size_t size = strlen(error_message) + strlen(file) + strlen(function) + 5 + 25 + 1;
		log_message = new char[size];
		sprintf_s(log_message,size,"%s \nIn File '%s' in line %s : %5d ",error_message,file,function,line);
	}
	return log_message;
}
