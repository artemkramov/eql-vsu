# Microsoft Developer Studio Project File - Name="VsuErr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VsuErr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VsuErr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VsuErr.mak" CFG="VsuErr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VsuErr - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VsuErr - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/vsu/VsuErr", VPCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VsuErr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "e:\vctmp\Release\vsuerr"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSUERR_EXPORTS" /YX /FD /c
# ADD CPP /nologo /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSUERR_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "NDEBUG"
# ADD RSC /l 0x422 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib /nologo /base:"0x36f00000" /version:1.0 /entry:"DllMainVsuErr" /dll /machine:I386 /nodefaultlib /Release /merge:.rdata=.text
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
IntDir=e:\vctmp\Release\vsuerr
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build counter
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\VsuErr.rc $(IntDir)\VsuErr.res
# End Special Build Tool

!ELSEIF  "$(CFG)" == "VsuErr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "e:\vctmp\Debug\vsuerr"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSUERR_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSUERR_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib /nologo /base:"0x36f00000" /version:1.0 /entry:"DllMainVsuErr" /dll /debug /machine:I386 /nodefaultlib /pdbtype:sept /merge:.rdata=.text
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
IntDir=e:\vctmp\Debug\vsuerr
ProjDir=.
SOURCE="$(InputPath)"
PreLink_Desc=Updating build counter
PreLink_Cmds=$(PerlBin)\perl.exe $(UpdVer)\updver.pl $(ProjDir)\VsuErr.rc $(IntDir)\VsuErr.res
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "VsuErr - Win32 Release"
# Name "VsuErr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\VsuErr.cpp
# End Source File
# Begin Source File

SOURCE=.\VsuErrM.mc

!IF  "$(CFG)" == "VsuErr - Win32 Release"

# Begin Custom Build - Executing Message Compiler (mc.exe)
TargetDir=.\Release
InputPath=.\VsuErrM.mc
InputName=VsuErrM

BuildCmds= \
	mc $(InputName) -x $(TargetDir)

"VsuErrM.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ru.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ua.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"us.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "VsuErr - Win32 Debug"

# Begin Custom Build - Executing Message Compiler (mc.exe)
TargetDir=.\Debug
InputPath=.\VsuErrM.mc
InputName=VsuErrM

BuildCmds= \
	mc $(InputName) -x $(TargetDir)

"VsuErrM.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ru.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ua.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"us.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\VsuErr.rc
# End Source File
# End Group
# End Target
# End Project
