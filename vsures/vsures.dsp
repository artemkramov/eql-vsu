# Microsoft Developer Studio Project File - Name="vsures" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vsures - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vsures.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vsures.mak" CFG="vsures - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vsures - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Release422" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Release409" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Release402" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Release408" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Release418" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vsures - Win32 Release419" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/vsu/vsures", GTDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vsures - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x38800000" /dll /debug /machine:I386 /nodefaultlib /pdbtype:sept /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Release422"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsures___Win32_Release422"
# PROP BASE Intermediate_Dir "vsures___Win32_Release422"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release422"
# PROP Intermediate_Dir "Release422"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_UKR"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x38800000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Release409"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsures___Win32_Release409"
# PROP BASE Intermediate_Dir "vsures___Win32_Release409"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release409"
# PROP Intermediate_Dir "Release409"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x38800000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x38800000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Release402"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsures___Win32_Release402"
# PROP BASE Intermediate_Dir "vsures___Win32_Release402"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release402"
# PROP Intermediate_Dir "Release402"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_UKR"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_BGR"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Release408"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsures___Win32_Release408"
# PROP BASE Intermediate_Dir "vsures___Win32_Release408"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release408"
# PROP Intermediate_Dir "Release408"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ELL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Release418"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsures___Win32_Release418"
# PROP BASE Intermediate_Dir "vsures___Win32_Release418"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release418"
# PROP Intermediate_Dir "Release418"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_UKR"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ROM"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vsures - Win32 Release419"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vsures___Win32_Release419"
# PROP BASE Intermediate_Dir "vsures___Win32_Release419"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release419"
# PROP Intermediate_Dir "Release419"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VSURES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_UKR"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_RUS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x38900000" /dll /machine:I386 /nodefaultlib /noentry
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "vsures - Win32 Release"
# Name "vsures - Win32 Debug"
# Name "vsures - Win32 Release422"
# Name "vsures - Win32 Release409"
# Name "vsures - Win32 Release402"
# Name "vsures - Win32 Release408"
# Name "vsures - Win32 Release418"
# Name "vsures - Win32 Release419"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\vsures.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
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

SOURCE=.\vsumc_402.bin
# End Source File
# Begin Source File

SOURCE=.\vsumc_408.bin
# End Source File
# Begin Source File

SOURCE=.\vsumc_409.bin
# End Source File
# Begin Source File

SOURCE=.\vsumc_418.bin
# End Source File
# Begin Source File

SOURCE=.\vsumc_419.bin
# End Source File
# Begin Source File

SOURCE=.\vsumc_422.bin
# End Source File
# Begin Source File

SOURCE=.\vsutray.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\vsumc.mc

!IF  "$(CFG)" == "vsures - Win32 Release"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Debug"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Release422"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Release409"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Release402"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Release408"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Release418"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vsures - Win32 Release419"

# Begin Custom Build - Compiling $(InputPath)
InputDir=.
InputPath=.\vsumc.mc
InputName=vsumc

BuildCmds= \
	mc.exe "$(InputPath)"

"$(InputDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
