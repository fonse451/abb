#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "pila.h"

//ESTRUCTURAS
struct nodo_abb{
        struct nodo_abb* nodo_izquierdo;
        struct nodo_abb* nodo_derecho;
        char* clave;
        void* dato;
};
        
typedef struct nodo_abb nodo_t;        


struct abb{
    nodo_t* raiz;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t funcion_de_comparacion;
    size_t cantidad;

};


struct abb_iter{
    pila_t* pila;
    const abb_t* arbol;
};




//FUNCIONES AUXILIARES

char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

bool abb_asignar(abb_t* arbol, nodo_t* nodo, const char* clave, void* dato){
    if (arbol->funcion_de_comparacion(clave,nodo->clave) == 0){
        return true;
    }
	nodo_t* nodo_auxiliar=malloc(sizeof(nodo_t));
    if(arbol->funcion_de_comparacion(clave,nodo->clave)>0){
        if(!nodo->nodo_derecho){                                      //cmp si devuelve 0 son iguales
								                                     // - si el primero menor
            nodo_auxiliar->clave = strdup(clave);                       // + si el primero mayor
            if(!nodo_auxiliar->clave)return false;
            nodo_auxiliar->dato = dato;
            nodo->nodo_derecho= nodo_auxiliar;
            arbol->cantidad++;
            return true;
        }
        else{
            return abb_asignar(arbol,nodo->nodo_derecho,clave,dato);
        }
    }
    else{
        if (!nodo->nodo_izquierdo){

            nodo_auxiliar->clave = strdup(clave);
            nodo_auxiliar->dato = dato;
            nodo->nodo_izquierdo = nodo_auxiliar;
            arbol->cantidad++;
            return true;
        }
        else{
            return abb_asignar(arbol,nodo->nodo_izquierdo,clave,dato);
        }
    }
}
    
bool abb_auxiliar_pertenece(const abb_t* arbol,nodo_t* nodo,const char* clave){
    if (nodo == NULL){
        return false;
    }
    if(arbol->funcion_de_comparacion(clave,nodo->clave)==0){
        return true;
    }
    bool respuesta_1 = abb_auxiliar_pertenece(arbol,nodo->nodo_izquierdo,clave);
    bool respuesta_2 = abb_auxiliar_pertenece(arbol,nodo->nodo_derecho,clave);
    if (respuesta_1 || respuesta_2){
        return true;
    }
    return false;
}
    
void* abb_obtener_auxiliar(const abb_t* arbol,nodo_t* nodo, const char* clave){
    if(arbol->funcion_de_comparacion(nodo->clave,clave) == 0){
        return nodo->dato;
    }
    else if(arbol->funcion_de_comparacion(nodo->clave,clave) > 0){
        return abb_obtener_auxiliar(arbol,nodo->nodo_derecho,clave);
    }
    else{
        return abb_obtener_auxiliar(arbol,nodo->nodo_izquierdo,clave);
    }
}


void abb_destruir_auxiliar(abb_t* arbol, nodo_t* nodo){
    if (nodo == NULL){
        return;
    }
    else{//Como no es una hoja, hay que seguir avanzando en ambos lados
        abb_destruir_auxiliar(arbol,nodo->nodo_izquierdo);
        abb_destruir_auxiliar(arbol,nodo->nodo_derecho);
    }
    //Aca ya es una hoja el nodo porque borramos sus hijos (POBRECITO U.U)
    if (!arbol->destruir_dato){
        arbol->destruir_dato(nodo->dato);
    }
    free(nodo);
    return;
}

void auxiliar_creacion(abb_iter_t* iter,nodo_t* nodo){
    if(nodo == NULL){
        return;
    }
    pila_apilar(iter->pila,nodo);
    auxiliar_creacion(iter,nodo->nodo_izquierdo);
}

//PRIMITIVAS DEL ARBOL
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
        if (cmp == NULL){
            return NULL;
        }

        abb_t* arbol = malloc(sizeof(abb_t));
        if (arbol == NULL){
                return NULL;
        }
        arbol->raiz = NULL;
        arbol->destruir_dato = destruir_dato;
        arbol->funcion_de_comparacion = cmp;
        arbol->cantidad = 0;
        return arbol;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
        if (arbol->raiz == NULL){
                arbol->raiz->clave = strdup(clave);
                arbol->raiz->dato = dato;
                arbol->cantidad++;
                return true;
        }
        return abb_asignar(arbol,arbol->raiz,clave,dato);
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
        if (arbol->raiz == NULL){
                return false;
        }
        return abb_auxiliar_pertenece(arbol,arbol->raiz,clave);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
        if(!abb_pertenece(arbol,clave)){
                return NULL;
        }
        void* dato = abb_obtener_auxiliar(arbol,arbol->raiz,clave);
        return dato;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}
