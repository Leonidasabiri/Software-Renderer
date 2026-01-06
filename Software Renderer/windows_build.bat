
cl /c main.cpp /I "%CD%\SDL2-2.32.6\include"
link main.obj /SUBSYSTEM:WINDOWS /LIBPATH:"%CD%\SDL2-2.32.6\lib\x64" SDL2main.lib SDL2.lib Shell32.lib /OUT:sr.exe
