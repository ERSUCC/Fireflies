#include "list.h"

list_node* list_node_allocate(const int x, const int y, const unsigned int frame, const unsigned int id)
{
    list_node* node = (list_node*)malloc(sizeof(list_node));

    node->ax = x;
    node->ay = y;
    node->point_count = 1;
    node->frame = frame;
    node->id = id;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void list_node_free(list_node* node)
{
    free(node);
}

list* list_allocate(void)
{
    list* l = (list*)malloc(sizeof(list));

    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
    l->next_id = 0;

    return l;
}

void list_add(list* l, const int x, const int y, const unsigned int frame)
{
    if (l->tail)
    {
        l->tail->next = list_node_allocate(x, y, frame, l->next_id);
        l->tail->next->prev = l->tail;
        l->tail = l->tail->next;
    }

    else
    {
        l->head = list_node_allocate(x, y, frame, l->next_id);
        l->tail = l->head;
    }

    l->count++;
    l->next_id++;
}

void list_remove(list* l, list_node* node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;

    list_node_free(node);

    l->count--;
}

void list_free(list* l)
{
    list_node* current = l->head;

    while (current)
    {
        list_node* next = current->next;

        list_node_free(current);

        current = next;
    }

    free(l);
}
