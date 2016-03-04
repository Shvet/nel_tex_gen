r:
cd \

del /Q r:\code\nel\tools\tex_gen\nel_tex_gen_10b_src.zip

mkdir c:\temp\nel_tex_gen_src
mkdir c:\temp\nel_tex_gen_src\nel
mkdir c:\temp\nel_tex_gen_src\nel\src
mkdir c:\temp\nel_tex_gen_src\nel\src\tex_gen
xcopy r:\code\nel\src\tex_gen\*.* c:\temp\nel_tex_gen_src\nel\src\tex_gen /S /V /Y /EXCLUDE:R:\excl.txt

mkdir c:\temp\nel_tex_gen_src\nel\tools
xcopy r:\code\nel\tools\tex_gen\*.* c:\temp\nel_tex_gen_src\nel\tools /S /V /Y /EXCLUDE:R:\excl.txt

c:
cd c:\temp\nel_tex_gen_src
zip -Tr r:\code\nel\tools\tex_gen\nel_tex_gen_10b_src.zip ./*.* 

rmdir /S /Q c:\temp\nel_tex_gen_src
rd c:\temp\nel_tex_gen_src
