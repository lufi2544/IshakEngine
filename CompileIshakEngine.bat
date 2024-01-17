@echo off

:: Set the path to IshakBuildTool.exe relative to the script's directory
set "buildToolPath=%~dp0Source\BuildTool\IshakBuildTool\bin\Debug\net6.0\IshakBuildTool.exe"

:: Check if IshakBuildTool.exe exists
if not exist "%buildToolPath%" (
    echo Error: IshakBuildTool.exe not found at "%buildToolPath%"
    pause
    exit /b 1
)

:: Run IshakBuildTool.exe with the provided arguments
"%buildToolPath%" -r "C:\Programming\IshakEngine" -r "C:\\Programming\\IshakEngine" -pt "Application" -bm "compile"
pause
