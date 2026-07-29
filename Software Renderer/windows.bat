

@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /c obj_parse/model_parser.cpp obj_parse/mesh_exctractor.cpp  main.cpp /I "%CD%\SDL2-2.32.6\include" 
link model_parser.obj mesh_exctractor.obj main.obj /SUBSYSTEM:CONSOLE /LIBPATH:"%CD%\SDL2-2.32.6\lib\x64" SDL2main.lib SDL2.lib shell32.lib /OUT:sr.exe
@REM srr.exe
