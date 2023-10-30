#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"

namespace ishak
{
	enum class CORE_API ELoggerVerbosity
	{
		Temp, 
		Warning,
		Error
	};


	/** Global Logger class for the Engine.
	 *  TODO: Store logs and output to a file.
	 */
	struct CORE_API Logger
	{
		struct LogData
		{
			ELoggerVerbosity verb{ ELoggerVerbosity::Temp };
			String msg;			
		};

	public:
		Logger() = default;

		void Log(const String& message , ELoggerVerbosity verb = ELoggerVerbosity::Temp);
		void Draw();

	private:
		TArray<LogData> m_allLogs;
	};	

	CORE_API Logger* GetGlobalLogger();
	

#define GLog GetGlobalLogger()

#define ISHAK_LOG(msg, Verb) GLog->Log(msg, ELoggerVerbosity::Verb);

}