#include "FileSystem/FileSystem.h"


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

#ifndef LINUX // WINDOWS
		// Running from .exe
		if (currentDirString.Find("Binaries"))
		{
			m_engineDir = String(currentDir.parent_path().string().c_str());
		}
		else
		{
			// Running from .vcxprj( VS project file ) located in the IntermediateFolder
			m_engineDir = String(currentDir.parent_path().parent_path().parent_path().string().c_str() + "/");
		}

#else  // LINUX
		if(currentDirString.Find("Binaries"))
		{
			m_engineDir =  String(currentDir.parent_path().parent_path().parent_path().c_str()) + "/" ;
		}
	

#endif // !LINUX
	   
		// See if windows accepts / in stead of double back slash
	  
		m_engineModulesDir = String(std::string(std::string(m_engineDir.c_str()) + "Source" + "/" + "Modules" + "/").c_str());
		m_assetsDir = String(m_engineDir + "Content" + "/");

#if DEBUG_ENGINE
		m_binariesDir = String(m_engineDir + "Binaries/Engine/Debug/");
#else
		m_binariesDir = String(m_engineDir + "Binaries/Engine/Release/");
#endif  // DEBUG_ENGINE

		std::cout << m_binariesDir.c_str() << std::endl;
		std::cout << m_engineModulesDir.c_str() << std::endl;
	}
}// ishak
