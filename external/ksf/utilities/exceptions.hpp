#ifndef exceptions_hpp__
#define exceptions_hpp__

#include <string>
#include <cstring>

#include <iostream>
#include <exception>

// Uiltities
#if !defined(_WIN32) || !defined(WIN32)
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

// Custom exception throwers, just so we can keep the file name
// and line whenenever we get an error
#define throw_exception_without_msg(EX)		 					\
	throw EX(static_cast<const char*>(__FILENAME__), __LINE__)		
#define throw_exception_with_msg(EX, MSG)			 			\
	throw EX(MSG, static_cast<const char*>(__FILENAME__), __LINE__)

// This is a little trick that we can use to have function "overloading" macros
// taken from https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments.
// Basically, when three arguments are placed in throw_exception_internal, 
// the third argument is used to call the macro
// (i.e: throw_exception_internal(1, 2, FOO, ...) = FOO and 
// throw_exception_internal(1, FOO1, FOO2, ...) = FOO2)
//
// As for throw_exception, once the desired macro has been selected
// it will use it along with (__VA_ARGS__)
// (i.e: if __VA_ARGS__ = 1,2 then throw_exception(1, 2, FOO, FOO1) = FOO(1,2))
#define throw_exception_internal(_1,_2, NAME, ...) NAME
#define throw_exception(...) 									\
	throw_exception_internal( 									\
		__VA_ARGS__, 											\
		throw_exception_with_msg, 								\
		throw_exception_without_msg 						    \
	)(__VA_ARGS__)

#define register_exception(EX, MSG) 							\
	class EX: public exceptions::base_exception 	    		\
    { 															\
    public: 													\
        EX(std::string m, const char *f, int l): 				\
            exceptions::base_exception(							\
				std::string(#EX) + ": " +  m,f,l) {}			\
        EX(const char *f, int l): 								\
            exceptions::base_exception(							\
				std::string(#EX) + ": " +  MSG,f,l) {}			\
    }
#define get_object_name(a) (typeid(a).name())

// Usage:
// Just copy this header into your project and include it wherever you need it
// After you include it, you can create a custom exception by just using the
// register_exception(EX, MSG) macro. i.e:
//		register_exception(my_exception, "this is my exception's message");
//
// And to reaise it (although you can totally just use throw) you can use
// throw_exception_without_msg or throw_exception_with_msg. i.e:
//		throw_exception_with_msg(my_exception, "this happened");
//		or
//		throw_exception_without_msg(my_exception);
// PS: You can also enclose your exception(s) in their own namespace if you
// would want to.

namespace exceptions
{
	class base_exception: public std::exception
	{
	protected:
		const std::string msg;
		const char *file;
		const int line;

	public:
		base_exception(
			std::string msg,
			const char *file, 
			int line
		):
			msg("[" + std::string(file) + ":" + std::to_string(line) + "]: " + msg),
			file(file),
			line(line)
		{}

		const char *what() const throw()
		{
			return msg.c_str();
		}

		const char *get_file() const
		{
			return this->file;
		}

		int get_line() const
		{
			return this->line;
		}
	};
}

#endif