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

#ifndef __TIMER_H__
#define __TIMER_H__

#include "debug_c.h"

#ifndef UNUSED
#   if defined(__GNUC__) || defined(__clang__)
#      define UNUSED __attribute__((unused))
#   elif defined(_MSC_VER)
#      define UNUSED __pragma(warning(suppress:4101))
#   else
#      define UNUSED
#   endif
#endif

#include <stdio.h>
#ifdef _WIN32
#   include <windows.h>
static LARGE_INTEGER UNUSED inicio, UNUSED fin, UNUSED frecuencia;
#else
#   include <time.h>
static struct timespec UNUSED inicio, UNUSED fin;
#endif
static double UNUSED tiempo_total;

#ifdef _MSC_VER
void __constructor_time_contador__();
void __destructor_time_contador__();
#else
void __attribute__((constructor)) __constructor_time_contador__();

void __attribute__((destructor)) __destructor_time_contador__();
#endif

#ifdef INCLUDE_COLORS_C
#   include "time_code.c"
#endif
#endif
