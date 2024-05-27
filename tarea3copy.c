#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/queue.h"

// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;            // Posición x del espacio vacío
    int y;            // Posición y del espacio vacío
    List* actions;    // Secuencia de movimientos para llegar al estado
    int steps;
} State;

// Función para calcular la distancia L1
int distancia_L1(State* state) {
    int ev = 0;
    int final[3][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state->square[i][j] != 0 && state->square[i][j] != final[i][j]) {
                int target_i = (state->square[i][j] - 1) / 3;
                int target_j = (state->square[i][j] - 1) % 3;
                ev += abs(target_i - i) + abs(target_j - j);
            }
        }
    }
    return ev;
}

// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Imprime un espacio en blanco para el espacio vacío
            if (estado->square[i][j] == 0) printf("x "); 
            else printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Función para clonar un estado
State* cloneState(State* src) {
    State* new_state = malloc(sizeof(State));
    if (new_state == NULL) {
        perror("Error al asignar memoria para clonar el estado");
        exit(EXIT_FAILURE);
    }
    memcpy(new_state->square, src->square, sizeof(src->square));
    new_state->x = src->x;
    new_state->y = src->y;
    new_state->actions = list_create();
    if (new_state->actions == NULL) {
        perror("Error al asignar memoria para las acciones del estado clonado");
        exit(EXIT_FAILURE);
    }
    void* node = list_first(src->actions);
    while (node) {
        list_pushBack(new_state->actions, node);
        node = list_next(src->actions);
    }
    return new_state;
}

