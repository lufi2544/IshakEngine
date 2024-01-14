@echo off
setlocal

:: Set the path to IshakBuildTool.exe relative to the script's directory
set "buildToolPath=%~dp0Source\BuildTool\IshakBuildTool\bin\Debug\net6.0\IshakBuildTool.exe"

:: Check if IshakBuildTool.exe exists
if not exist "%buildToolPath%" (
    echo Error: IshakBuildTool.exe not found at "%buildToolPath%"
pause
    
)

:: Run IshakBuildTool.exe with the provided arguments
"%buildToolPath%" -r "C:\IshakEngine" -pt "Makefile" -bm "generate"

pause