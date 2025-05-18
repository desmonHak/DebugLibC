CC 			  = gcc
ARR			  = ar

VESRION_C     =11

PATH_SRC 	      = src
PATH_INCLUDE      = include
PATH_EXAMPLES	  = example
PATH_COLORS		  = colors-C-C-plus-plus

LINKER_FLAGS  	  =  -L. -ldebug -L$(PATH_COLORS) -lcolors

INCLUDE_FLAGS = -I. -I$(PATH_COLORS)
GLOBAL_CFLAGS = -std=c$(VESRION_C) $(INCLUDE_FLAGS) -masm=intel \
				-D_ExceptionHandler -fdiagnostics-color=always -D_GNU_SOURCE $(DEBUG_LINUX)

ifeq ($(OS_NAME),windows)
else
	GLOBAL_CFLAGS += -fPIC -D_GNU_SOURCE  -D_POSIX_C_SOURCE=200809L
endif

CFLAGS 		  =  $(GLOBAL_CFLAGS) -O3 -Wno-unused-parameter \
				-Wno-implicit-fallthrough -Wno-type-limits  \
				-Wno-unused-variable -Wno-pointer-sign
ARR_FLAGS     = -rc

OBJECTS 	= debug_c.o error_c.o time_code.o