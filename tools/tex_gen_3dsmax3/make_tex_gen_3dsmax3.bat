call "C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
set INCLUDE=C:\LIB\STLPORT\STLPORT;C:\Program Files\Microsoft SDK\include;r:\code\nel\src;r:\code\nel\include;C:\Program Files\3dsmax3_1\Maxsdk\Include;%INCLUDE%
set LIB=C:\LIB\STLPORT\LIB;C:\Program Files\Microsoft SDK\lib;r:\code\nel\lib;C:\Program Files\3dsmax3_1\Maxsdk\lib;%LIB%
msdev ../tex_gen_editor/nel_tex_gen.dsw /MAKE "tex_gen_3dsmax3 - Win32 Release" /USEENV
