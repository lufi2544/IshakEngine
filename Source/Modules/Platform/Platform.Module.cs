
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class PlatformModuleBuilder : ModuleBuilder
{
	public PlatformModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
		PublicModuleDependencies = new List<string>()
		{			
			"SDL"						
		};
	}
	
};