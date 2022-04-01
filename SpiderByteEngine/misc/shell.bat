@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
rem call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
set path=g:\SpiderByteEngine\misc\;%path%

"z:\bin\emacs.exe" -q -l "g:\SpiderByteEngine\misc\.emacs
