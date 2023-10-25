#include "CoreMinimal.h"
#include "CoreConfig.h"

// STD
#include <filesystem>

namespace ishak {

	class CORE_API FileSystem
	{		

	public:
		static FileSystem& Get();

		FileSystem& operator=(FileSystem&&) = delete;
		FileSystem& operator=(const FileSystem&) = delete;
		FileSystem(FileSystem&&) = delete;
		FileSystem(const FileSystem&) = delete;
		~FileSystem();

		String GetEngineDir();
		
	private:
		FileSystem() = default;

		void SetEngineDir();

	private:
		String m_engineDir;
		String m_engineModulesDir;
		static FileSystem* m_singleton;
	};

	

}// ishak