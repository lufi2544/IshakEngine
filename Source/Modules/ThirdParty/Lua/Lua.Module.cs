
using IshakBuildTool.Project.Modules;


public class LuaModuleBuilder : ModuleBuilder
{
	public LuaModuleBuilder()
	{
		SetModuleDependencies();		
	}

	void SetModuleDependencies()
	{		
		ThirdPartyDLLName = "lua53.dll";
		ThirdPartyDLLImportName = "liblua53.a";
	}
	
};