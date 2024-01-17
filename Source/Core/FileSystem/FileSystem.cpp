#include "Core/FileSystem/FileSystem.h"


namespace ishak {

	FileSystem* FileSystem::m_singleton = nullptr;

	FileSystem& FileSystem::Get()
	{
		if(m_singleton == nullptr)
		{
			m_singleton = new FileSystem();
			m_singleton->InitCoreDirs();
		}

		return *m_singleton;
	}

	FileSystem::~FileSystem()
	{
		delete m_singleton;
		m_singleton = nullptr;
	}
	const String& FileSystem::GetEngineDir()
	{	
		return m_engineDir;
	}

	const String& FileSystem::GetAssetsDir()
	{
		return m_assetsDir;
	}

	const String& FileSystem::GetModulesDir()
	{
		return m_engineModulesDir;
	}

	const String& FileSystem::GetBinariesDir()
	{
		return m_binariesDir;
	}

	void FileSystem::InitCoreDirs()
	{
		namespace fs = std::filesystem;

		fs::path currentDir{ fs::current_path() };
		String currentDirString{ currentDir.string().c_str()};
		fs::path engineRootDir;

#ifdef WINDOWS
		// Running from .exe
		if (currentDirString.Find("Binaries"))
		{
			m_engineDir = String(currentDir.parent_path().string().c_str());
	}
		else
		{
			// Running from .vcxprj( VS project file ) located in the IntermediateFolder			
			m_engineDir = String(currentDir.parent_path().parent_path().string().c_str()) + "/Engine/";
		}
#endif // WINDOWS

#ifdef LINUX
		if (currentDirString.Find("Binaries"))
		{
			m_engineDir = String(currentDir.parent_path().parent_path().parent_path().c_str()) + "/";
		}
#endif// LINUX

	   // TODO Solve the problem with the back slash vs the forward slash.
		// See if windows accepts / in stead of double back slash	 
		m_assetsDir = String(m_engineDir + "Content" + "/");
		m_binariesDir = String(m_engineDir + "Binaries/");

		std::cout << m_binariesDir.c_str() << std::endl;
		std::cout << m_engineModulesDir.c_str() << std::endl;
	}
}// ishak
