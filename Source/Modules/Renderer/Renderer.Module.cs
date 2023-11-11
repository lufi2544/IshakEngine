
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class RendererModuleBuilder : ModuleBuilder
{
	public RendererModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
        PublicModuleDependencies = new List<string>()
        {
            "Core",
			"SDL",
			"SDLIMAGE",
			"Platform",
			"IMGUI",
			"ECS"
        };
    }
	
};