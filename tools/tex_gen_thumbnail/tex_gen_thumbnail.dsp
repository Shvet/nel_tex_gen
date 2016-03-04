# Microsoft Developer Studio Project File - Name="tex_gen_thumbnail" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tex_gen_thumbnail - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tex_gen_thumbnail.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tex_gen_thumbnail.mak" CFG="tex_gen_thumbnail - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tex_gen_thumbnail - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tex_gen_thumbnail - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tex_gen_thumbnail - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /D "NDEBUG" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 freetype214MT.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing Registration on $(InputPath)
OutDir=.\Release
TargetPath=.\Release\tex_gen_thumbnail.dll
InputPath=.\Release\tex_gen_thumbnail.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "tex_gen_thumbnail - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "__STL_DEBUG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /D "_DEBUG" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 freetype214MT.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing Registration on $(InputPath)
OutDir=.\Debug
TargetPath=.\Debug\tex_gen_thumbnail.dll
InputPath=.\Debug\tex_gen_thumbnail.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "tex_gen_thumbnail - Win32 Release"
# Name "tex_gen_thumbnail - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ExtractImageApp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\tex_gen_thumbnail.def
# End Source File
# Begin Source File

SOURCE=.\tex_gen_thumbnail.rc
# End Source File
# Begin Source File

SOURCE=.\tex_gen_thumbnailApp.cpp
# End Source File
# Begin Source File

SOURCE=.\TexGenThumbnailDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TexGenThumbnailExtractor.cpp
# End Source File
# Begin Source File

SOURCE=.\ThumbTexGenThumbnail.idl
# ADD MTL /tlb "./ThumbTexGenThumbnail.tlb" /h "ThumbTexGenThumbnail_i.h" /iid "ThumbTexGenThumbnail_i.c" /Oicf
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ExtractImageApp.h
# End Source File
# Begin Source File

SOURCE=.\IExtractImage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tex_gen_thumbnailApp.h
# End Source File
# Begin Source File

SOURCE=.\TexGenThumbnailDoc.h
# End Source File
# Begin Source File

SOURCE=.\TexGenThumbnailExtractor.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\tex_gen_thumbnail.rc2
# End Source File
# Begin Source File

SOURCE=.\tex_gen_thumbnail.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
