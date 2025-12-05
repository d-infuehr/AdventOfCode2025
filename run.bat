@echo off

REM cd adv01 && clang++ main.cpp -std=c++20 -o main.exe && main.exe && del main.exe && cd..
REM cd adv02 && dotnet run main.cs
REM cd adv03 && clang main.c -std=c99 -o main.exe && main.exe && del main.exe && cd..
REM cd adv04 && python main.py && cd..
cd adv05 && call :setup_vs_env && nvcc main.cu -o main.exe -arch=native && main.exe && del main.exp main.lib main.exe && cd..

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