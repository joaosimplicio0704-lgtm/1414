@echo off
setlocal enableextensions enabledelayedexpansion
cd /d "%~dp0"

echo Searching for MSBuild...

set "MSBUILD="
for %%I in (Community Professional Enterprise BuildTools) do (
    set "CAND=C:\Program Files\Microsoft Visual Studio\2022\%%I\MSBuild\Current\Bin\MSBuild.exe"
    if exist "!CAND!" set "MSBUILD=!CAND!"
)
if not defined MSBUILD (
    for %%I in (Community Professional Enterprise BuildTools) do (
        set "CAND=C:\Program Files (x86)\Microsoft Visual Studio\2022\%%I\MSBuild\Current\Bin\MSBuild.exe"
        if exist "!CAND!" set "MSBUILD=!CAND!"
    )
)

if not defined MSBUILD (
    echo [ERROR] MSBuild.exe not found.
    echo Install Visual Studio 2022 ^(Desktop development with C++ workload^) or Build Tools 2022.
    exit /b 1
)

echo Using: !MSBUILD!
echo.
"!MSBUILD!" fortnite-external.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /m /v:minimal
if %ERRORLEVEL% neq 0 (
    echo.
    echo [BUILD FAILED]
    exit /b 1
)

echo.
echo [BUILD OK] build\fortnite-external.exe
if exist "build\fortnite-external.exe" (
    "build\fortnite-external.exe"
    echo.
    echo [EXIT CODE %ERRORLEVEL%]
) else (
    echo [WARN] exe not found at expected path
)
endlocal
