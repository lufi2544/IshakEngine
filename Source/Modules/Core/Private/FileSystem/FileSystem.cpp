#include "FileSystem/FileSystem.h"


namespace ishak {

	FileSystem* FileSystem::m_singleton = nullptr;

	FileSystem& FileSystem::Get()
	{
		if(m_singleton == nullptr)
		{
			m_singleton = new FileSystem();
		}

		return *m_singleton;
	}

	FileSystem::~FileSystem()
	{
		delete m_singleton;
		m_singleton = nullptr;
	}
	String FileSystem::GetEngineDir()
	{
		if(m_engineDir.IsEmpty())
		{
			
		}

		return m_engineDir;
	}

	void FileSystem::SetEngineDir()
	{
		namespace fs = std::filesystem;

		fs::path currentDir{ fs::current_path() };
		std::string currentDirString{ currentDir.string() };
		fs::path engineRootDir;

		// Running from .exe
		if (currentDirString.find("Binaries") != std::string::npos)
		{
			m_engineDir = String(currentDir.parent_path().string().c_str());
		}
		else
		{
			// Loading from .vcxprj( VS project file ) located in the IntermediateFolder
			m_engineDir = String(currentDir.parent_path().parent_path().string().c_str());
		}


		std::string modulesDir{ engineRootDir.string() + "\\" + "Source" + "\\" + "Modules" };


	}
}// ishak
