@echo off
REM ---------------------------------------------------------------------------
REM  Copyright (c) 2010 Semiconductor Components Industries, LLC
REM  (d/b/a ON Semiconductor). All rights reserved.
REM  
REM  This code is the property of ON Semiconductor and may not be redistributed
REM  in any form without prior written permission from ON Semiconductor. The
REM  terms of use and warranty for this code are covered by contractual
REM  agreements between ON Semiconductor and the licensee.
REM ---------------------------------------------------------------------------
REM  $Revision: 1.4 $
REM  $Date: 2010/12/01 16:44:37 $
REM ---------------------------------------------------------------------------

setlocal

if "%INCLUDE%"=="" (
    if exist "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT" (
        call "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT"
    ) else (
        echo Could not find VCVARS32.BAT
        exit /B 1
    )
)

if exist Obj rmdir /S /Q Obj
if exist Exe rmdir /S /Q Exe

mkdir Obj
mkdir Exe

cl /nologo /c hidlib.cpp test.cpp /FoObj\
if errorlevel 1 goto error

cl /nologo Obj\hidlib.obj Obj\test.obj setupapi.lib /FeExe\test.exe
if errorlevel 1 goto error

echo Build succeeded
exit /b

:error
echo Build failed
exit /b 1
