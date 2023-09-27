
using IshakBuildTool.Project.Modules;


public class SDLIMAGEModuleBuilder : ModuleBuilder
{
	public SDLIMAGEModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
		ThirdPartyDLLName = "SDL2_imaged.dll";
		ThirdPartyDLLImportName = "SDL2_imaged.lib";	
	}
	
};