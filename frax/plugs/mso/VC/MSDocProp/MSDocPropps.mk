
MSDocPropps.dll: dlldata.obj MSDocProp_p.obj MSDocProp_i.obj
	link /dll /out:MSDocPropps.dll /def:MSDocPropps.def /entry:DllMain dlldata.obj MSDocProp_p.obj MSDocProp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del MSDocPropps.dll
	@del MSDocPropps.lib
	@del MSDocPropps.exp
	@del dlldata.obj
	@del MSDocProp_p.obj
	@del MSDocProp_i.obj
