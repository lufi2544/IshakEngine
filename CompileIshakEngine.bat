@echo off
cd /d "Source\BuildTool\IshakBuildTool\bin\Debug\net6.0"
"C:\IshakEngine\Source\BuildTool\IshakBuildTool\bin\Debug\net6.0\IshakBuildTool.exe" -r "C:\\IshakEngine" -pt "Application" -bm "compile"
pause