# Microsoft Developer Studio Project File - Name="nel_tex_gen" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=nel_tex_gen - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nel_tex_gen.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nel_tex_gen.mak" CFG="nel_tex_gen - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nel_tex_gen - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "nel_tex_gen - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "nel_tex_gen - Win32 ReleaseDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nel_tex_gen - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 freetype214MT.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "nel_tex_gen - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "__STL_DEBUG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 freetype214MT.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "nel_tex_gen - Win32 ReleaseDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "nel_tex_gen___Win32_ReleaseDebug"
# PROP BASE Intermediate_Dir "nel_tex_gen___Win32_ReleaseDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDebug"
# PROP Intermediate_Dir "ReleaseDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 freetype214MT.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 freetype214MT.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "nel_tex_gen - Win32 Release"
# Name "nel_tex_gen - Win32 Debug"
# Name "nel_tex_gen - Win32 ReleaseDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\action.cpp
# End Source File
# Begin Source File

SOURCE=.\bitmap_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\child_frm.cpp
# End Source File
# Begin Source File

SOURCE=.\color_box.cpp
# End Source File
# Begin Source File

SOURCE=.\color_picker.cpp
# End Source File
# Begin Source File

SOURCE=.\color_picker_color_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\color_picker_hue_panel.cpp
# End Source File
# Begin Source File

SOURCE=.\color_picker_rgb_panel.cpp
# End Source File
# Begin Source File

SOURCE=.\dib_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\gradient_cursor_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\gradient_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\gradient_rect_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\gradient_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\main_frm.cpp
# End Source File
# Begin Source File

SOURCE=.\my_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen.cpp
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen.rc
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen_doc.cpp
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen_view.cpp
# End Source File
# Begin Source File

SOURCE=.\op_property_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\operator_win.cpp
# End Source File
# Begin Source File

SOURCE=.\port.cpp
# End Source File
# Begin Source File

SOURCE=.\rot_button.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\action.h
# End Source File
# Begin Source File

SOURCE=.\bitmap_wnd.h
# End Source File
# Begin Source File

SOURCE=.\child_frm.h
# End Source File
# Begin Source File

SOURCE=.\color_box.h
# End Source File
# Begin Source File

SOURCE=.\color_picker.h
# End Source File
# Begin Source File

SOURCE=.\color_picker_color_wnd.h
# End Source File
# Begin Source File

SOURCE=.\color_picker_hue_panel.h
# End Source File
# Begin Source File

SOURCE=.\color_picker_rgb_panel.h
# End Source File
# Begin Source File

SOURCE=.\dib_wnd.h
# End Source File
# Begin Source File

SOURCE=.\gradient_cursor_wnd.h
# End Source File
# Begin Source File

SOURCE=.\gradient_dlg.h
# End Source File
# Begin Source File

SOURCE=.\gradient_rect_wnd.h
# End Source File
# Begin Source File

SOURCE=.\gradient_wnd.h
# End Source File
# Begin Source File

SOURCE=.\main_frm.h
# End Source File
# Begin Source File

SOURCE=.\my_edit.h
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen.h
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen_doc.h
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen_view.h
# End Source File
# Begin Source File

SOURCE=.\op_property_dlg.h
# End Source File
# Begin Source File

SOURCE=.\operator_win.h
# End Source File
# Begin Source File

SOURCE=.\port.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rot_button.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\app.ico
# End Source File
# Begin Source File

SOURCE=.\res\arrow_in.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_hole.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_hole_mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\nel_tex_gen.rc2
# End Source File
# Begin Source File

SOURCE=.\res\nel_tex_gen_doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\quadrati.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\history.txt
# End Source File
# Begin Source File

SOURCE=.\nel_tex_gen.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
