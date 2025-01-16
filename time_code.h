#ifndef __TIMER_H__
#define __TIMER_H__

#include "debug_c.h"

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
static LARGE_INTEGER inicio, fin, frecuencia;
#else
#include <time.h>
struct timespec inicio, fin;
#endif
static double tiempo_total;

#ifdef _MSC_VER
void __constructor_time_contador__();
void __destructor_time_contador__();
#else
void __attribute__((constructor)) __constructor_time_contador__();
void __attribute__((destructor)) __destructor_time_contador__();
#endif

#ifdef INCLUDE_COLORS_C
#include "time_code.c"
#endif
#endif