@echo off



@REM fetch a file, but don't clobber existing
SET GETTER=..\tools\wget.exe -nc

@REM unzip an archive, always overwrite files
SET UNZIPPER=..\tools\7za.exe x -aoa





@REM Download all the things
%GETTER% "http://pocoproject.org/releases/poco-1.6.1/poco-1.6.1.zip"
if %ERRORLEVEL% neq 0 goto bail

%GETTER% "http://www.sfml-dev.org/files/SFML-2.3.2-windows-vc14-64-bit.zip"
if %ERRORLEVEL% neq 0 goto bail

%GETTER% "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/windows/tbb44_20160128oss_win_0.zip"
if %ERRORLEVEL% neq 0 goto bail

%GETTER% "http://bitbucket.org/eigen/eigen/get/3.2.8.zip"
if %ERRORLEVEL% neq 0 goto bail





@REM extract all the things
if not exist poco-1.6.1 (%UNZIPPER% poco-1.6.1.zip 1>&0 >> log.txt)
if not exist SFML-2.3.2 (%UNZIPPER% SFML-2.3.2-windows-vc14-64-bit.zip 1>&0 >> log.txt)
if not exist tbb44_20160128oss (%UNZIPPER% tbb44_20160128oss_win_0.zip 1>&0 >> log.txt)
if not exist eigen-eigen-07105f7124f9 (%UNZIPPER% 3.2.8.zip 1>&0 >> log.txt)



exit /b 0



:bail
@echo.
@echo Something failed...
@echo.
exit /b 1