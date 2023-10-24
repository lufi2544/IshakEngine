
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class DTESTModuleBuilder : ModuleBuilder
{
	public DTESTModuleBuilder()
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