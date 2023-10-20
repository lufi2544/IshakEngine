
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class EngineModuleBuilder : ModuleBuilder
{
	public EngineModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
        PublicModuleDependencies = new List<string>()
        {
			"Platform", 			
            "Core",
			"SDL",
			"SDLIMAGE",
			"Renderer"
        };
    }
	
};