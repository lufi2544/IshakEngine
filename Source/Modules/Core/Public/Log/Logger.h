#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"




namespace ishak
{
	enum class CORE_API ELoggerVerbosity
	{
		None, 
		Warining,
		Error
	};

	namespace Log {

		ishak::uint8 MAX_VERBOSITY_MESSAGE_LEN = 3;
		static void Print(const char* message, ELoggerVerbosity verbosity = ELoggerVerbosity::None)
		{
			// For now no custom verbosity, just errors and warnings.
			char* verbosityMessage{ new char[MAX_VERBOSITY_MESSAGE_LEN + 1/* null character */] };

			if (verbosity == ELoggerVerbosity::Warining)
			{
				strncpy(verbosityMessage, "WAR", MAX_VERBOSITY_MESSAGE_LEN);
			}
			else if (verbosity == ELoggerVerbosity::Error)
			{
				strncpy(verbosityMessage, "ERR", MAX_VERBOSITY_MESSAGE_LEN);
			}
			else
			{
				strncpy(verbosityMessage, "LOG", MAX_VERBOSITY_MESSAGE_LEN);
			}

			std::cerr << "LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOL" << std::endl;
			std::cerr << message << std::endl;

			delete[] verbosityMessage;
		}

	}// Log
	
}