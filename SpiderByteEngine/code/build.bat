@echo off
set CommonCompilerFlags=-nologo -Zi -EHsc -FC
set CFiles=..\SpiderByteEngine\code\XBufferEngine.cpp ..\SpiderByteEngine\code\glad.c ..\SpiderByteEngine\code\includes\Shader\Shader.cpp ..\SpiderByteEngine\code\includes\Camera\FPSCamera.cpp ..\SpiderByteEngine\code\includes\Model\Model.cpp ..\SpiderByteEngine\code\includes\Mesh\Mesh.cpp
set CommonLinkerFlags="user32.lib" "opengl32.lib" "gdi32.lib" "..\SpiderByteEngine\code\includes\libs\glfw3.lib" "shell32.lib" "..\SpiderByteEngine\code\includes\libs\assimp-vc142-mtd.lib"
REM TODO - can we just build both with one exe?
IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build

REM 32-bit Build
rem cl  %CommonCompilerFlags% %CFiles% -Ig:\SpiderByteEngine\code\includes\ /link %CommonLinkerFlags%

REM 64-bit Build
cl  %CommonCompilerFlags% %CFiles% -Ig:\SpiderByteEngine\code\includes\ /link %CommonLinkerFlags%
popd



