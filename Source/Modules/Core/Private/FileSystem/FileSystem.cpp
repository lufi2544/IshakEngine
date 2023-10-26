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

	void FileSystem::InitCoreDirs()
	{
		namespace fs = std::filesystem;

		fs::path currentDir{ fs::current_path() };
		String currentDirString{ currentDir.string().c_str()};
		fs::path engineRootDir;

		// Running from .exe
		if (currentDirString.Find("Binaries"))
		{
			m_engineDir = String(currentDir.parent_path().string().c_str());
		}
		else
		{
			// Running from .vcxprj( VS project file ) located in the IntermediateFolder
			m_engineDir = String(currentDir.parent_path().parent_path().string().c_str());
		}

		m_engineModulesDir = String(std::string(std::string(m_engineDir.c_str()) + "\\" + "Source" + "\\" + "Modules" + "\\").c_str());
		m_assetsDir = String(m_engineDir + "\\" + "Content" + "\\");
	}
}// ishak
