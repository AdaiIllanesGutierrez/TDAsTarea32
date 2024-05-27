Tarea N3 correspondientes a: ```Adai Illanes Gutierrez y Marcel Gutierrez Avila```
# EL PROBLEMA DE 8-PUZZLE:

a)
Para poder compilar se usa este comando(en la carpeta raiz) : 
`````
gcc tdas/*.c tarea3copy.c -Wno-unused-result -o tarea3copy
`````
para luego ejecutar el siguiente comando 
`````
./tarea3copy
`````

## FUNCIONAMIENTO DEL PROGRAMA:
Las tres opciones y las extra funcionan de manera que piden:

Funciones extras:

a) is_final_state: esta como dice el nombre es la encargada para tener el estado final, para asi ir comparando para cuando se llame.

b) State* transition(State* state, int action): Esta es encargada de generar nuevos estados a partir de un  movimiento.

1) BusquedaEnProfundidad(dfs): funciona de buena forma, cumple con lo requiere, el de recorrer, nos sirve para saber cuantas veces se itera  para asi poder encontrar la solucion final al problema.
 ejemplo:
 ```
Ingrese su opción: 1
Ejecutando Búsqueda en Profundidad
Solución encontrada en 290244 iteraciones
1 3 3 2 1 1 3 2 2 
x 1 2 
3 4 5 
6 7 8 

 ```
2) BusquedaEnAnchura(bfs): funciona de manera indicada, ya que esta nos ayuda para encontrar el camino mas corto para poder llegar al final, consume a su vez mas memoria que la dfs.

ejemplo:
```
Ingrese su opción: 2
Ejecutando Búsqueda en Anchura
Solución encontrada en 1408 iteraciones
1 3 3 2 1 1 3 2 2 
x 1 2 
3 4 5 
6 7 8 

```
3) Best_first: Funciona de manera que se pide, la cual sirve para encontrar el mejor primer estado para asi acceder, y tener que iterar la menor cantidad posible.
ejemplo:
```
Ingrese su opción: 3
Ejecutando Búsqueda Mejor Primero
Iteración 1 - Estado actual del puzzle:
x 2 8 
1 3 4 
6 5 7 
.
.
.
Distancia L1: 0
Iteración 30 - Estado actual del puzzle:
x 1 2 
3 4 5 
6 7 8 

Distancia L1: 0
Solución encontrada en 30 iteraciones
1 3 3 2 1 1 3 2 2 
```

# Ejemplos de uso:
```
Estado inicial del puzzle: (0 seria la casilla vacia, que despues se muestra como x).
3 1 2 
0 4 5 
6 7 8 


***** MENÚ ******
========================================
     Escoge método de búsqueda
========================================
1) Búsqueda en Profundidad
2) Búsqueda en Anchura
3) Búsqueda Mejor Primero
4) Salir
Ingrese su opción: 
   ```
aqui debes seleccionar la opcion desde el 1 al 4. En donde si el usuario ingresa algun numero diferente aparecera un error hasta que escojas una opcion valida.

## 1) BúsquedaEnProfundidad(dfs):
Si el usuario ingresa esta opcion, hara que el puzzle busque varias formas de encontrar la solucion, pero usando un camino largo por lo que puede presentar que demore mas pero a la vez usara menos memoria que bfs.
ejemplo:
```
Ingrese su opción: 1
Ejecutando Búsqueda en Profundidad
3 1 2 
x 4 5 
6 7 8 

3 1 2 
x 4 5 
6 7 8 

3 1 2 
4 x 5 
6 7 8 

3 1 2 
6 4 5 
x 7 8 

x 1 2 
3 4 5 
6 7 8 

Solución encontrada en 4 iteraciones

x 1 2 
3 4 5 
6 7 8 
```
## 2) BúsquedaEnAnchura(bfs): 
al ingresar esta opcion hara que consuma mas memoria que dfs pero hara que encuentre la solucion en una menor cantidad comparado al dfs.
ejemplo:
```
Ingrese su opción: 2
Ejecutando Búsqueda en Anchura
3 1 2 
x 4 5 
6 7 8 

x 1 2 
3 4 5 
6 7 8 

Solución encontrada en 2 iteraciones

```
## 3)BúsquedaMejorPrimero(best_first):
si el usuario ingresa esta opcion, hara que se muestre el caso o estado mas optimo para poder seguir avanzando.
ejemplo:
```
Ingrese su opción: 3
Ejecutando Búsqueda Mejor Primero
Iteración 1 - Estado actual del puzzle:
3 1 2 
x 4 5 
6 7 8 

Distancia L1: 2
Iteración 2 - Estado actual del puzzle:
x 1 2 
3 4 5 
6 7 8 

Distancia L1: 0
Solución encontrada en 2 iteraciones
```
## Contribuciones por integrante:
En esta parte ambos realizamos el codigo y contribuimos en el desarrollo

### Marcel Gutierrez 
- ayudo y creo una la funcion de busqueda en anchura(bfs) la cual era primitiva a la que al final termino siendo usada.
- ayudo tambien en las ideas y en la implementacion de la Busqueda mejor primero.
- Documentacion del README.
- **Auto-evaluacion** 2(Aporte bueno).


### Adai Illanes 

- Implemento la funcion de Busqueda En Profundidad(dfs)
- Implemento la funcion de Búsqueda en Anchura(bfs)
- implemento la Búsqueda Mejor Primero
- Documentación y comentarios en el código.
- **Auto-evaluación**: 3 (Aporte excelente).