void* abb_borrar_auxiliar(abb_t *arbol,nodo_t* nodo,const char* clave, nodo_t* padre);
void *abb_borrar(abb_t *arbol, const char *clave){
    
    if(!arbol->raiz)return NULL;
    else if (!abb_pertenece(arbol,clave))return NULL;
    void* dato;
	if(!arbol->raiz->nodo_derecho && !arbol->raiz->nodo_izquierdo){
        dato = arbol->raiz->dato;
        free(arbol->raiz->clave);
        free(arbol->raiz);
    }
    else{
        dato = abb_borrar_auxiliar(arbol,arbol->raiz,clave, NULL);
    }
    arbol->cantidad--;
    return dato;
}
void verificar_borrar(abb_t *arbol,nodo_t* nodo,nodo_t* padre);           
void swap_mgc_nodo(abb_t* arbol,nodo_t* nodo,nodo_t* padre, nodo_t* mgc);
nodo_t* buscar_padre_de_mgc(abb_t* arbol,nodo_t* nodo, const char* clave);
nodo_t* buscar_de_grande_el_mas_chico(nodo_t* nodo);

void* abb_borrar_auxiliar(abb_t *arbol,nodo_t* nodo,const char* clave, nodo_t* padre){
        if (arbol->funcion_de_comparacion(clave,nodo->clave) == 0){
            void* dato = nodo->dato;
            verificar_borrar(arbol,nodo,padre);
            return dato;
        }
        else if(arbol->funcion_de_comparacion(clave,nodo->clave)>0){
                return abb_borrar_auxiliar(arbol,nodo->nodo_derecho,clave,nodo);
        }                                                                     //cmp si devuelve 0 son iguales
                                                                            // - si el primero menor
                                                                            // + si el primero mayor
        else{
            return abb_borrar_auxiliar(arbol,nodo->nodo_izquierdo,clave,nodo);
        }
}

bool hijo_unico(nodo_t* nodo){
    return (!nodo->nodo_derecho && !nodo->nodo_izquierdo);
}
           
void verificar_borrar(abb_t *arbol,nodo_t* nodo,nodo_t* padre){
    if (hijo_unico(nodo));
    else if(!nodo->nodo_izquierdo && hijo_unico(nodo->nodo_derecho)){
        padre->nodo_derecho = nodo->nodo_derecho;
    }
    else if(!nodo->nodo_derecho && hijo_unico(nodo->nodo_izquierdo)){
        padre->nodo_izquierdo = nodo->nodo_izquierdo;
    }
    else{
        nodo_t* mgc = buscar_de_grande_el_mas_chico(nodo->nodo_derecho); //mgc "MAS GRANDE CHICO"
        if(mgc->nodo_derecho){
            nodo_t* padre_mgc = buscar_padre_de_mgc(arbol,nodo->nodo_derecho, mgc->clave);
            padre_mgc->nodo_izquierdo = mgc->nodo_derecho;
        }
        swap_mgc_nodo(arbol,nodo,padre,mgc);
    }
    free(nodo->clave);
    free(nodo);
    return;
}

void swap_mgc_nodo(abb_t* arbol,nodo_t* nodo,nodo_t* padre, nodo_t* mgc){
    if (arbol->funcion_de_comparacion(padre->clave,nodo->clave) >0){
        padre->nodo_izquierdo = mgc;
        mgc->nodo_izquierdo = nodo->nodo_izquierdo;
        mgc->nodo_derecho = nodo->nodo_derecho;
    }
    else{
        padre->nodo_derecho = mgc;
        mgc->nodo_izquierdo = nodo->nodo_izquierdo;
        mgc->nodo_derecho = nodo->nodo_derecho;
    }
}
           
nodo_t* buscar_padre_de_mgc(abb_t* arbol,nodo_t* nodo, const char* clave){
    if(arbol->funcion_de_comparacion(clave,nodo->nodo_izquierdo->clave) == 0){
        return nodo;
    }
    return buscar_padre_de_mgc(arbol,nodo->nodo_izquierdo,clave);
}
           
nodo_t* buscar_de_grande_el_mas_chico(nodo_t* nodo){
    if(!nodo->nodo_izquierdo){
        return nodo;
    }
    return buscar_de_grande_el_mas_chico(nodo->nodo_izquierdo);
}
           
void abb_visitar(nodo_t* nodo,bool visitar(const char*, void*,void*), void *extra){
    if(!nodo){
            return;
    }
    abb_visitar(nodo->nodo_izquierdo,visitar,extra);
    visitar(nodo->clave,nodo->dato,extra);
    abb_visitar(nodo->nodo_derecho,visitar,extra);
}

void abb_destruir(abb_t* arbol){
    abb_destruir_auxiliar(arbol, arbol->raiz);
    free(arbol);
}
           
           
//PRIMITIVAS DEL ITERADOR EXTERNO



abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
        abb_iter_t* iter = malloc(sizeof(abb_iter_t));
        if (iter == NULL){
                return NULL;
        }
        iter->pila = pila_crear();
        if (iter->pila == NULL){
                free(iter);
                return NULL;
        }
        iter->arbol = arbol;
    
        auxiliar_creacion(iter,arbol->raiz);
        return iter;
}


const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
        nodo_t* nodo = pila_ver_tope(iter->pila);
        if (!nodo)return NULL;
        return nodo->clave;
}


bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}


bool abb_iter_in_avanzar(abb_iter_t *iter){
        if (abb_iter_in_al_final(iter)){
                return false;
        }
        nodo_t* nodo = pila_desapilar(iter->pila);
        auxiliar_creacion(iter,nodo->nodo_derecho);
        return true;
}
