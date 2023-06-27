
using IshakBuildTool.Project.Modules;


public class RendererModuleBuilder : ModuleBuilder
{
	public RendererModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{


		PublicModuleDependencies.AddRange(new[]
		{
			"Core"
		});
		
	}
	
};