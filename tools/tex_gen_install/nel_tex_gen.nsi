; nel_tex_gen.nsi

!define VER_MAJOR 1
!define VER_MINOR 0b

;--------------------------------

; The name of the installer
Name "NeL Texture Generator v${VER_MAJOR}.${VER_MINOR}"
Caption "NeL Texture Generator v${VER_MAJOR}.${VER_MINOR}"
Icon "..\tex_gen_editor\res\app.ico"
UninstallIcon "..\tex_gen_editor\res\app.ico"

Page components
Page directory
Page custom plugin_max3_begin ": 3dsmax3 directory"
Page custom plugin_max5_begin ": 3dsmax5 directory"
Page custom installation_options ": installation options"
Page instfiles

ShowInstDetails show
XPStyle on

ComponentText "Select the components you want to install."

; The file to write
OutFile nel_tex_gen_${VER_MAJOR}${VER_MINOR}.exe

; The default installation directory
InstallDir "$PROGRAMFILES\Digital Murder\nel_tex_gen"

; The text to prompt the user to enter a directory
DirText "Choose a directory to install the NeL Texture Generator files."

;--------------------------------

InstType "Full"

;--------------------------------

Section ""
	SetOutPath $INSTDIR
	WriteUninstaller Uninst.exe

	; Set output path to the installation directory.
	File "..\tex_gen_editor\history.txt"
SectionEnd

; The stuff to install
SubSection "Texture Generator Editor"

Section "Executable"
SectionIn 1
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File ..\tex_gen_editor\Release\nel_tex_gen.exe
  File C:\Lib\STLport\lib\stlport_vc645.dll
  File C:\bin\ProfUIS221m.dll

  WriteRegStr HKCR ".texgen" "" "NeLTexGen"
  WriteRegStr HKCR "NeLTexGen" "" "NeL Texture Generator File"
  WriteRegStr HKCR "NeLTexGen\shell" "" "open"
  WriteRegStr HKCR "NeLTexGen\DefaultIcon" "" $INSTDIR\nel_tex_gen.exe,1
  WriteRegStr HKCR "NeLTexGen\shell\open\command" "" '$INSTDIR\nel_tex_gen.exe "%1"'

SectionEnd

Section "Sample Textures"
SectionIn 1
  SetOutPath $INSTDIR\samples
  File ..\tex_gen_editor\samples\*.texgen
SectionEnd

SubSectionEnd

Section "Windows Explorer Thumbnail Plug-in"
SectionIn 1
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File ..\tex_gen_thumbnail\Release\tex_gen_thumbnail.dll
  File C:\Lib\STLport\lib\stlport_vc645.dll
  RegDll $INSTDIR\tex_gen_thumbnail.dll

SectionEnd

Section "3dsmax3 Plug-in" pluginmax3
SectionIn 1
	; Set output path to the installation directory.
	ReadINIStr $1 $0 "Field 2" "State"

	SetOutPath $1\plugins

	; Put file there
	File "c:\program files\3dsmax3_1\plugins\tex_gen_3dsmax3.bmi"

	SetOutPath $1
	File C:\Lib\STLport\lib\stlport_vc645.dll

SectionEnd

Section "3dsmax5 Plug-in" pluginmax5
SectionIn 1
	; Set output path to the installation directory.
	ReadINIStr $1 $4 "Field 2" "State"

	SetOutPath $1\plugins

	; Put file there
	File "c:\program files\3dsmax5\plugins\tex_gen_3dsmax5.bmi"

	SetOutPath $1
	File C:\Lib\STLport\lib\stlport_vc645.dll

SectionEnd

Section "Uninstall"
  UnRegDLL $INSTDIR\tex_gen_thumbnail.dll
  Delete $INSTDIR\Uninst.exe ; delete self (see explanation below why this works)
  Delete $INSTDIR\tex_gen_thumbnail.dll
  Delete $INSTDIR\nel_tex_gen.exe
  Delete $INSTDIR\log.log
  Delete $INSTDIR\stlport_vc645.dll
  Delete $INSTDIR\ProfUIS221m.dll
  Delete $INSTDIR\history.txt
  Delete $INSTDIR\samples\*.texgen
  Delete "$DESKTOP\NeL TexGen Editor.lnk"
  RMDir /r "$SMPROGRAMS\NeL Texture Generator"
  RMDir $INSTDIR\samples
  RMDir $INSTDIR

  DeleteRegKey HKCR ".texgen"
  DeleteRegKey HKCR "NeLTexGen"
  DeleteRegKey HKCR "Neltexgen.Document"
SectionEnd

Function installation_options
	GetTempFileName $5
	File /oname=$5 options.ini
	InstallOptions::dialog $5
FunctionEnd

Function plugin_max3_begin
	SectionGetFlags ${pluginmax3} $R1
	IntOp $R2 $R1 & 1
	StrCmp $R2 1 continue
		Abort
continue:
	GetTempFileName $0
	File /oname=$0 plugin_max3.ini
	InstallOptions::dialog $0
FunctionEnd

Function plugin_max5_begin
	SectionGetFlags ${pluginmax5} $R1
	IntOp $R2 $R1 & 1
	StrCmp $R2 1 continue2
		Abort
continue2:
	GetTempFileName $4
	File /oname=$4 plugin_max5.ini
	InstallOptions::dialog $4
FunctionEnd

Function .onInstSuccess

	; Menu shortcuts
	ReadINIStr $1 $5 "Field 2" "State"
	StrCmp $1 0 abort_menu
		IfFileExists "$INSTDIR\nel_tex_gen.exe" "" +3
			CreateDirectory "$SMPROGRAMS\NeL Texture Generator"
			CreateShortCut "$SMPROGRAMS\NeL Texture Generator\NeL TexGen Editor.lnk" "$INSTDIR\nel_tex_gen.exe"
		IfFileExists "$INSTDIR\Uninst.exe" "" +3
			CreateDirectory "$SMPROGRAMS\NeL Texture Generator"
			CreateShortCut "$SMPROGRAMS\NeL Texture Generator\Uninstall NeL Texture Generator.lnk" "$INSTDIR\Uninst.exe"
abort_menu:

	; Desktop shortcut
	ReadINIStr $1 $5 "Field 1" "State"
	StrCmp $1 0 abort_desktop
		IfFileExists "$INSTDIR\nel_tex_gen.exe" "" +2
			CreateShortCut "$DESKTOP\NeL TexGen Editor.lnk" "$INSTDIR\nel_tex_gen.exe"
abort_desktop:

	ReadINIStr $1 $5 "Field 4" "State"
	StrCmp $1 0 abort_view_history
		IfFileExists "$INSTDIR\nel_tex_gen.exe" "" +2
			Exec "$INSTDIR\nel_tex_gen.exe"

abort_view_history:
	ReadINIStr $1 $5 "Field 3" "State"
	StrCmp $1 0 abort_run
		IfFileExists "$INSTDIR\history.txt" "" +2
			Exec "notepad.exe $INSTDIR\history.txt"
abort_run:

FunctionEnd
