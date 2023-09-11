
using IshakBuildTool.Project.Modules;


public class SDLModuleBuilder : ModuleBuilder
{
	public SDLModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{		
		ThirdPartyDLLName = "SDL2.dll";
		ThirdPartyDLLImportName = "SDL2.lib";
	}
	
};