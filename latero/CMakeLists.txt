
####
#### SOURCE FILES
####

cmake_minimum_required(VERSION 3.0)

set(SRC_TL_C
	tl-latero/latero.c
	tl-latero/latero_io.c
)

set(SRC_TL_H
	tl-latero/latero.h
	tl-latero/latero_io.h
)


set(SRC_CPP
	tactiledisplay.cpp
	tactograph.cpp
)

set(SRC_H
	point.h
	tactiledisplay.h
	tactileimg.h
	tactograph.h
)

set(SRC ${SRC_H} ${SRC_CPP})
set(SRC_TL ${SRC_TL_H} ${SRC_TL_C})

####
#### BUILD
####

add_library (latero ${SRC} ${SRC_TL})

####
#### INSTALL
####

#install library files to /usr/local/lib/
INSTALL(TARGETS latero
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# install header files to /usr/local/include/latero
INSTALL(FILES ${SRC_H} DESTINATION include/latero)
INSTALL(FILES ${SRC_TL_H} DESTINATION include/latero/tl-latero)