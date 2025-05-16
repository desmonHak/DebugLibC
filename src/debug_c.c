/*
 *	Licencia Apache, Version 2.0 con Modificacion
 *	
 *	Copyright 2023 Desmon (David)
 *	
 *	Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *	este software y archivos de documentacion asociados (el "Software"), para tratar el 
 *	Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *	modificacion, fusion, publicacion, distribucion, sublicencia y/o venta de copias del 
 *	Software, y para permitir a las personas a quienes se les proporcione el Software 
 *	hacer lo mismo, sujeto a las siguientes condiciones:
 *	
 *	El anterior aviso de copyright y este aviso de permiso se incluiran en todas las 
 *	copias o partes sustanciales del Software.
 *	
 *	EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTiA DE NINGÚN TIPO, EXPRESA O 
 *	IMPLiCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTiAS DE COMERCIABILIDAD, IDONEIDAD 
 *	PARA UN PROPoSITO PARTICULAR Y NO INFRACCIoN. EN NINGÚN CASO LOS TITULARES DEL 
 *	COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERaN RESPONSABLES DE NINGÚN 
 *	RECLAMO, DAnO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIoN DE CONTRATO, AGRAVIO O DE 
 *	OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIoN CON EL SOFTWARE O EL USO U OTRO TIPO
 *	DE ACCIONES EN EL SOFTWARE.
 *	
 *	Ademas, cualquier modificacion realizada por terceros se considerara propiedad del 
 *	titular original de los derechos de autor. Los titulares de derechos de autor 
 *	originales no se responsabilizan de las modificaciones realizadas por terceros.
 *	
 *	Queda explicitamente establecido que no es obligatorio especificar ni notificar los 
 *	cambios realizados entre versiones, ni revelar porciones especificas de codigo 
 *	modificado.
 */

#ifndef __DEBUG_C__
#define __DEBUG_C__
#include "debug_c.h"

// antiguas versiones de esta libreria usaba la macro _ExceptionHandler_WIN_ para activar las excepciones
// se añadio la siguiente linea para que sea retrocompatible con versiones anteriores
#ifdef _ExceptionHandler_WIN_
#define _ExceptionHandler
#endif

