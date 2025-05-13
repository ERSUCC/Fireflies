#pragma once

#include <cstdlib>

typedef struct list_node
{
    int ax;
    int ay;
    unsigned int point_count;
    unsigned int frame;
    unsigned int id;
    struct list_node* prev;
    struct list_node* next;
} list_node;

list_node* list_node_allocate(const int x, const int y, const unsigned int frame, const unsigned int id);
void list_node_free(list_node* node);

typedef struct list
{
    list_node* head;
    list_node* tail;
    unsigned int count;
    unsigned int next_id;
} list;

list* list_allocate(void);
void list_add(list* l, const int x, const int y, const unsigned int frame);
void list_remove(list* l, list_node* node);
void list_free(list* l);
