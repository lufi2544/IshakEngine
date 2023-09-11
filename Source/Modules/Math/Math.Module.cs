
using IshakBuildTool.Project.Modules;


public class MathModuleBuilder : ModuleBuilder
{
	public MathModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{
		// We are using the GLM lib for the math library.
		OnlyHeaderLib = true;
	}
	
};