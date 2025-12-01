#ifndef TREE_H
#define TREE_H

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"

#include "log.h"
#include "node.h"
#include <stdlib.h>

typedef enum { UP, HEAD, LEFT, RIGHT } Direction;

typedef struct {
    Node *head;
    unsigned depth;
} RedBlackTree;

RedBlackTree *RedBlackTree_init();
void RedBlackTree_destroy(RedBlackTree *tree);

Node *RedBlackTree_insert(RedBlackTree *tree, int value);
void RedBlackTree_remove(RedBlackTree *tree, int value);
void RedBlackTree_display(RedBlackTree *tree);

#endif
