
using IshakBuildTool.Project.Modules;


public class SolParserModuleBuilder : ModuleBuilder
{
	public SolParserModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
		// Sol binding from lua to C++.
		OnlyHeaderLib = true;
	}
	
};