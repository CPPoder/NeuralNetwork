#include "Source\Framework\Log.hpp"


namespace Logger
{

	Log::Log()
	{
		oStream = new std::ofstream("./log.txt");
	}


	Log::~Log()
	{
		delete oStream;
		oStream = nullptr;
	}

	std::ofstream* Log::oStream = nullptr;

	Log logger = Log();

}





