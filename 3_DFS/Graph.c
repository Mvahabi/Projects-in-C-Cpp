/*
Mahyar Mike Vahabi
mvahabi
Winter 22 - CSE101 - pa3
Graph.c - Graph ADT
*/

// Citation: Professor Tantalo's PsudoCode, notes, and queue.c implementation

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

// private Graph object type 
typedef struct GraphObj {
    List* list;
    int order;   // stores #vertices = n
    int size;    // # of edges
    char* color; // array of color of vertex "i"
    int* parent; // array of parent of vertex "i"
    int* disc;   // arra
    int* finish; // 
}GraphObj;

// constructor for graph
Graph newGraph(int n) {
    if (n<0) {
        fprintf(stderr, "Error in newGraph(). Can't graph 0 or negative vertices.\n");
        exit(EXIT_FAILURE);
    }
    Graph G = malloc(sizeof(GraphObj));
    if (G) {
        G->order = n;
        G->size = 0;
        G->list = malloc(sizeof(List) * n+1);
        G->color = malloc(sizeof(char) * n+1);
        G->parent = malloc(sizeof(int) * n+1);
        G->disc = malloc(sizeof(int) * n+1);
        G->finish = malloc(sizeof(int) * n+1);
        for (int i = 1; i < n+1; i++) {
            G->list[i] = newList();
            G->color[i] = 'w';
            G->parent[i] = NIL;
            G->disc[i] = UNDEF;
            G->finish[i] = UNDEF;
        }
    }
    return G;
}

// destructor for graph
void freeGraph(Graph* pG) {
    if (!pG){
        fprintf(stderr, "Error in FreeGraph(). Can not free an empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (pG && *pG) {
        for (int i = 1; i < (*pG)->order + 1; i++) {
            freeList(&(*pG)->list[i]);
        }
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->disc);
        free((*pG)->finish);
        free(*pG);
        *pG = NULL;
    }
}

