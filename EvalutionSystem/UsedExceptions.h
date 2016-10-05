#pragma once
#include <stdlib.h>
#include <exception>

#define EXCEPTION_LOG( EX_CALL ) EX_CALL.set_file(__FILE__).set_line(__LINE__).set_func(__FUNCTION__)

class UsedExceptions  
{
public:
	UsedExceptions() = delete;
	~UsedExceptions() = delete;

	class ExtendedException : public std::exception
	{
	protected:
		const char *file = "NO_FILE";
		size_t line = 0;
		const char *function = "NO_FUNCTION";
		const char *error_message = nullptr;
		char *log_message = nullptr;
	public:
		ExtendedException() : std::exception() { error_message = ""; };
		ExtendedException(const char *m) : std::exception() { error_message = m; };
		~ExtendedException() { if (log_message) delete log_message; };
		ExtendedException &set_file(const char *f) { file = f; return *this; };
		ExtendedException &set_line(size_t l) { line = l; return *this; };
		ExtendedException &set_func(const char *f) { function = f; return *this; };
		const char *get_message();
	};

	class NotImplementedYet : public ExtendedException
	{
	public:
		NotImplementedYet() : ExtendedException("A called function was not implemented yet.") {};
		~NotImplementedYet() {};
	};
};

