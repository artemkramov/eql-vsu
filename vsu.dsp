# Microsoft Developer Studio Project File - Name="vsu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=vsu - Win32 Debug3m
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vsu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vsu.mak" CFG="vsu - Win32 Debug3m"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vsu - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Release MinSize" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Release MinDependency" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Unicode Release MinSize" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Debug3m" (based on "Win32 (x86) Application")
!MESSAGE "vsu - Win32 Release3m" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/vsu", KABAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vsu - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "e:\vctmp\Debug\vsu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "E:\boost_1_33_1" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /D ALTERFUNC=1 /FR /Yu"stdafx.h" /FD /GZ /Zm500 /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(lib1998)\d" /libpath:"E:\boost_1_33_1\libs\regex\build\vc6" /force:multiple
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\vsu.exe
InputPath=.\Debug\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
# Begin Special Build Tool
IntDir=e:\vctmp\Debug\vsu
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "E:\boost_1_33_1" /D "_DEBUG" /D "_UNICODE" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\vsu.exe
InputPath=.\DebugU\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode EXE on Windows 95 
	:end 
	
# End Custom Build
# Begin Special Build Tool
IntDir=.\DebugU
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "E:\boost_1_33_1" /D "NDEBUG" /D "_MBCS" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vsu.exe
InputPath=.\ReleaseMinSize\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
# Begin Special Build Tool
IntDir=.\ReleaseMinSize
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "E:\boost_1_33_1" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\vsu.exe
InputPath=.\ReleaseMinDependency\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
# Begin Special Build Tool
IntDir=.\ReleaseMinDependency
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "E:\boost_1_33_1" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\vsu.exe
InputPath=.\ReleaseUMinSize\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode EXE on Windows 95 
	:end 
	
# End Custom Build
# Begin Special Build Tool
IntDir=.\ReleaseUMinSize
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "E:\boost_1_33_1" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\vsu.exe
InputPath=.\ReleaseUMinDependency\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode EXE on Windows 95 
	:end 
	
# End Custom Build
# Begin Special Build Tool
IntDir=.\ReleaseUMinDependency
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsu___Win32_Release"
# PROP BASE Intermediate_Dir "vsu___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "e:\vctmp\Release\vsu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "E:\boost_1_33_1" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /D ALTERFUNC=1 /FR /Yu"stdafx.h" /FD /Zm500 /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(lib1998)\r" /libpath:"E:\boost_1_33_1\libs\regex\build\vc6"
# SUBTRACT LINK32 /debug
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\Release\vsu.exe
InputPath=.\Release\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
# Begin Special Build Tool
IntDir=e:\vctmp\Release\vsu
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Debug3m"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vsu___Win32_Debug3m"
# PROP BASE Intermediate_Dir "vsu___Win32_Debug3m"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug3m"
# PROP Intermediate_Dir "e:\vctmp\Debug3m\vsu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "HARD_LOCK" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "E:\boost_1_33_1" /D "_DEBUG" /D "_MBCS" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ccerrobj.obj myreg.lib kernel32.lib user32.lib gdi32.lib shell32.lib eqllib.lib cclib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(lib1998)\d" /force:multiple
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(lib1998)\d" /force:multiple
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug3m
TargetPath=.\Debug3m\vsu.exe
InputPath=.\Debug3m\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
# Begin Special Build Tool
IntDir=e:\vctmp\Debug3m\vsu
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "vsu - Win32 Release3m"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsu___Win32_Release3m"
# PROP BASE Intermediate_Dir "vsu___Win32_Release3m"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release3m"
# PROP Intermediate_Dir "e:\vctmp\Release3m\vsu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "E:\boost_1_33_1" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "HARD_LOCK" /D "WIN32" /D "_WINDOWS" /D "ALL_FLAGS" /D "STRICT_INTEGRITY" /D "__LOGS__" /D "FULLPROD" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ccerrobj.obj myreg.lib kernel32.lib user32.lib gdi32.lib shell32.lib eqllib.lib cclib.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(lib1998)\r"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 ccerrobj.obj myreg.lib shell32.lib msi.lib eqllib.lib cclib.lib keo.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(lib1998)\r"
# SUBTRACT LINK32 /debug
# Begin Custom Build - Performing registration
OutDir=.\Release3m
TargetPath=.\Release3m\vsu.exe
InputPath=.\Release3m\vsu.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
# Begin Special Build Tool
IntDir=e:\vctmp\Release3m\vsu
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build version
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\vsu.rc $(IntDir)\vsu.res
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "vsu - Win32 Debug"
# Name "vsu - Win32 Unicode Debug"
# Name "vsu - Win32 Release MinSize"
# Name "vsu - Win32 Release MinDependency"
# Name "vsu - Win32 Unicode Release MinSize"
# Name "vsu - Win32 Unicode Release MinDependency"
# Name "vsu - Win32 Release"
# Name "vsu - Win32 Debug3m"
# Name "vsu - Win32 Release3m"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\HcUniverse.cpp
# End Source File
# Begin Source File

SOURCE=.\kg.cpp
# End Source File
# Begin Source File

SOURCE=.\mbto.cpp
# End Source File
# Begin Source File

SOURCE=.\md5c.c

!IF  "$(CFG)" == "vsu - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "vsu - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "vsu - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "vsu - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "vsu - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "vsu - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "vsu - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "vsu - Win32 Debug3m"

!ELSEIF  "$(CFG)" == "vsu - Win32 Release3m"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Vsu.cpp
# End Source File
# Begin Source File

SOURCE=.\vsu.idl
# ADD MTL /tlb ".\vsu.tlb" /h "vsu.h" /iid "vsu_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\vsu.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\HcUniverse.h
# End Source File
# Begin Source File

SOURCE=.\kg.h
# End Source File
# Begin Source File

SOURCE=.\mbto.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\HcUniverse.rgs
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=".\magnolia-g.ico"
# End Source File
# Begin Source File

SOURCE=".\magnolia-r.ico"
# End Source File
# Begin Source File

SOURCE=".\magnolia-y.ico"
# End Source File
# Begin Source File

SOURCE=.\vsu.rgs
# End Source File
# End Group
# End Target
# End Project
