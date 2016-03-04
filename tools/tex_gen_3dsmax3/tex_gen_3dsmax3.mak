# Microsoft Developer Studio Generated NMAKE File, Based on tex_gen_3dsmax3.dsp
!IF "$(CFG)" == ""
CFG=tex_gen_3dsmax3 - Win32 Hybrid
!MESSAGE No configuration specified. Defaulting to tex_gen_3dsmax3 - Win32 Hybrid.
!ENDIF 

!IF "$(CFG)" != "tex_gen_3dsmax3 - Win32 Release" && "$(CFG)" != "tex_gen_3dsmax3 - Win32 Debug" && "$(CFG)" != "tex_gen_3dsmax3 - Win32 Hybrid"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tex_gen_3dsmax3.mak" CFG="tex_gen_3dsmax3 - Win32 Hybrid"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tex_gen_3dsmax3 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "tex_gen_3dsmax3 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "tex_gen_3dsmax3 - Win32 Hybrid" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi"

!ELSE 

ALL : "tex_gen - Win32 Release" "misc - Win32 Release" "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"misc - Win32 ReleaseCLEAN" "tex_gen - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DllEntry.obj"
	-@erase "$(INTDIR)\tex_gen_3dsmax3.obj"
	-@erase "$(INTDIR)\tex_gen_3dsmax3.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.exp"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.lib"
	-@erase "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MD /W3 /GR /GX /O2 /I "C:\3dsmax3_1\Maxsdk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\tex_gen_3dsmax3.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\tex_gen_3dsmax3.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tex_gen_3dsmax3.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib bmm.lib core.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\tex_gen_3dsmax3.pdb" /machine:I386 /def:".\tex_gen_3dsmax3.def" /out:"C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi" /implib:"$(OUTDIR)\tex_gen_3dsmax3.lib" /libpath:"C:\3dsmax3_1\Maxsdk\lib" /release 
DEF_FILE= \
	".\tex_gen_3dsmax3.def"
LINK32_OBJS= \
	"$(INTDIR)\DllEntry.obj" \
	"$(INTDIR)\tex_gen_3dsmax3.obj" \
	"$(INTDIR)\tex_gen_3dsmax3.res" \
	"..\..\..\lib\nlmisc_r.lib" \
	"..\..\..\lib\nltex_gen_r.lib"

"C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "C:\3dsmax3_1 debug\exe\Plugins\tex_gen_3dsmax3.bmi"

!ELSE 

ALL : "tex_gen - Win32 Debug" "misc - Win32 Debug" "C:\3dsmax3_1 debug\exe\Plugins\tex_gen_3dsmax3.bmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"misc - Win32 DebugCLEAN" "tex_gen - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DllEntry.obj"
	-@erase "$(INTDIR)\tex_gen_3dsmax3.obj"
	-@erase "$(INTDIR)\tex_gen_3dsmax3.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.exp"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.lib"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.pdb"
	-@erase "C:\3dsmax3_1 debug\exe\Plugins\tex_gen_3dsmax3.bmi"
	-@erase "C:\3dsmax3_1 debug\exe\Plugins\tex_gen_3dsmax3.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\3dsmax3_1\Maxsdk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__STL_DEBUG" /Fp"$(INTDIR)\tex_gen_3dsmax3.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\tex_gen_3dsmax3.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tex_gen_3dsmax3.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib bmm.lib core.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\tex_gen_3dsmax3.pdb" /debug /machine:I386 /def:".\tex_gen_3dsmax3.def" /out:"C:\3dsmax3_1 debug\exe\Plugins\tex_gen_3dsmax3.bmi" /implib:"$(OUTDIR)\tex_gen_3dsmax3.lib" /pdbtype:sept /libpath:"C:\3dsmax3_1\Maxsdk\lib" 
DEF_FILE= \
	".\tex_gen_3dsmax3.def"
LINK32_OBJS= \
	"$(INTDIR)\DllEntry.obj" \
	"$(INTDIR)\tex_gen_3dsmax3.obj" \
	"$(INTDIR)\tex_gen_3dsmax3.res" \
	"..\..\..\lib\nlmisc_d.lib" \
	"..\..\..\lib\nltex_gen_d.lib"

"C:\3dsmax3_1 debug\exe\Plugins\tex_gen_3dsmax3.bmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Hybrid"

OUTDIR=.\tex_gen_3dsmax3___Win32_Hybrid
INTDIR=.\tex_gen_3dsmax3___Win32_Hybrid

!IF "$(RECURSE)" == "0" 

ALL : "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi"

!ELSE 

ALL : "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DllEntry.obj"
	-@erase "$(INTDIR)\tex_gen_3dsmax3.obj"
	-@erase "$(INTDIR)\tex_gen_3dsmax3.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.exp"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.lib"
	-@erase "$(OUTDIR)\tex_gen_3dsmax3.pdb"
	-@erase "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi"
	-@erase "C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MD /W3 /Gm /GR /GX /ZI /Od /I "C:\3dsmax3_1\Maxsdk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\tex_gen_3dsmax3.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\tex_gen_3dsmax3.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tex_gen_3dsmax3.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib bmm.lib core.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\tex_gen_3dsmax3.pdb" /debug /machine:I386 /def:".\tex_gen_3dsmax3.def" /out:"C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi" /implib:"$(OUTDIR)\tex_gen_3dsmax3.lib" /pdbtype:sept /libpath:"C:\3dsmax3_1\Maxsdk\lib" 
DEF_FILE= \
	".\tex_gen_3dsmax3.def"
LINK32_OBJS= \
	"$(INTDIR)\DllEntry.obj" \
	"$(INTDIR)\tex_gen_3dsmax3.obj" \
	"$(INTDIR)\tex_gen_3dsmax3.res"

"C:\program files\3dsmax3_1\Plugins\tex_gen_3dsmax3.bmi" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tex_gen_3dsmax3.dep")
!INCLUDE "tex_gen_3dsmax3.dep"
!ELSE 
!MESSAGE Warning: cannot find "tex_gen_3dsmax3.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tex_gen_3dsmax3 - Win32 Release" || "$(CFG)" == "tex_gen_3dsmax3 - Win32 Debug" || "$(CFG)" == "tex_gen_3dsmax3 - Win32 Hybrid"
SOURCE=.\DllEntry.cpp

"$(INTDIR)\DllEntry.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tex_gen_3dsmax3.cpp

"$(INTDIR)\tex_gen_3dsmax3.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tex_gen_3dsmax3.rc

"$(INTDIR)\tex_gen_3dsmax3.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Release"

"misc - Win32 Release" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Release" 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

"misc - Win32 ReleaseCLEAN" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

!ELSEIF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Debug"

"misc - Win32 Debug" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Debug" 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

"misc - Win32 DebugCLEAN" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\misc.mak CFG="misc - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

!ELSEIF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Hybrid"

!ENDIF 

!IF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Release"

"tex_gen - Win32 Release" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\tex_gen.mak CFG="tex_gen - Win32 Release" 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

"tex_gen - Win32 ReleaseCLEAN" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\tex_gen.mak CFG="tex_gen - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

!ELSEIF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Debug"

"tex_gen - Win32 Debug" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\tex_gen.mak CFG="tex_gen - Win32 Debug" 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

"tex_gen - Win32 DebugCLEAN" : 
   cd "\code\nel\src"
   $(MAKE) /$(MAKEFLAGS) /F .\tex_gen.mak CFG="tex_gen - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tools\tex_gen\tex_gen_3dsmax3"

!ELSEIF  "$(CFG)" == "tex_gen_3dsmax3 - Win32 Hybrid"

!ENDIF 


!ENDIF 

