#include "ABB.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//ESTRUCTURAS
//brian te comiste todos los punto y coma despues de las llaves de los struct
struct nodo_abb{
        struct nodo_abb* nodo_izquierdo;
        struct nodo_abb* nodo_derecho;
        char* clave;
        void* dato;
};
        
typedef struct nodo_abb nodo_t;        


struct abb{
        nodo_abb* raiz;                                     //#cantidad?
        abb_destruir_dato_t* destruir_dato;
        abb_comparar_clave_t* funcion_de_comparacion;


};


struct abb_iterador{
    pila_t* pila;
    abb_t* arbol;
};
typedef struct abb_iter abb_iter_t;



//FUNCIONES AUXILIARES

char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

bool abb_asignar(abb_t* arbol, nodo_t* nodo, const char* clave, void* dato){
    if (arbol->funcion_de_comparacion(clave,nodo->clave) == 0){
        return true;// or false //Y QUE HACES CON LA MEMORIA QUE PEDISTE PARA LA CLAVE?
    }
    else if(arbol->funcion_de_comparacion(clave,nodo->clave)>0){
        if(nodo->derecho == NULL){                                      //cmp si devuelve 0 son iguales
            nodo_t* nodo_auxiliar = nodo_crear();                                      // - si el primero menor
            nodo_auxiliar->clave = strdup(clave);                                    // + si el primero mayor
            nodo_auxiliar->dato = dato;             //clave= NULL?
            nodo->derecho= nodo_auxiliar;
            return true;
        }
        else{
            return abb_asignar(arbol,nodo->derecho,clave,dato);
        }
    else{
        if (nodo->izquierdo == NULL){
            nodo_t* nodo_auxiliar = nodo_crear();
            nodo_auxiliar->clave = strdup(clave);
            nodo_auxiliar->dato = dato;
            nodo->izquierdo = nodo_auxiliar;
            return true;
        }
        else{
            return abb_asignar(arbol,nodo->izquierdo,clave,dato);
        }
    }
}
    
bool abb_auxiliar_pertenece(abb_t* arbol,nodo_t* nodo,clave){ //falta el tipo de la clave const char*
    if (nodo == NULL){                                     //el tipo del arbol deberia ser const tambien
        return false;
    }
        if(arbol->funcion_de_comparacion(clave,nodo->clave)==0){
        return true;
    }
    bool respuesta_1 = abb_auxiliar_pertenece(arbol,nodo->izquierdo,clave);
    bool respuesta_2 = abb_auxiliar_pertenece(arbol,nodo->derecho,clave);
    if (respuesta_1 || respuesta_2){
        return true;
    }
    return false;
}
    
void* abb_obtener_auxiliar(abb_t* arbol,nodo_t* nodo, clave); //tipo const del arbol y clave
    if(arbol->funcion_de_comparacion(nodo->clave,clave) == 0){
        return nodo->clave->dato;
    }
    else if(arbol->funcion_de_comparacion(nodo->clave,clave) > 0){
        return abb_obtener_auxiliar(arbol,nodo->derecho,clave);
    }
    else{
        return abb_obtener_auxiliar(arbol,nodo->izquierdo,clave);
    }
}

size_t abb_cantidad_auxiliar(abb_t* arbol, nodo_t* nodo){
    if (nodo == NULL){
        return 0;
    }
    size_t cantidad_izquierda = abb_auxiliar(arbol,nodo->izquierdo); //DONDE ESTA EL ABB_AUXILIAR
    size_t cantidad_derecha = abb_auxiliar(arbol,nodo->derecho);
    return cantidad_izquierda + cantidad_derecha +1;
}

void abb_destruir_auxiliar(abb_t* arbol, nodo_t* nodo){
    if (nodo == NULL){
        return;
    }
    else if (!nodo->izquierdo && !nodo->derecho){ //Esto quiere decir que es una hoja |
        if (arbol->destruir_dato){                                                |//ESTO LO HACE SI LO
            destruir_dato(nodo->dato);                                        |//BORRAS
        }
        free(nodo);//Liberamos el nodo asociado
        return;//Cortamos la funcion porque solo nos interesa que haga esto en las hojas
    }
    else{//Como no es una hoja, hay que seguir avanzando en ambos lados
        abb_destruir_auxiliar(arbol,nodo->izquierdo);
        abb_destruir_auxiliar(arbol,nodo->derecho);
    }
    //Aca ya es una hoja el nodo porque borramos sus hijos (POBRECITO U.U)
    if (arbol->destruir_dato){
        destruir_dato(nodo->dato);
    }
    free(nodo);
    return;
}

void auxiliar_creacion(abb_iter_t* iter,nodo_t* nodo){
    if(nodo == NULL){
        return;
    }
    pila_apilar(nodo);
    auxiliar_creacion(iter,nodo->izquierdo);
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
        arbol->destruir_dato = destruir dato;
        arbol->funcion_de_comparacion = cmp;
        return arbol;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
        if (arbol->raiz == NULL){
                arbol->raiz->clave = strdup(clave);
                arbol->raiz->dato = dato;
                arbol->altura++; //abb no tiene altura
                return true;
        }
        return abb_asignar(raiz,clave,dato);
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
        if (arbol->raiz == NULL){
                return false;
        }
        return abb_auxiliar_pertenece(arbol,raiz,clave);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
        if(!abb_pertenece((const abb_t*) arbol,clave){
                return NULL;
        }
        void* dato = abb_obtener_auxiliar(arbol,raiz,clave);
        return dato;
}

size_t abb_cantidad(abb_t *arbol){
        return abb_cantidad_auxiliar(arbol,arbol->raiz);
}

void abb_destruir(abb_t *arbol){
        abb_destruir_auxiliar(arbol,arbol->raiz);
        free(arbol);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
        if (arbol->raiz == NULL){
                return;
        }
        abb_visitar(raiz,visitar,extra);
}


void abb_visitar(nodo_t* nodo,bool visitar(const char*, void*,void*), void *extra){
        if(!nodo){
                return;
        }
        abb_visitar(nodo->izquierdo,visitar,extra);
        visitar(nodo->clave,nodo->dato,extra);
        abb_visitar(nodo->derecho,visitar,extra);
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
        pila_apilar(arbol->raiz);
        auxiliar_creacion(iter); // no le pasas ningun nodo calculo que deberias borrar la linea de arriba
        return iter;             // y pasarle arbol->raiz->dato
}


const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
        nodo_t* nodo = ver_tope(iter->pila);
        if (nodo == NULL){ // podria ser !nodo
                return NULL;
        }
        return nodo->clave; // no seria el dato
}


bool abb_iter_in_al_final(const abb_iter_t *iter){
        if (pila_esta_vacia(iter->pila)){
                return true;
        }
        return false; //esto se resume en return pila_esta_vacia(iter->pila)
}


bool abb_iter_in_avanzar(abb_iter_t *iter){
        if (abb_iter_in_al_final(iter)){
                return false;
        }
        nodo_t* nodo = pila_desapilar(iter->pila);
        auxiliar_creacion(iter,nodo->derecho);
        return true;
        
}