
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class LaunchModuleBuilder : ModuleBuilder
{
	public LaunchModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
        PublicModuleDependencies = new List<string>()
        {			
            "Core", 
			"Engine",
			"Renderer"
        };
    }
	
};