
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class CoreModuleBuilder : ModuleBuilder
{
	public CoreModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
		PublicModuleDependencies = new List<string>()
		{
			"Renderer"			
		};
	}
	
};