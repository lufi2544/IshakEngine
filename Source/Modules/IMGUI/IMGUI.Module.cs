
using IshakBuildTool.Project.Modules;
using System.Collections.Generic;

public class IMGUIModuleBuilder : ModuleBuilder
{
	public IMGUIModuleBuilder()
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