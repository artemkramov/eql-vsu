
vsups.dll: dlldata.obj vsu_p.obj vsu_i.obj
	link /dll /out:vsups.dll /def:vsups.def /entry:DllMain dlldata.obj vsu_p.obj vsu_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del vsups.dll
	@del vsups.lib
	@del vsups.exp
	@del dlldata.obj
	@del vsu_p.obj
	@del vsu_i.obj
