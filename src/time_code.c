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

#ifndef __TIMER_C__
#define __TIMER_C__
#include "time_code.h"

#ifdef _WIN32
void __constructor_time_contador__() {
    // Obtiene la frecuencia del contador de rendimiento
    QueryPerformanceFrequency(&frecuencia);
    // Inicio del temporizador
    QueryPerformanceCounter(&inicio);
}
void __destructor_time_contador__(){
    // Fin del temporizador
    QueryPerformanceCounter(&fin);
    // Calcula el tiempo total de ejecución en segundos
    tiempo_total = (double)(fin.QuadPart - inicio.QuadPart) / frecuencia.QuadPart;
    debug_set_level(DEBUG_LEVEL_INFO);
    SET_FG_LIGHTYELLOW;
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white} El programa tardo #{FG:red}%.6f#{FG:white} segundos en ejecutarse.\n", tiempo_total);
    
}
#else
void __constructor_time_contador__() {
    // Inicio del temporizador
    clock_gettime(CLOCK_MONOTONIC, &inicio);
}

void __destructor_time_contador__(){
    // Fin del temporizador
    clock_gettime(CLOCK_MONOTONIC, &fin);

    // Calcula el tiempo total de ejecución en segundos
    tiempo_total = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
    //debug_set_log_file("debug_log.txt");
    debug_set_level(DEBUG_LEVEL_INFO);
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "El programa tardo #{FG:red}%.6f#{FG:white} segundos en ejecutarse.\n", tiempo_total);
}
#endif

#endif