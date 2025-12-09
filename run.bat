@echo off

REM cd adv01 && clang++ main.cpp -std=c++20 -o main.exe && main.exe && del main.exe && cd..
REM cd adv02 && dotnet run main.cs && cd..
REM cd adv03 && clang main.c -std=c99 -o main.exe && main.exe && del main.exe && cd..
REM cd adv04 && python main.py && cd..
REM cd adv05 && call :setup_vs_env && nvcc main.cu -o main.exe -arch=native && main.exe && del main.exp main.lib main.exe && cd..
REM cd adv06 && java main.java && cd..
REM cd adv07 && compile.bat && cd..
REM cd adv08 && node main.js && cd..
cd adv09 && dotnet publish -c Release -r win-x64 --self-contained true /p:PublishAot=true main.cs -o . && main.exe && del main.exe main.pdb && cd..

:setup_vs_env
where cl.exe >nul 2>&1 && exit /b 0
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
    exit /b 0
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
    exit /b 0
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
    exit /b 0
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
    exit /b 0
)
exit /b 1