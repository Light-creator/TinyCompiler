
#ifndef _TREE_H
#define _TREE_H

#include "../token/token.h"

typedef struct _node {
    struct _node *right;
    struct _node *left;
    Token value;
} Node;

typedef struct _tree {
    Node *head;
    Node *curr_ptr;
} Tree;

Tree create_tree();
Node *create_node(Token value);

void tree_insert(Tree *tree, Token value);
void print_tree(Tree *tree);
void print_node(Node *node);

#endif