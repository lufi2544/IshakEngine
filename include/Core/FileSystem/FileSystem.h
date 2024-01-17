#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"

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

		const String& GetEngineDir();
		const String& GetAssetsDir();
		const String& GetModulesDir();
		const String& GetBinariesDir();
		
	private:
		FileSystem() = default;

		void InitCoreDirs();

	private:
		String m_engineDir;
		String m_engineModulesDir;
		String m_assetsDir;
		String m_binariesDir;
		static FileSystem* m_singleton;
	};

	

}// ishak
