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
} State;

// Función para calcular la distancia L1
int distancia_L1(State* state) {
    int ev = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int val = state->square[i][j];
            if (val == 0) continue;
            int target_i = (val - 1) / 3;
            int target_j = (val - 1) % 3;
            ev += abs(target_i - i) + abs(target_j - j);
        }
    }
    return ev;
}

// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Función para clonar un estado
State* cloneState(State* src) {
    State* new_state = malloc(sizeof(State));
    memcpy(new_state->square, src->square, sizeof(src->square));
    new_state->x = src->x;
    new_state->y = src->y;
    new_state->actions = list_create();
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
        list_pushBack(new_state->actions, (void*)(intptr_t)action); // Record the action
    } else {
        free(new_state);
        return NULL;
    }
    return new_state;
}

// Función para generar todos los estados adyacentes a partir de un estado dado
List* get_adjacent_states(State* state) {
    List* adj_states = list_create();
    for (int action = 0; action < 4; action++) {
        State* new_state = transition(state, action);
        if (new_state) {
            list_pushBack(adj_states, new_state);
        }
    }
    return adj_states;
}

// Búsqueda en profundidad (DFS)
//bueno2
void dfs(State* initial_state) {
    int max_depth = 0;
    while (1) {
        List* stack = list_create();
        list_pushBack(stack, initial_state);
        List* visited = list_create();
        int iterations = 0;

        while (list_size(stack) > 0) {
            State* state = list_popBack(stack);
            iterations++;
            if (is_final_state(state)) {
                printf("Solución encontrada en %d iteraciones\n", iterations);
                imprimirEstado(state);
                void* node = list_first(state->actions);
                while (node) {
                    printf("%d ", (int)(intptr_t)node);
                    node = list_next(state->actions);
                }
                printf("\n");
                list_clean(stack);
                list_clean(visited);
                free(stack);
                free(visited);
                return;
            }
            // Añadir el estado actual a la lista de visitados
            list_pushBack(visited, state);
            if (list_size(state->actions) < max_depth) {
                List* adj_states = get_adjacent_states(state);
                void* node = list_first(adj_states);
                while (node) {
                    State* adj_state = (State*)node;
                    // Verificar si el estado ya ha sido visitado
                    int already_visited = 0;
                    void* visited_node = list_first(visited);
                    while (visited_node) {
                        State* visited_state = (State*)visited_node;
                        if (memcmp(visited_state->square, adj_state->square, sizeof(adj_state->square)) == 0) {
                            already_visited = 1;
                            break;
                        }
                        visited_node = list_next(visited);
                    }
                    if (!already_visited) {
                        list_pushBack(stack, adj_state);
                    } else {
                        free(adj_state);
                    }
                    node = list_next(adj_states);
                }
                list_clean(adj_states);
                free(adj_states);
            }
        }

        // Incrementar la profundidad máxima para la siguiente iteración
        max_depth++;
        list_clean(visited);
        free(visited);
    }
    printf("No se encontró solución\n");
}
// Búsqueda en anchura (BFS)
// BFS HECHO POR MARCEL(CAUSA ERROR)
/*void bfs(State* initial_state) {
    Queue* queue = queue_create();
    queue_insert(queue, initial_state);
    int iterations = 0;

    while (queue_size(queue) > 0) {
        State* state = (State*)queue_remove(queue);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
            void* node = list_first(state->actions);
            while (node) {
                printf("%d ", (int)(intptr_t)node);
                node = list_next(state->actions);
            }
            printf("\n");

            queue_clean(queue);
            free(queue);
            free(state);
            return;
        }

        List* adj_states = get_adjacent_states(state);
        void* node = list_first(adj_states);
        while (node) {
            State* adj_state = (State*)node;

            // Verificar si el estado ya ha sido visitado
            int already_visited = 0;
            void* queue_node = queue_first(queue);
            while (queue_node) {
                State* visited_state = (State*)queue_node;
                if (memcmp(visited_state->square, adj_state->square, sizeof(adj_state->square)) == 0) {
                    already_visited = 1;
                    break;
                }
                queue_node = queue_next(queue_node);
            }

            if (!already_visited) {
                queue_insert(queue, adj_state);
            } else {
                free(adj_state);
            }

            node = list_next(node);
        }

        list_clean(adj_states);
        free(adj_states);
    }

    printf("No se encontró solución\n");
    queue_clean(queue);
    free(queue);
}*/

//nuevo
// Función para crear un hash del estado (para almacenamiento en conjunto)
size_t hash_state(const State* state) {
    size_t hash = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            hash = hash * 31 + state->square[i][j];
        }
    }
    return hash;
}
// Función para comparar dos estados
bool states_equal(const State* s1, const State* s2) {
    return memcmp(s1->square, s2->square, sizeof(s1->square)) == 0;
}
void bfs(State* initial_state) {
    Queue* queue = queue_create();
    List* visited = list_create(); // Para almacenar los estados visitados
    queue_insert(queue, initial_state);
    list_pushBack(visited, initial_state);
    int iterations = 0;

    while (queue_size(queue) > 0) {
        State* state = (State*)queue_remove(queue);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
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
        free(state); // Liberar el estado actual
    }

    printf("No se encontró solución\n");
    queue_clean(queue);
    list_clean(visited);
    free(queue);
    free(visited);
}


// Búsqueda mejor-primero
/*void best_first(State* initial_state) {}*/
// Búsqueda mejor-primero
/*
HECHA POR MARCEL(TAMBIEN DA ERROR EN COMPILAR)
void best_first(State* initial_state) {
    Heap *openHeap = heap_create();

    heap_push(openHeap, initial_state, distancia_L1(initial_state));
    int iteraciones = 0;
    while(!heap_empty(openHeap)){
        State* state = (State*)heap_top(openHeap);
        heap_pop(openHeap);
        iteraciones++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iteraciones);
            printf("Distancia L1:%d\n", distancia_L1(state));
            imprimirEstado(state);
            // Liberamos la memoria del estado y la lista de acciones
            list_destroy(state->actions);
            free(state);
            // Liberamos la memoria del heap
            heap_clean(openHeap);
            return;
        }
        List* adj_states = get_adjacent_states(state);
        Node* node = adj_states->head;
        while(node != NULL){
            State* adj_state = (State*)node->data;
            heap_push(openHeap, adj_state, distancia_L1(adj_state));
            node = node->next;
        }

        list_clean(adj_states);
    }
    printf("No se encontró solución\n");
    heap_clean(openHeap);

}
*/
void best_first(State* initial_state) {
    Heap* heap = heap_create();
    List* visited = list_create();
    heap_push(heap, initial_state, distancia_L1(initial_state));
    int iterations = 0;

    while (heap_top(heap)) {
        State* state = (State*)heap_top(heap);
        heap_pop(heap);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
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
                heap_push(heap, adj_state, distancia_L1(adj_state));
                list_pushBack(visited, adj_state);
            } else {
                free(adj_state);
            }

            node = list_next(adj_states);
        }

        list_clean(adj_states);
        free(adj_states);
        free(state);
    }

    printf("No se encontró solución\n");
    list_clean(visited);
    free(visited);
    free(heap);
}


int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{3, 1, 2}, // Primera fila (0 representa espacio vacío)
         {0, 4, 5}, // Segunda fila
         {6, 7, 8}, // Tercera fila
         },  
        1,0    // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    int opcion;
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
            dfs(initial_state);
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
