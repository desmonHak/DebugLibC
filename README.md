# DebugLibC

----

## Libreria debug_c.h

Una pequeña lib para el debug y mostrar informacion.

Se hace uso de la libreria [colors](https://github.com/desmonHak/colors-C-C-plus-plus)


La libreria usa la macro `DEBUG_ENABLE` para habilitar o deshabilitar el debug.

Usted debera compilar usando `-DDEBUG_ENABLE` para habilitar el debug tal como se muestra:

```bash
gcc -DDEBUG_ENABLE main.c -o main.elf
```

Todos lo relacionado con el debug se encuentra en el archivo `debug_c.h` y `debug_c.c`. El tema de los colores se encuentra en el archivo `colors.h` y `colors.c` el cual permite que podamos usar los colores en la consola. Si desea desabilitarlos compile con `__DISABLE_COLORS_FORE_BACK_GROUND__`, eso hara que las siguientes funciones no hagan nada: `background_color_custom_RGB`, `background_color_custom_`, `foreground_color_custom_RGB` y `foreground_color_custom_`. Se recomienda hacer esto en versiones de Windows 7, pues en algunos no funciona como deberia.

En caso de que en su codigo se producto algun tipo de error, al incluiir la libreria `debug_c.c` se mostrara un mensaje de error y el programa se cerrara. Se define los metodos `signalHandler`(linux) y el metodo `ExceptionHandler` (windows) para poder capturar el error y mostrarlo, junto al estado de los registros. (debera activar la macro `_ExceptionHandler`) para que este comportamiento funcione, podra hacerlo añadiendo `#define _ExceptionHandler` antes de incluir la libreria `debug_c.c` o la libreria `time_code.c` la cual ya incluye la anterior.

> Cabe mencionar que tambiene puede usar la macro ``_ExceptionHandler_WIN_``  que hara lo mismo que 
> `_ExceptionHandler`, la primera se mantiene por retrocompatibilidad con versiones anteriores de la > libreria.

### macro ``DEBUG_PRINT``

La macro `DEBUG_PRINT` es una macro que permite imprimir mensajes de debug en la consola, esta macro se define en el archivo `debug_c.h` y se define como:
```c
#define DEBUG_PRINT(level, fmt, ...) debug_print(level, "#{FG:lgreen}[#{FG:lpurple}%s#{FG:reset}:#{FG:cyan}%s#{FG:reset}:#{FG:red}%03d#{FG:green}]#{FG:reset} " fmt , get_level_debug(level), __FILE__, __LINE__, ##__VA_ARGS__)
```

Esta macro solo sera usada si la macro `DEBUG_ENABLE` esta definida. Esta funcion usa


----

## Libreria time_code.h

la pequeña libreria incluye a la libreria `debug_c.h` y agrega funcionalidades para medir el tiempo de ejecucion de su codigo, usted no tendra que llamar a ninguna funcion adicional para hacer esto, solo debe incluir la libreria `time_code.h`

----