// Función para verificar si un estado es final
int is_final_state(State* state) {
    int final[3][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state->square[i][j] != final[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// Función para generar nuevos estados a partir de un movimiento
State* transition(State* state, int action) {
    State* new_state = cloneState(state);
    int dx[4] = {-1, 1, 0, 0}; // movimientos de arriba, abajo, izquierda, derecha
    int dy[4] = {0, 0, -1, 1};
    int nx = state->x + dx[action];
    int ny = state->y + dy[action];
    if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
        // Swap the empty space with the adjacent number
        new_state->square[state->x][state->y] = new_state->square[nx][ny];
        new_state->square[nx][ny] = 0;
        new_state->x = nx;
        new_state->y = ny;
        list_pushBack(new_state->actions, (void*)(intptr_t)action); 
    } else {
        free(new_state->actions); // Liberar la lista de acciones
        free(new_state);
        return NULL;
    }
    return new_state;
}

// Función para generar todos los estados adyacentes a partir de un estado dado
List* get_adjacent_states(State* state) {
    List* adj_states = list_create();
    if (adj_states == NULL) {
        perror("Error al asignar memoria para la lista de estados adyacentes");
        exit(EXIT_FAILURE);
    }
    for (int action = 0; action < 4; action++) {
        State* new_state = transition(state, action);
        if (new_state) {
            list_pushBack(adj_states, new_state);
        }
    }
    return adj_states;
}

// Función para comparar dos estados
bool states_equal(const State* s1, const State* s2) {
    return memcmp(s1->square, s2->square, sizeof(s1->square)) == 0;
}

// Búsqueda en profundidad (DFS)
void dfs(State* initial_state, int max_depth) {
    for (int depth = 0; depth <= max_depth; depth++) {
        List* stack = list_create();
        list_pushBack(stack, initial_state);
        int iterations = 0;

        while (list_size(stack) > 0) {
            State* state = list_popBack(stack);
            iterations++;
            //el imprimir estados lo dejamos comentado ya que al tener muchas iteraciones muestra todas 
            //y eso hace que tarde el programa imprimiento para probar se puede usar la matriz pequeña 
            //y descomentar el imprimir 
            //imprimirEstado(state);
            if (is_final_state(state)) {
                printf("Solución encontrada en %d iteraciones\n", iterations);
                void* node = list_first(state->actions);
                while (node) {
                    printf("%d ", (int)(intptr_t)node);
                    node = list_next(state->actions);
                    //imprimirEstado(state);
                }
                printf("\n");
                imprimirEstado(state);
                list_clean(stack);
                free(stack);
                return;
            }
            if (list_size(state->actions) < depth) {
                List* adj_states = get_adjacent_states(state);
                void* node = list_first(adj_states);
                while (node) {
                    State* adj_state = (State*)node;
                    list_pushBack(stack, adj_state);
                    node = list_next(adj_states);
                }
                list_clean(adj_states);
                free(adj_states);
            }
        }
        list_clean(stack);
        free(stack);
    }
    printf("No se encontró solución hasta la profundidad %d\n", max_depth);
}

// Función de búsqueda en anchura (BFS)
void bfs(State* initial_state) {
    Queue* queue = queue_create();
    List* visited = list_create(); // Para almacenar los estados visitados
    queue_insert(queue, initial_state);
    list_pushBack(visited, initial_state);
    int iterations = 0;

    while (queue_size(queue) > 0) {
        State* state = (State*)queue_remove(queue);
        iterations++;

        // Imprimir el estado actual
        imprimirEstado(state);

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            void* node = list_first(state->actions);
            while (node) {
                printf("%d ", (int)(intptr_t)node);
                node = list_next(state->actions);
            }
            printf("\n");

            queue_clean(queue);
            list_clean(visited);
            free(queue);
            free(visited);
            return;
        }

        List* adj_states = get_adjacent_states(state);
        void* node = list_first(adj_states);
        while (node) {
            State* adj_state = (State*)node;

            // Verificar si el estado ya ha sido visitado
            bool already_visited = false;
            void* visited_node = list_first(visited);
            while (visited_node) {
                State* visited_state = (State*)visited_node;
                if (states_equal(visited_state, adj_state)) {
                    already_visited = true;
                    break;
                }
                visited_node = list_next(visited);
            }

            if (!already_visited) {
                queue_insert(queue, adj_state);
                list_pushBack(visited, adj_state);
            } else {
                free(adj_state);
            }

            node = list_next(adj_states);
        }

        list_clean(adj_states);
        free(adj_states);
        // free(state); // No se debe liberar aquí porque los estados en visited pueden ser referenciados nuevamente
    }

    printf("No se encontró solución\n");
    queue_clean(queue);
    list_clean(visited);
    free(queue);
    free(visited);
}

//comparar prioridad
int comparar_prioridad(const void* a, const void* b) {
    State* state_a = (State*)a;
    State* state_b = (State*)b;
    // Comparar por la distancia L1 negativa para priorizar la menor distancia
    return -(state_a->steps + distancia_L1(state_a)) + (state_b->steps + distancia_L1(state_b));
}

// Búsqueda mejor-primero
void best_first(State* initial_state) {
    Heap* heap = heap_create(comparar_prioridad); // Crear el heap con la función de comparación
    List* visited = list_create();
    initial_state->steps = 0;
    heap_push(heap, initial_state, -(initial_state->steps + distancia_L1(initial_state)));

    int iterations = 0;

    while (heap_top(heap)) {
        State* state = (State*)heap_top(heap);
        heap_pop(heap);
        iterations++;

        // Imprimir el estado actual del puzzle
        printf("Iteración %d - Estado actual del puzzle:\n", iterations);
        imprimirEstado(state);

        // Calcular y mostrar la distancia L1
        int dist = distancia_L1(state);
        printf("Distancia L1: %d\n", dist);

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            void* node = list_first(state->actions);
            while (node) {
                printf("%d ", (int)(intptr_t)node);
                node = list_next(state->actions);
            }
            printf("\n");

            list_clean(visited);
            free(visited);
            free(heap);
            return;
        }

        List* adj_states = get_adjacent_states(state);
        void* node = list_first(adj_states);
        while (node) {
            State* adj_state = (State*)node;

            // Verificar si el estado ya ha sido visitado
            bool already_visited = false;
            void* visited_node = list_first(visited);
            while (visited_node) {
                State* visited_state = (State*)visited_node;
                if (memcmp(visited_state->square, adj_state->square, sizeof(adj_state->square)) == 0) {
                    already_visited = true;
                    break;
                }
                visited_node = list_next(visited);
            }

            if (!already_visited) {
                adj_state->steps = state->steps + 1;
                heap_push(heap, adj_state, -(adj_state->steps + distancia_L1(adj_state)));
                list_pushBack(visited, adj_state);
            } else {
                list_clean(adj_state->actions); // Limpiar la lista de acciones
                free(adj_state); // Liberar memoria del estado adyacente
            }

            node = list_next(adj_states);
        }

        list_clean(adj_states); // Limpiar la lista de estados adyacentes
        free(adj_states);
        list_pushBack(state->actions, (void*)(intptr_t)-1); // Agregar un marcador para marcar el final de las acciones de este estado
    }

    printf("No se encontró solución\n");
    list_clean(visited); // Limpiar la lista de estados visitados
    free(visited);
    free(heap); // Liberar la memoria del heap
}

int main() {
    // Estado inicial del puzzle
    //matriz de prueba
    /*State estado_inicial = {
        {{3, 1, 2}, // Primera fila (0 representa espacio vacío)
         {0, 4, 5}, // Segunda fila
         {6, 7, 8}, // Tercera fila
         },  
        1,0    // Posición del espacio vacío (fila 0, columna 1)
    };*/
    
    //matriz del profe 
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
        {1, 3, 4}, // Segunda fila
        {6, 5, 7}, // Tercera fila
        },  
        0, 0 
    };
    
    estado_inicial.actions = list_create();
    
    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    int opcion;
    List* visited = list_create(); // Lista para almacenar los estados visitados
    do {
        printf("\n***** MENÚ ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Búsqueda en Anchura");
        puts("3) Búsqueda Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf("%d", &opcion);

        State* initial_state = cloneState(&estado_inicial);

        switch (opcion) {
        case 1:
            printf("Ejecutando Búsqueda en Profundidad\n");
            dfs(initial_state,10);
            break;
        case 2:
            printf("Ejecutando Búsqueda en Anchura\n");
            bfs(initial_state);
            break;
        case 3:
            printf("Ejecutando Búsqueda Mejor Primero\n");
            best_first(initial_state);
            break;
        case 4:
            printf("Saliendo...\n");
            break;
        default:
            printf("Opción no válida\n");
        }

    } while (opcion != 4);

    return 0;
}
