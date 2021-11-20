@set VC_DIR=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC
@set VC_BIN_DIR=%VC_DIR%\Tools\MSVC\14.29.30133\bin\Hostx64\x64
@set VC_AUX_DIR=%VC_DIR%\Auxiliary\Build
@set CC_FILES=main.cpp grid.cpp

:: Compile code
if [%VCVARSALL_RAN%]==[] (
	set VCVARSALL_RAN=1
	call "%VC_AUX_DIR%\vcvarsall.bat" x64
)
"%VC_BIN_DIR%\cl.exe" /std:c++17 /EHsc %CC_FILES% /link /out:main.exe

pause
