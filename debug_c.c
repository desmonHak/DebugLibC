#ifndef __DEBUG_C__
#define __DEBUG_C__
#include "debug_c.h"

void __attribute__((constructor)) __constructor_debug_c__(){
    //debug_set_log_file("debug_log.txt");
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white}[#{FG:red}DEBUG INIT#{FG:white}]");
}
void __attribute__((destructor)) __destructor_debug_c__(){
    if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }
}

const char* get_level_debug(DebugLevel level) {
    switch(level){

    case DEBUG_LEVEL_INFO:
        return ErrorLevelStrings[0];
        break;
    case DEBUG_LEVEL_WARNING:
        return ErrorLevelStrings[1];
        break;
    case DEBUG_LEVEL_ERROR:
        return ErrorLevelStrings[2];
        break;
    default:
        return "UNKNOW";
        break;
    };
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

void debug_set_level(DebugLevel level)
{
    currentLevel = level;
}

void debug_print(DebugLevel level, const char *fmt, ...)
{
    if (level < currentLevel)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = (vsnprintf(NULL, 0, fmt, args_copy) + 1) * sizeof(char);
    va_end(args_copy);
    va_copy(args_copy, args);

    vprintf_color( fmt, args_copy);
    va_end(args_copy);
    putchar( '\n');

    /*if (logFile != NULL)
    {
        vfprintf(logFile, fmt, args);
        fprintf(logFile, "\n");
    }*/

    va_end(args);

    if (level == DEBUG_LEVEL_ERROR)
    {
        va_start(args, fmt);
        char *message = malloc(size * sizeof(char));
        vsnprintf(message, size, fmt, args);
        va_end(args);
        
        ERROR_C(ERROR_LEVEL_ERROR, message);
    }
}
#endif