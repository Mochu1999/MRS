@echo off
rem This .bat loops through the specified subfolders to clean build files, it ignores source code and resources.
rem It deletes Visual Studio solutions. Solutions are meant to be later created with createSolutions.bat

echo CLEANING ALL PROJECT SUBDIRECTORIES:

rem Iterates through directories /d (not files). %%D is the current subfolder, (*) everything in %%D
for /d %%D in (*) do (
	rem Only directories with a CMakeLists.txt will have solutions to delete
	if exist "%%D\CMakeLists.txt" (
	
	rem Moves into directory
	pushd "%%D"
	
	echo --- --- ---
	echo Cleaning project folder: %%D
	echo --- --- ---
	echo.

        rem rmdir = removeDirectory, /s delete all subdirectories (it would fail if folder isn't empty) 
	rem /q =quiet doesn't ask the user for confirmation
        if exist build rmdir /s /q build
	if exist CMakeFiles rmdir /s /q CMakeFiles
	if exist .vs rmdir /s /q .vs
	if exist .idea rmdir /s /q .idea
	rem Remove directories ending in dir
	for /d %%X in (*.dir) do rmdir /s /q "%%X"
        

        rem Equivalent to if exist CMakeCache.txt del /f /q CMakeCache.txt, but for files
	rem /f force, 2>nul supresses error messages (if 2 (error), redirect to nul output)
        rem del /f /q CMakeCache.txt 2>nul (But ended up putting it in the following for loop instead:)
        for %%F in (CMakeCache.txt cmake_install.cmake *.sln *.vcxproj *.vcxproj.filters *.vcxproj.user) do (
		del /f /q "%%F" 2>nul
	)

        
	rem Return to original folder
        popd
    )
)

echo Done.

rem Pausing script only if it was launched manually (instead from cleanSolutions)
rem Quotes to avoid errors if paths contain spaces, %0 the name of the script, %~0 is %0 without quotes
rem %f0 the expanded full absolut path. If run directly %~0 is clean.bat and %~f0 C:\dev...\clean.bat
if "%~0"=="%~f0" pause