#ifdef _ExceptionHandler
#if defined(_WIN64) ||  defined(_WIN32)
static const char* ExceptionCodeDescription( const unsigned int code )
{
    #define STRING_EXCEPTION_WINDOWS(error_define) (const char*)#error_define
    switch( code ) {
        case EXCEPTION_ACCESS_VIOLATION:         return STRING_EXCEPTION_WINDOWS(EXCEPTION_ACCESS_VIOLATION)         ;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return STRING_EXCEPTION_WINDOWS(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)    ;
        case EXCEPTION_BREAKPOINT:               return STRING_EXCEPTION_WINDOWS(EXCEPTION_BREAKPOINT)               ;
        case EXCEPTION_DATATYPE_MISALIGNMENT:    return STRING_EXCEPTION_WINDOWS(EXCEPTION_DATATYPE_MISALIGNMENT)    ;
        case EXCEPTION_FLT_DENORMAL_OPERAND:     return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_DENORMAL_OPERAND)     ;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_DIVIDE_BY_ZERO)       ;
        case EXCEPTION_FLT_INEXACT_RESULT:       return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_INEXACT_RESULT)       ;
        case EXCEPTION_FLT_INVALID_OPERATION:    return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_INVALID_OPERATION)    ;
        case EXCEPTION_FLT_OVERFLOW:             return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_OVERFLOW)             ;
        case EXCEPTION_FLT_STACK_CHECK:          return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_STACK_CHECK)          ;
        case EXCEPTION_FLT_UNDERFLOW:            return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_UNDERFLOW)            ;
        case EXCEPTION_ILLEGAL_INSTRUCTION:      return STRING_EXCEPTION_WINDOWS(EXCEPTION_ILLEGAL_INSTRUCTION)      ;
        case EXCEPTION_IN_PAGE_ERROR:            return STRING_EXCEPTION_WINDOWS(EXCEPTION_IN_PAGE_ERROR)            ;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       return STRING_EXCEPTION_WINDOWS(EXCEPTION_INT_DIVIDE_BY_ZERO)       ;
        case EXCEPTION_INT_OVERFLOW:             return STRING_EXCEPTION_WINDOWS(EXCEPTION_INT_OVERFLOW)             ;
        case EXCEPTION_INVALID_DISPOSITION:      return STRING_EXCEPTION_WINDOWS(EXCEPTION_INVALID_DISPOSITION)      ;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return STRING_EXCEPTION_WINDOWS(EXCEPTION_NONCONTINUABLE_EXCEPTION) ;
        case EXCEPTION_PRIV_INSTRUCTION:         return STRING_EXCEPTION_WINDOWS(EXCEPTION_PRIV_INSTRUCTION)         ;
        case EXCEPTION_SINGLE_STEP:              return STRING_EXCEPTION_WINDOWS(EXCEPTION_SINGLE_STEP)              ;
        case EXCEPTION_STACK_OVERFLOW:           return STRING_EXCEPTION_WINDOWS(EXCEPTION_STACK_OVERFLOW)           ;
        default: return "UNKNOWN EXCEPTION" ;
    }
}

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS *ExceptionInfo) {
    EXCEPTION_RECORD *registro_excepcion = ExceptionInfo->ExceptionRecord;
    #ifdef _MSC_VER
    WOW64_CONTEXT *registro_contexto  = ExceptionInfo->ContextRecord;
    #else
    CONTEXT         *registro_contexto  = ExceptionInfo->ContextRecord;
    #endif

    printf("Se ha producido una excepcion (codigo 0x%lx) en la direccion 0x%p\n\n", 
        registro_excepcion->ExceptionCode, registro_excepcion->ExceptionAddress);

    printf( 
            "\tDWORD  ExceptionCode     = 0x%lu = %s;\n"
            "\tDWORD  ExceptionFlags    = 0x%lu;\n"
            "\tSIZE_T ExceptionRecord   = 0x%p;\n"
            "\tSIZE_T ExceptionAddress  = 0x%p;\n"
            "\tDWORD  NumberParameters  = %lu;\n"
            "\tSIZE_T ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS] = {", 
            registro_excepcion->ExceptionCode, ExceptionCodeDescription(registro_excepcion->ExceptionCode),
            registro_excepcion->ExceptionFlags, 
            (void*)(registro_excepcion->ExceptionRecord),
            registro_excepcion->ExceptionAddress,
            registro_excepcion->NumberParameters
        );
        uint8_t tab = EXCEPTION_MAXIMUM_PARAMETERS / 4;
        for (size_t i = 0; i < EXCEPTION_MAXIMUM_PARAMETERS; i++){
            if ((i % tab) == 0) printf("\n\t\t");
            printf(" %llu,", registro_excepcion->ExceptionInformation[i]);
        }

        printf("\n\t}\n\n");
        #ifdef __x86_64__
        printf("\tDWORD64 P1Home       = %llu;\n", registro_contexto->P1Home);
        printf("\tDWORD64 P2Home       = %llu;\n", registro_contexto->P2Home);
        printf("\tDWORD64 P3Home       = %llu;\n", registro_contexto->P3Home);
        printf("\tDWORD64 P4Home       = %llu;\n", registro_contexto->P4Home);
        printf("\tDWORD64 P5Home       = %llu;\n", registro_contexto->P5Home);
        printf("\tDWORD64 P6Home       = %llu;\n", registro_contexto->P6Home);
        printf("\tDWORD   ContextFlags = %08lx;\n", registro_contexto->ContextFlags);
        printf("\tDWORD   MxCsr        = %08lx;\n", registro_contexto->MxCsr);
        printf("\tWORD    SegCs        = %04x;\n", registro_contexto->SegCs);
        printf("\tWORD    SegDs        = %04x;\n", registro_contexto->SegDs);
        printf("\tWORD    SegEs        = %04x;\n", registro_contexto->SegEs);
        printf("\tWORD    SegFs        = %04x;\n", registro_contexto->SegFs);
        printf("\tWORD    SegGs        = %04x;\n", registro_contexto->SegGs);
        printf("\tWORD    P1Home       = %08llx;\n", registro_contexto->P1Home);
        printf("\tDWORD   EFlags       = %08lx;\n", registro_contexto->EFlags);
        printf("\tDWORD64 Dr0          = %llu;\n", registro_contexto->Dr0);
        printf("\tDWORD64 Dr1          = %llu;\n", registro_contexto->Dr1);
        printf("\tDWORD64 Dr2          = %llu;\n", registro_contexto->Dr2);
        printf("\tDWORD64 Dr3          = %llu;\n", registro_contexto->Dr3);
        printf("\tDWORD64 Dr6          = %llu;\n", registro_contexto->Dr6);
        printf("\tDWORD64 Dr7          = %llu;\n", registro_contexto->Dr7);
        printf("\tDWORD64 Rax          = %llu;\n", registro_contexto->Rax);
        printf("\tDWORD64 Rcx          = %llu;\n", registro_contexto->Rcx);
        printf("\tDWORD64 Rdx          = %llu;\n", registro_contexto->Rdx);
        printf("\tDWORD64 Rbx          = %llu;\n", registro_contexto->Rbx);
        printf("\tDWORD64 Rsp          = %llu;\n", registro_contexto->Rsp);
        printf("\tDWORD64 Rbp          = %llu;\n", registro_contexto->Rbp);
        printf("\tDWORD64 Rsi          = %llu;\n", registro_contexto->Rsi);
        printf("\tDWORD64 Rdi          = %llu;\n", registro_contexto->Rdi);
        printf("\tDWORD64 R8           = %llu;\n", registro_contexto->R8);
        printf("\tDWORD64 R9           = %llu;\n", registro_contexto->R9);
        printf("\tDWORD64 R10          = %llu;\n", registro_contexto->R10);
        printf("\tDWORD64 R11          = %llu;\n", registro_contexto->R11);
        printf("\tDWORD64 R12          = %llu;\n", registro_contexto->R12);
        printf("\tDWORD64 R13          = %llu;\n", registro_contexto->R13);
        printf("\tDWORD64 R14          = %llu;\n", registro_contexto->R14);
        printf("\tDWORD64 R15          = %llu;\n", registro_contexto->R15);
        printf("\tDWORD64 Rip          = %llu;\n", registro_contexto->Rip);

        // Header[0] y Header[1]
        printf("\tM128A   Header[0]    = { Low: %llu, High: %lld };\n",
               registro_contexto->Header[0].Low, registro_contexto->Header[0].High);
        printf("\tM128A   Header[1]    = { Low: %llu, High: %lld };\n",
               registro_contexto->Header[1].Low, registro_contexto->Header[1].High);

        // Legacy[0] a Legacy[7]
        for (int i = 0; i < 8; ++i) {
            printf("\tM128A   Legacy[%d]    = { Low: %llu, High: %lld };\n",
                   i,
                   registro_contexto->Legacy[i].Low,
                   registro_contexto->Legacy[i].High);
        }

        // Xmm0 a Xmm15
        printf("\tM128A   Xmm0         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm0.Low, registro_contexto->Xmm0.High);
        printf("\tM128A   Xmm1         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm1.Low, registro_contexto->Xmm1.High);
        printf("\tM128A   Xmm2         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm2.Low, registro_contexto->Xmm2.High);
        printf("\tM128A   Xmm3         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm3.Low, registro_contexto->Xmm3.High);
        printf("\tM128A   Xmm4         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm4.Low, registro_contexto->Xmm4.High);
        printf("\tM128A   Xmm5         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm5.Low, registro_contexto->Xmm5.High);
        printf("\tM128A   Xmm6         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm6.Low, registro_contexto->Xmm6.High);
        printf("\tM128A   Xmm7         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm7.Low, registro_contexto->Xmm7.High);
        printf("\tM128A   Xmm8         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm8.Low, registro_contexto->Xmm8.High);
        printf("\tM128A   Xmm9         = { Low: %llu, High: %lld };\n", registro_contexto->Xmm9.Low, registro_contexto->Xmm9.High);
        printf("\tM128A   Xmm10        = { Low: %llu, High: %lld };\n", registro_contexto->Xmm10.Low, registro_contexto->Xmm10.High);
        printf("\tM128A   Xmm11        = { Low: %llu, High: %lld };\n", registro_contexto->Xmm11.Low, registro_contexto->Xmm11.High);
        printf("\tM128A   Xmm12        = { Low: %llu, High: %lld };\n", registro_contexto->Xmm12.Low, registro_contexto->Xmm12.High);
        printf("\tM128A   Xmm13        = { Low: %llu, High: %lld };\n", registro_contexto->Xmm13.Low, registro_contexto->Xmm13.High);
        printf("\tM128A   Xmm14        = { Low: %llu, High: %lld };\n", registro_contexto->Xmm14.Low, registro_contexto->Xmm14.High);
        printf("\tM128A   Xmm15        = { Low: %llu, High: %lld };\n", registro_contexto->Xmm15.Low, registro_contexto->Xmm15.High);


        tab = 26 / 5;
        for (size_t i = 0; i < 26; i++){
            if ((i % tab) == 0) printf("\n\t\t");
            M128A val = registro_contexto->VectorRegister[i];
            printf(" %p%p,", (void*)val.High, (void*)val.Low);
        }
        printf("\n\t}\n");
        printf(
            "\tDWORD64 VectorControl        = %llu;\n"
            "\tDWORD64 DebugControl         = %llu;\n"
            "\tDWORD64 LastBranchToRip      = %llu;\n"
            "\tDWORD64 LastBranchFromRip    = %llu;\n"
            "\tDWORD64 LastExceptionToRip   = %llu;\n"
            "\tDWORD64 LastExceptionFromRip = %llu;\n",
            registro_contexto->VectorControl,
            registro_contexto->DebugControl,
            registro_contexto->LastBranchToRip,
            registro_contexto->LastBranchToRip,
            registro_contexto->LastExceptionToRip,
            registro_contexto->LastExceptionFromRip
        );
        #else
        printf(
            "\tDWORD 		ContextFlags = %p;\n"
            "\tDWORD 		Dr0          = %p;\n"
            "\tDWORD 		Dr1          = %p;\n"
            "\tDWORD 		Dr2          = %p;\n"
            "\tDWORD 		Dr3          = %p;\n"
            "\tDWORD 		Dr6          = %p;\n"
            "\tDWORD 		Dr7          = %p;\n"
            "\tDWORD 		SegGs        = %p;\n"
            "\tDWORD 		SegFs        = %p;\n"
            "\tDWORD 		SegEs        = %p;\n"
            "\tDWORD 		SegDs        = %p;\n"
            "\tDWORD 		Edi          = %p;\n"
            "\tDWORD 		Esi          = %p;\n"
            "\tDWORD 		Ebx          = %p;\n"
            "\tDWORD 		Edx          = %p;\n"
            "\tDWORD 		Ecx          = %p;\n"
            "\tDWORD 		Eax          = %p;\n"
            "\tDWORD 		Ebp          = %p;\n"
            "\tDWORD 		Eip          = %p;\n"
            "\tDWORD 		SegCs        = %p;\n"
            "\tDWORD 		EFlags       = %p;\n"
            "\tDWORD 		Esp          = %p;\n"
            "\tDWORD 		SegSs        = %p;\n"
            "\tBYTE			ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION] = ",
            registro_contexto->ContextFlags,
            registro_contexto->Dr0,         registro_contexto->Dr1,
            registro_contexto->Dr2,         registro_contexto->Dr3,
            registro_contexto->Dr6,         registro_contexto->Dr7,
            registro_contexto->SegGs,       registro_contexto->SegFs,
            registro_contexto->SegEs,       registro_contexto->SegDs,
            registro_contexto->Edi,         registro_contexto->Esi,
            registro_contexto->Ebx,         registro_contexto->Edx,
            registro_contexto->Ecx,         registro_contexto->Eax,
            registro_contexto->Ebp,         registro_contexto->Eip,
            registro_contexto->SegCs,       registro_contexto->EFlags,
            registro_contexto->Esp,         registro_contexto->SegSs

        );
        #ifndef MAXIMUM_SUPPORTED_EXTENSION
        #define MAXIMUM_SUPPORTED_EXTENSION 512
        #endif
        tab = MAXIMUM_SUPPORTED_EXTENSION / 16;
        for (size_t i = 0; i < MAXIMUM_SUPPORTED_EXTENSION; i++){
            if ((i % tab) == 0) printf("\n\t\t");
            printf(" %02x", registro_contexto->ExtendedRegisters[i]);
        }
        printf("\n\t}\n");
    #endif

    return EXCEPTION_EXECUTE_HANDLER; // Manejar la excepcion
}
#else // linux

void signalHandler(int sig, siginfo_t *info, void *context) {
    ucontext_t *uc = (ucontext_t *)context;

    printf("Señal capturada: %d (%s)\n", sig, strsignal(sig));
    printf("Dirección de la falla: %p\n", info->si_addr);

    // Mostrar backtrace
    void *buffer[10];
    int frames = backtrace(buffer, 10);
    printf("Backtrace:\n");
    backtrace_symbols_fd(buffer, frames, STDOUT_FILENO);

#if defined(__x86_64__)
    #define REG_RIP 16
    #define REG_RSP 7
    #define REG_RBP 6
    #define REG_RAX 10
    #define REG_RBX 11
    #define REG_RCX 8
    #define REG_RDX 9

    printf("\nRegistros (x86_64):\n");
    printf("RIP: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RIP]);
    printf("RSP: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RSP]);
    printf("RBP: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RBP]);
    printf("RAX: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RAX]);
    printf("RBX: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RBX]);
    printf("RCX: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RCX]);
    printf("RDX: %llx\n", (long long unsigned int)uc->uc_mcontext.gregs[REG_RDX]);
#endif

    exit(EXIT_FAILURE);
}
#endif
#endif

#ifndef _MSC_VER
void __attribute__((constructor))
#endif
__constructor_debug_c__()
{
    //debug_set_log_file("debug_log.txt");
    //open_file(&Log_debug_file, NAME_DEFAULT_LOG_DEBUG, READ_WRITE );
    #if defined(_ExceptionHandler)
        #if (defined(_WIN64) ||  defined(_WIN32))
        SetUnhandledExceptionFilter(ExceptionHandler);
        #else
        struct sigaction sa = {0};
        sa.sa_sigaction = signalHandler;
        sa.sa_flags = SA_SIGINFO;

        // Capturar señales
        sigaction(SIGSEGV, &sa, nullptr); // Segmentation fault
        sigaction(SIGFPE,  &sa, nullptr);  // Floating point exception
        sigaction(SIGILL,  &sa, nullptr);  // Illegal instruction
        sigaction(SIGBUS,  &sa, nullptr);  // Bus error
        #endif
    #endif
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white}[#{FG:red}DEBUG INIT#{FG:white}]\n");
    
    /*if (Log_debug_file.archivo == OPEN_ERROR) {
        printf("Error al abrir el archivo " NAME_DEFAULT_LOG_DEBUG "\n");
        return 1;
    }*/

}
void
#ifndef _MSC_VER
__destructor_debug_c__()
#endif
{
    /*if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }*/
    /*close_file(&Log_debug_file);*/
}

const char*
#ifndef _MSC_VER
__attribute__((pure))
#endif
get_level_debug(const DebugLevel level) {
    if (level <= DEBUG_LEVEL_ERROR) return ErrorLevelStrings[level];
    return ErrorLevelStrings[DEBUG_LEVEL_UNKNOWN];
}  

/*void debug_set_log_file(const char *filename)
{
    if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }

    logFile = fopen(filename, "w");
}*/



void debug_print(const DebugLevel level, const char *fmt, ...)
{
    if (fmt == (const char *)NULL) return;
    if (level < currentLevel)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = (vsnprintf(nullptr, 0, fmt, args_copy) + 1) * sizeof(char);
    va_end(args_copy);
    va_copy(args_copy, args);

    vprintf_color( fmt, args_copy);
    va_end(args_copy);

    /*if (logFile != NULL)
    {
        vfprintf(logFile, fmt, args);
        fprintf(logFile, "\n");
    }*/

    /*unsigned char *formatted_buffer = (unsigned char *)malloc(size);
    vsprintf(formatted_buffer, fmt, args);

    write_file(&Log_debug_file, formatted_buffer);
    if(Log_debug_file.size == WRITE_ERROR){
        printf("Error al escribir el archivo\n");
    }*/

    va_end(args);

    if (level == DEBUG_LEVEL_ERROR)
    {
        va_start(args, fmt);
        char *message = calloc(size, sizeof(char));
        vsnprintf(message, size, fmt, args);
        va_end(args);

        ERROR_C(ERROR_LEVEL_ERROR, message);
        free(message);
    }
}


#endif