
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;


public class GameLoopModuleBuilder : ModuleBuilder
{
	public GameLoopModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
		PublicModuleDependencies = new List<string>()
		{			
			"SDL",						
			"Platform"
		};
	}
	
};