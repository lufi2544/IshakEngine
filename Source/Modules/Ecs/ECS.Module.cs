
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class ECSModuleBuilder : ModuleBuilder
{
	public ECSModuleBuilder()
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