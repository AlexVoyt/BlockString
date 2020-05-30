@echo off
clear
echo Compiling...
cl /Zi /Od main.cpp
IF %ERRORLEVEL% == 0 (
    echo SUCCSESS
    main.exe
) ELSE (
    echo ERROR DURING COMPILING
)
