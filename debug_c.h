/*
 *	Licencia Apache, Versión 2.0 con Modificación
 *	
 *	Copyright 2023 Desmon (David)
 *	
 *	Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *	este software y archivos de documentación asociados (el "Software"), para tratar el 
 *	Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *	modificación, fusión, publicación, distribución, sublicencia y/o venta de copias del 
 *	Software, y para permitir a las personas a quienes se les proporcione el Software 
 *	hacer lo mismo, sujeto a las siguientes condiciones:
 *	
 *	El anterior aviso de copyright y este aviso de permiso se incluirán en todas las 
 *	copias o partes sustanciales del Software.
 *	
 *	EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTÍA DE NINGÚN TIPO, EXPRESA O 
 *	IMPLÍCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTÍAS DE COMERCIABILIDAD, IDONEIDAD 
 *	PARA UN PROPÓSITO PARTICULAR Y NO INFRACCIÓN. EN NINGÚN CASO LOS TITULARES DEL 
 *	COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERÁN RESPONSABLES DE NINGÚN 
 *	RECLAMO, DAÑO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIÓN DE CONTRATO, AGRAVIO O DE 
 *	OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIÓN CON EL SOFTWARE O EL USO U OTRO TIPO
 *	DE ACCIONES EN EL SOFTWARE.
 *	
 *	Además, cualquier modificación realizada por terceros se considerará propiedad del 
 *	titular original de los derechos de autor. Los titulares de derechos de autor 
 *	originales no se responsabilizan de las modificaciones realizadas por terceros.
 *	
 *	Queda explícitamente establecido que no es obligatorio especificar ni notificar los 
 *	cambios realizados entre versiones, ni revelar porciones específicas de código 
 *	modificado.
 */

 #ifndef __DEBUG_H__
 #define __DEBUG_H__

 #ifndef _WIN32
 #define _GNU_SOURCE
 #define _POSIX_C_SOURCE 200809L
#endif

 #include <stdio.h>
 #include <stdarg.h>
 #include "colors.h"
 #include "error_c.h"
#ifdef _MSC_VER
#   pragma comment(lib, "libcolors.a")
#   pragma comment(lib, "libdebug.a")
#endif

 //#include "WL_lib.h"
 //#define NAME_DEFAULT_LOG_DEBUG "debug_log.txt"
 //MyFile Log_debug_file;

 #ifdef DEBUG_ENABLE
 #define DEBUG_PRINT(level, fmt, ...) debug_print(level, "#{FG:lgreen}[#{FG:lpurple}%s#{reset}:#{FG:cyan}%s#{reset}:#{FG:red}%03d#{FG:green}]#{reset} " fmt , get_level_debug(level), __FILE__, __LINE__, ##__VA_ARGS__)

 #else
 #define DEBUG_PRINT(level, fmt, ...) if (level == DEBUG_LEVEL_ERROR) debug_print(level, "#{FG:lgreen}[#{FG:lpurple}%s#{reset}:#{FG:cyan}%s#{reset}:#{FG:red}%03d#{FG:green}]#{reset} " fmt , get_level_debug(level), __FILE__, __LINE__, ##__VA_ARGS__)
 #endif


 #define INIT_TYPE_FUNC_DBG(type_return, name_func) "#{FG:lred} "#type_return" #{FG:cyan}"#name_func"#{FG:white}("
 #define TYPE_DATA_DBG(type_data, name_arg) " #{FG:lyellow}"#type_data" #{FG:white} "name_arg", "
 #define END_TYPE_FUNC_DBG "#{FG:white}); #{reset}\n"

typedef enum
{
     DEBUG_LEVEL_INFO,
     DEBUG_LEVEL_WARNING,
     DEBUG_LEVEL_ERROR,

     DEBUG_LEVEL_UNKNOWN
} DebugLevel;
#ifndef IS_USED
#   ifndef _MSC_VER
#       define IS_USED __attribute__((used))
#   else
#       define IS_USED
#   endif
#endif
static IS_USED DebugLevel currentLevel = DEBUG_LEVEL_INFO; // nivel de debbug del programa por default

