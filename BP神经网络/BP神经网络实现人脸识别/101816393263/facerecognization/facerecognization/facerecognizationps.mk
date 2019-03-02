
facerecognizationps.dll: dlldata.obj facerecognization_p.obj facerecognization_i.obj
	link /dll /out:facerecognizationps.dll /def:facerecognizationps.def /entry:DllMain dlldata.obj facerecognization_p.obj facerecognization_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del facerecognizationps.dll
	@del facerecognizationps.lib
	@del facerecognizationps.exp
	@del dlldata.obj
	@del facerecognization_p.obj
	@del facerecognization_i.obj
