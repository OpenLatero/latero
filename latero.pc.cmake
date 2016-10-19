prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include
	
Name: liblatero
Description: user library for Latero tactile displays
Version: 0.0.1

Libs: -L${libdir} -llatero @DEPS_LDFLAGS_WSPACE@
Cflags: -I${includedir} @DEPS_CFLAGS_WSPACE@