static IS_USED const char* ErrorLevelStrings[] = {
     "INFO",
     "WARNING",
     "ERROR",

     [DEBUG_LEVEL_UNKNOWN] = "UNKNOW"
 };

 typedef struct {
     char *message;
     const char *file;
     unsigned int line;
     DebugLevel level_c;
 } Debug;

 #define DEBUG_C(level, msg)                                                                 \
     do                                                                                    \
     {                                                                                     \
         Debug Deb = {.message = msg, .file = __FILE__, .line = __LINE__, .level_c = level}; \
         debug_print(Deb.level_c, Deb.message);     \
  } while (0)

 #ifdef DEBUG_ENABLE
 #define debug_malloc(type, name_var, size) \
         name_var = (type*)malloc(size); \
         DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:red}[#{FG:yellow}MALLOC#{FG:red}]#{FG:lred}  #{FG:white}(#{FG:lred}%s#{FG:white}*)#{FG:cyan}malloc#{FG:white}(%d) #{FG:lgreen}Variable#{FG:white}: %s, #{FG:lgreen}Tipo#{FG:white}: %s, #{FG:lgreen}Puntero#{FG:white}: %p\n", #type, size, #name_var, #type, name_var); \
         if (NULL == name_var) {DEBUG_PRINT(DEBUG_LEVEL_ERROR,"malloc error\n");}
#else
 #define debug_malloc(type, name_var, size) name_var = (type*)malloc(size)
 #endif
 #ifdef DEBUG_ENABLE
 #define debug_calloc(type, name_var, cantidad, size) \
         name_var = (type*)calloc(cantidad, size); \
         DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:red}[#{FG:yellow}CALLOC#{FG:red}] #{FG:white}(#{FG:lred}%s#{FG:white}*)#{FG:cyan}calloc#{FG:white}(%d, %d) #{FG:lgreen}Variable#{FG:white}: %s, #{FG:lgreen}Tipo#{FG:white}: %s, #{FG:lgreen}Puntero#{FG:white}: %p\n", #type, cantidad, size, #name_var, #type, name_var); \
         if (NULL == name_var) DEBUG_PRINT(DEBUG_LEVEL_ERROR,"calloc error\n");
 #else
 #define debug_calloc(type, name_var, cantidad, size) name_var = (type*)calloc(cantidad, size); if (NULL == name_var) DEBUG_PRINT(DEBUG_LEVEL_ERROR,"calloc error\n");
 #endif

 #ifdef DEBUG_ENABLE
 #define debug_realloc(type, name_var, size) \
         name_var = (type*)realloc(name_var, size); \
         DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:red}[#{FG:yellow}REALLOC#{FG:red}] #{FG:white}(#{FG:lred}%s#{FG:white}*)#{FG:cyan}realloc#{FG:white}(%p, %d) #{FG:lgreen}Variable#{FG:white}: %s, #{FG:lgreen}Tipo#{FG:white}: %s, #{FG:lgreen}Puntero#{FG:white}: %p\n", #type, name_var, size, #name_var, #type, name_var); \
         if (NULL == name_var) DEBUG_PRINT(DEBUG_LEVEL_ERROR,"realloc error\n");
 #else
 #define debug_realloc(type, name_var, size) name_var = (type*)realloc(name_var, size); if (NULL == name_var) DEBUG_PRINT(DEBUG_LEVEL_ERROR,"realloc error\n");
 #endif

 const char* get_level_debug(DebugLevel level);
 //void debug_set_log_file(const char *filename);
static inline void debug_set_level(const DebugLevel level)
{
    currentLevel = level;
}
 void debug_print(DebugLevel level, const char *fmt, ...);
 #ifdef _MSC_VER
 void __constructor_debug_c__();
 void __destructor_debug_c__();
 #else
 void __attribute__((constructor)) __constructor_debug_c__();
 void __attribute__((destructor)) __destructor_debug_c__();
 #endif

 #ifdef _WIN32
 #ifndef _ExceptionHandler_WIN_
 #define _ExceptionHandler_WIN_
 LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS *ExceptionInfo);
 #endif
 #else
 #define _GNU_SOURCE
 #define _POSIX_C_SOURCE 200809L
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 #include <signal.h>
 #include <ucontext.h>
 #include <execinfo.h>
 #include <unistd.h>
 #include <inttypes.h>
 void signalHandler(int sig, siginfo_t *info, void *context);
 #endif

 //static FILE *logFile = NULL;

 #ifdef INCLUDE_COLORS_C
 #include "debug_c.c"
 #endif
 #endif