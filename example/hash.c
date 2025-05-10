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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define _ExceptionHandler
#define DEBUG_ENABLE 
#include "time_code.h"

typedef struct Entry {
    char* key;
    void* value;
    struct Entry* next;
} Entry;

typedef struct HashTable {
    size_t size;
    size_t capacity;
    Entry** table;
} HashTable;

unsigned long hash(const char* str, size_t size) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(unsigned long, hash)
                TYPE_DATA_DBG(const char*, "str = %s")
                TYPE_DATA_DBG(size_t, "size = %zu")
            END_TYPE_FUNC_DBG,
            str, size);
    #endif
    size_t hash = 0x1505;
    int c;
    while ((c = *str++)) { 
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c  djb2 hash algorithm */
    }

    return hash % size;
}

HashTable* createHashTable(size_t size) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(HashTable*, createHashTable)
                TYPE_DATA_DBG(size_t, "size = %zu")
            END_TYPE_FUNC_DBG,
            size);
    #endif
    HashTable* hashTable; //= (HashTable*)malloc(sizeof(HashTable));
    debug_malloc(HashTable, hashTable, sizeof(HashTable) * 1);

    hashTable->size = 0;
    hashTable->capacity = size;
    //hashTable->table = (Entry**)calloc(size, sizeof(Entry*));
    debug_calloc(Entry*, hashTable->table, size, sizeof(Entry*));
    return hashTable;
}

void put(HashTable* hashTable, const char* key, void* value) {
    if ((key == value) || (key == (const char*)NULL) || (hashTable == NULL)) {
        #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_WARNING,
            INIT_TYPE_FUNC_DBG(void, put)
                TYPE_DATA_DBG(HashTable*, "hashTable = %p")
                TYPE_DATA_DBG(const char*, "key = %s")
                TYPE_DATA_DBG(void*, "value = %p")
            END_TYPE_FUNC_DBG,
            hashTable, key, value);
        #endif
        return;
    }
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(void, put)
                TYPE_DATA_DBG(HashTable*, "hashTable = %p")
                TYPE_DATA_DBG(const char*, "key = %s")
                TYPE_DATA_DBG(void*, "value = %p")
            END_TYPE_FUNC_DBG,
            hashTable, key, value);
    #endif

    size_t index = hash(key, hashTable->capacity);
    //printf("put: %zu\n", index);

    //Entry* entry = (Entry*)malloc(sizeof(Entry));
    Entry* entry = NULL;
    debug_malloc(Entry, entry, sizeof(Entry));

    entry->key = strdup(key);
    entry->value = value;
    entry->next = NULL;

    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = entry;
        hashTable->size++;
    } else {
        Entry* current = hashTable->table[index];
        while (current->next != NULL) {
            if (strcmp(current->key, key) == 0) {
                current->value = value;
                free(entry->key);
                free(entry);
                return;
            }
            current = current->next;
        }
        current->next = entry;
    }

    // Check if table needs to be resized
    if (hashTable->size >= hashTable->capacity) {
        size_t newSize = hashTable->capacity * 2;
        Entry** newTable = (Entry**)calloc(newSize, sizeof(Entry*));

        // Rehash and reinsert existing elements
        for (size_t i = 0; i < hashTable->capacity; i++) {
            Entry* entry = hashTable->table[i];
            while (entry != NULL) {
                Entry* next = entry->next;
                size_t newIndex = hash(entry->key, newSize);
                entry->next = newTable[newIndex];
                newTable[newIndex] = entry;
                entry = next;
            }
        }

        free(hashTable->table);
        hashTable->table = newTable;
        hashTable->capacity = newSize;
    }
}


void* get(HashTable* hashTable, const char* key) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(void*, get)
                TYPE_DATA_DBG(HashTable*, "hashTable = %p")
                TYPE_DATA_DBG(const char*, "key = %s")
            END_TYPE_FUNC_DBG,
            hashTable, key);
    #endif
    #ifdef __ERROR_H__ 
    if (hashTable == NULL){
        debug_set_level(DEBUG_LEVEL_WARNING);
        DEBUG_PRINT(DEBUG_LEVEL_WARNING, "get(HashTable* hashTable = %p, const char* key = NULL(%p))\n", hashTable, key);
    }
    #endif

    size_t index = hash(key, hashTable->capacity);

    Entry* entry = hashTable->table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL; // Key not found
}

void printHashTable(HashTable* hashTable) {

    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(void, printHashTable)
                TYPE_DATA_DBG(HashTable*, "hashTable = %p")
            END_TYPE_FUNC_DBG,
            hashTable);

    if (hashTable == NULL){
        debug_set_level(DEBUG_LEVEL_INFO);
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "printHashTable: NULL(%p)\n", hashTable);
    }
    #endif
    for (size_t i = 0; i < hashTable->capacity; i++) {
        Entry* entry = hashTable->table[i];
        while (entry != NULL) {
            printf("Key: %s, Value: %p\n", entry->key, entry->value);
            entry = entry->next;
        }
    }
}

void freeHashTable(HashTable* hashTable) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(void, freeHashTable)
                TYPE_DATA_DBG(HashTable*, "hashTable = %p")
            END_TYPE_FUNC_DBG,
            hashTable);

    if (hashTable == NULL){
        debug_set_level(DEBUG_LEVEL_INFO);
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "freeHashTable: NULL(%p)\n", hashTable);
    }
    #endif
    for (size_t i = 0; i < hashTable->size; i++) {
        Entry* entry = hashTable->table[i];
        while (entry != NULL) {
            Entry* temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
        hashTable->table[i] = NULL; // Set the table slot to NULL after freeing entries
    }
}



int main() {


    HashTable* hashTable = createHashTable(100);

    // Insert values
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    put(hashTable, "key1", &value1);
    put(hashTable, "key2", &value2);
    put(hashTable, "key3", &value3);
    put(hashTable, NULL, &value1);

    // hacer un ejemplo insertando 200 valores en la tabla de hash
    //debug_set_log_file("debug_log.txt");
    for (size_t i = 0; i < 200; i++) {
        int* val = malloc(sizeof(int));
        *val = i;
        unsigned char key[2];
        key[0] = 'A' + i% 'Z'; // Assuming the keys are 'A' to 'J'
        key[1] = '\0';
        //printf("%s\n", key);

        put(hashTable, key, val);
        debug_set_level(DEBUG_LEVEL_INFO);
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white}Value for key '%s': %d#{reset}\n", key, *((unsigned char*)get(hashTable, key)));
    }

    // Get values
    printf("Value for key 'key1': %d\n", *(int*)get(hashTable, "key1"));
    printf("Value for key 'key2': %d\n", *(int*)get(hashTable, "key2"));
    printf("Value for key 'key3': %d\n", *(int*)get(hashTable, "key3"));

    // Print hash table
    printf("Hash Table:\n");
    printHashTable(hashTable);

    // Print size and capacity
    printf("Size: %zu\n", hashTable->size);
    printf("Capacity: %zu\n", hashTable->capacity);

    /*
    int *a = 1;
    *a = 1;
    */

    //print_table_hex(" ", hashTable, 500);

    // Free memory
    freeHashTable(hashTable);
    puts("memoria liberada");

    return 0;
}
