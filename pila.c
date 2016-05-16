#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t tam;
    size_t largo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...

#define TAM_INI 20
#define LARGO_INI 0


pila_t* pila_crear(void){
    pila_t* pila= malloc(sizeof(pila_t));
    
    if (pila == NULL) {
        return NULL;
    }
    pila->datos = malloc(TAM_INI * sizeof(void*));
    
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    
    pila->tam = TAM_INI;
    pila->largo = LARGO_INI;
    return pila;
}

bool pila_redimensionar(pila_t* pila, size_t nuevo_tam) {
    
    void** datos_nuevo = realloc(pila->datos, nuevo_tam * sizeof(void*));
    

    if (datos_nuevo == NULL) {
        return false;
    }
    
    pila->datos = datos_nuevo;
    pila->tam = nuevo_tam;
    return true;
}



bool pila_apilar(pila_t *pila, void* valor){
    
    if (pila->largo == pila->tam){
        size_t nuevo_tam =  pila->tam*2;
        
        if (pila_redimensionar(pila,nuevo_tam)){
            pila->datos[pila->largo] = valor;
            pila->largo++;
            return true;
        }
        return false;
    }
    
    pila->datos[pila->largo] = valor;
    pila->largo++ ;
    return true;
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->largo == 0);
}

void* pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    return  pila->datos[pila->largo-1];
}

void* pila_desapilar(pila_t *pila){
    
    if (pila->tam > TAM_INI  && pila->largo < pila->tam/3){
        size_t nuevo_tam = pila->tam/2;
        pila_redimensionar(pila,nuevo_tam);
    }
    
    if (!pila_esta_vacia(pila)){
        void* desapilado= pila_ver_tope(pila);
        pila->largo--;
        return desapilado;
        
    }
    return NULL;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}
