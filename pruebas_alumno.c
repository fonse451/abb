#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
int cmp(const char* c1, const char* c2){
    if (atoi(c1) == atoi(c2))return 0;
    else if(atoi(c1) > atoi(c2))return 1;
    return -1;
}


void pruebas_abb_alumno() {
    //CREAR ARBOL VACIO
    
    abb_t* arbol_vacio = abb_crear(cmp,NULL);
    print_test("se creo el arbol vacio", abb_cantidad(arbol_vacio) == 0);
    abb_destruir(arbol_vacio);
    abb_t* arbol_1= abb_crear(cmp,NULL);
    char* val_1= "1";
    char* val_2= "2";
    char* val_3= "4";
    abb_guardar(arbol_1,val_1,val_1);
    print_test("se guardo un elemento en el arbol",abb_cantidad(arbol_1) == 1 );
    print_test("pertenece el 1",abb_pertenece(arbol_1,val_1));
    abb_guardar(arbol_1,val_2,val_2);
    print_test("se guardo dos elemento en el arbol",abb_cantidad(arbol_1) == 2 );
    print_test("pertenece el 2",abb_pertenece(arbol_1,val_2));
    abb_guardar(arbol_1,val_3,val_3);
    print_test("se guardo tres elemento en el arbol",abb_cantidad(arbol_1) == 3 );
    print_test("pertenece el 3",abb_pertenece(arbol_1,val_3));
    
    char* dato = abb_obtener(arbol_1,val_2);
    print_test("el dato es el val_2", atoi(dato)==atoi(val_2));
    char* val_4= "3";
    char* val_5= "5";
    abb_guardar(arbol_1,val_4,val_4);
    abb_guardar(arbol_1,val_5,val_5);
    imprimir(arbol_1);
    print_test("la cantidad es 5", abb_cantidad(arbol_1) == 5);
    abb_borrar(arbol_1,val_5);
    imprimir(arbol_1);
    abb_borrar(arbol_1,val_4);
    imprimir(arbol_1);
    abb_borrar(arbol_1,val_3);
    imprimir(arbol_1);
    abb_borrar(arbol_1,val_2);
    imprimir(arbol_1);
    abb_borrar(arbol_1,val_1);
    imprimir(arbol_1);
    printf("cantidad es %zu\n",abb_cantidad(arbol_1));
    abb_destruir(arbol_1);
}