// returns number of vertices
int getOrder(Graph G){
    if (!G) { 
        fprintf(stderr, "Error in getOrder(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (G->order < 1 ) {
        fprintf(stderr, "Error in getOrder(). Num of vertices can not be 0 or negative.\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// returns number of edges
int getSize(Graph G){
    if (!G) {
        fprintf(stderr, "Error in getSize(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// return the parent of vertex u in BFS() or nul if BFS() wasn't called
int getParent(Graph G, int u){
    if (!G) {
        fprintf(stderr, "Error in getParent(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Error in getParent(). U is out of range.\n");
        exit(EXIT_FAILURE);
    } else {
        return G->parent[u];
    }
}

// returns the distance from the most recent BFS source to vertex u, or INF if BFS()
int getDiscover(Graph G, int u){
    if (!G) {
        fprintf(stderr, "Error in getDiscover(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Error in getDiscover(). U is out of range.\n");
        exit(EXIT_FAILURE);
    } else {
        return G->disc[u];
    }
}

// appends to L the value NIL if no such path exists
int getFinish(Graph G, int u) {
    if (!G) {
        fprintf(stderr, "Error in getFinish(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Error in getFinish(). U is out of range.\n");
        exit(EXIT_FAILURE);
    }
    return G->finish[u];
}

//inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, 
//and v to the adjacency List of u. 
void addEdge(Graph G, int u, int v) {
    if (!G) {
        fprintf(stderr, "Error in addEdge(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (u < 0 || v < 0) {
        fprintf(stderr, "Error in addEdge(). Invalid u or v.\n");
        exit(EXIT_FAILURE);
    }

    // connect u, v
    moveFront(G->list[u]);
    bool bool_flag = true;
    if (length(G->list[u]) == 0) {
        append(G->list[u], v);
        bool_flag = false;
    }
    while (bool_flag && index(G->list[u]) >= 0 && index(G->list[u]) < length(G->list[u])) {
        if (get(G->list[u]) >= v) {
            insertBefore(G->list[u], v);
            bool_flag = false;
        }
        else if (index(G->list[u]) == (length(G->list[u]) - 1)) {
            append(G->list[u], v);
            bool_flag = false;
        } else {
            moveNext(G->list[u]);
        }
    }

    // connect v, u
    moveFront(G->list[v]);
    bool_flag = true;
    if (length(G->list[v]) == 0) {
            append(G->list[v], u);
            bool_flag = false;
    }
    while (bool_flag && index(G->list[v]) >= 0 && index(G->list[v]) < length(G->list[v])) {
        if (get(G->list[v]) >= u) {
            insertBefore(G->list[v], u);
            bool_flag = false;
        }
        else if (index(G->list[v]) == (length(G->list[v]) - 1)) {
            append(G->list[v], u);
            bool_flag = false;
        } else {
            moveNext(G->list[v]);
        }
    }
    G->size++;
}

// adds a directed edge
void addArc(Graph G, int u, int v) {
    if (!G) {
        fprintf(stderr, "Error in addArc(). Empty graph.\n");
        exit(EXIT_FAILURE);
    }
    if (u < 0 || v < 0) {
        fprintf(stderr, "Error in addArc(). Invalid u or v.\n");
        exit(EXIT_FAILURE);
    }

    // connect u, v but not other way around
    moveFront(G->list[u]);
    bool bool_flag = true;
    if (length(G->list[u]) == 0) {
        append(G->list[u], v);
        bool_flag = false;
    }
    while (bool_flag && index(G->list[u]) >= 0 && index(G->list[u]) < length(G->list[u])) {
        if (get(G->list[u]) >= v) {
            insertBefore(G->list[u], v);
            bool_flag = false;
        }
        else if (index(G->list[u]) == (length(G->list[u]) - 1)) {
            append(G->list[u], v);
            bool_flag = false;
            } else {
            moveNext(G->list[u]);
        }
    }
    G->size++;
}

// private function used in BFS()
int Visit(Graph G, int x, int time, List S){
    if(!G){
        fprintf(stderr, "Error in Visit(). Empty graph or stack.\n");
        exit(EXIT_FAILURE);
    }
    time++;
    G->disc[x] = time;
    G->color[x] = 'g'; 
    moveFront(G->list[x]);
    for (int iter = 1; iter <= length(G->list[x]); iter++){
        int y = get(G->list[x]);
        if(G->color[y] == 'w'){
            G->parent[y] = x;
            time = Visit(G, y, time, S);
        }
        moveNext(G->list[x]);
    }
    G->color[x] = 'b';
    time++;
    G->finish[x] = time;
    //fprintf(stdout, "THIS IS PRINTING!!!\n");
    prepend(S, x);
    return time;
}

// perform Depdth First Search
void DFS(Graph G, List S) {
    if(!G||!S){
        fprintf(stderr, "Error in DFS(). Empty graph or stack.\n");
        return;
    }
    if (length(S) != getOrder(G)) {
        fprintf(stderr, "Error in DFS(). Sizes do not match.\n");
        exit(EXIT_FAILURE);
    }
    for(int x = 1; x < G->order + 1; x++) {
        G->color[x] = 'w';
        G->parent[x] = NIL;
    }
    int time = 0;
    clear(S);
    for (int y = 1; y < G-> order + 1; y++){
        if (G->color[y] == 'w')  {
            //clear(S);
//fprintf(stdout, "\n\n--------------------------\n\nDFS IS HAPPENING\n\n\n----------------------------\n\n");
            time = Visit(G, y, time, S);
        }
    }
}

// changes the direction of the vertices
Graph transpose(Graph G){
    if(!G){
        fprintf(stderr, "Error in transpose(). Empty graph.\n");
        return NULL;
    }
    Graph Trans = newGraph(G->order);
    for(int i = 1; i < G->order + 1; i++){
        moveFront(G->list[i]);
        while(index(G->list[i]) != -1){
            int y = get(G->list[i]);
            addArc(Trans, y, i);
            moveNext(G->list[i]);
        }
    }
    return Trans;
}

// copies graph 
Graph copyGraph(Graph G){
    if(!G){
        fprintf(stderr, "Error in copyGraph(). Empty graph.\n");
        return NULL;
    }
    Graph copy = newGraph(G->order);
    for(int i = 1; i < G->order+1; i++){
        moveFront(G->list[i]);
        while(get(G->list[i]) != -1){
            addArc(copy, i, get(G->list[i]));
        }
    }
    return copy;
}

//prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G){
    if (!G) {
        fprintf(stderr, "Error in pringGraph(). empty graph.\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i <= G->order; i++){
        moveFront(G->list[i]);
        fprintf(out, "%d: ",i );
        while(index(G->list[i]) >  -1){
            fprintf(out, "%d ", get(G->list[i]));
            moveNext(G->list[i]);
        }
        fprintf(out, "\n");
    }
}

