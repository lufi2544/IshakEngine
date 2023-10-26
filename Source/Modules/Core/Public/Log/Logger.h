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


	
	class CORE_API Logger
	{
	public:
		Logger() = default;

		void Log(const String& message , ELoggerVerbosity verb = ELoggerVerbosity::None);
		void Draw();

	private:
		TArray<String> m_allLogs;
	};
	
	extern CORE_API Logger* GLogger;


#define ISHAK_LOG(msg) GLogger->Log({msg});
}