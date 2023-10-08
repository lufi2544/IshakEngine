
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class GameModuleBuilder : ModuleBuilder
{
	public GameModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
        PublicModuleDependencies = new List<string>()
        {
            "Core"
        };
    }
	
};