#ifndef LOG_HPP
#define LOG_HPP


#include <iostream>
#include <string>
#include <fstream>

namespace Logger
{

	class Log
	{
	public:
		static std::ofstream* oStream;

	public:
		Log();
		~Log();

	};

	extern Log logger;

	template <typename T> Log& operator<<(Log& log, T const & x)
	{
		*log.oStream << x;
		return log;
	}

}

using Logger::logger;
using Logger::operator<<;



#endif //LOG_HPP
