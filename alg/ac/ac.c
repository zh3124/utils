#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ac.h"

#define POINT_NUM 256

typedef struct node
{
    struct node *fail;
    struct node *next[POINT_NUM];
    uint8_t *patten;
    uint8_t patten_len;
    uint8_t ifend;
}NODE;

typedef struct {
    int max_depth;
    NODE *root;
}TREE;

void *ac_machine_init()
{
    int i;
    TREE *tree = NULL;
    NODE *root = NULL;

    root = (NODE *)malloc(sizeof(*root));
    if(NULL == root)
        goto init_err;

    root->fail = NULL;
    for(i = 0; i < POINT_NUM; i ++)
    {
        root->next[i] = NULL;
    }
    root->patten = NULL;
    root->patten_len = 0;
    root->ifend = 0;

    tree = (TREE *)malloc(sizeof(*tree));
    if (NULL == tree)
        goto init_err;
    
    tree->root = root;
    tree->max_depth = 0;

    return tree;

init_err:
    if (tree)
        free(tree);
    if (root)
        free(root);
    return NULL;
}

static NODE* new_node(NODE* root)
{
    int i;
    NODE *node = (NODE *)malloc(sizeof(*node));
    if(node == NULL)
    {
        return NULL;
    }
    node->fail = root;
    for(i = 0; i < POINT_NUM; i ++)
    {
        node->next[i] = NULL;
    }
    node->patten = NULL;
    node->patten_len = 0;
    node->ifend = 0;

    return node;
}

int ac_machine_insert(void *handle, uint8_t *patten, uint8_t len)
{
    int i;
    TREE *tree = (TREE *)handle;
    NODE *root = tree->root;
    NODE *cur = root;
    for(i = 0; i < len; i ++)
    {
        if(cur->next[patten[i]] == NULL)
        {
            cur->next[patten[i]] = new_node(root);
        }
        cur = cur->next[patten[i]];
    }
    cur->patten = patten;
    cur->patten_len = len;
    cur->ifend = 1;
    if (tree->max_depth < len)
    {
        tree->max_depth = len;
    }

    return 0;
}

static void build_node_child_fail(NODE *node)
{
    int i;
    NODE *child, *fail;
    if (node->fail == NULL)
    {
        return;
    }
    for (i = 0; i < POINT_NUM; i ++)
    {
        if (NULL != node->next[i])
        {
            child = node->next[i];
            fail = node->fail;
            while (NULL != fail)
            {
                if (NULL != fail->next[i])
                {
                    child->fail = fail->next[i];
                    break;
                }
                fail = fail->fail;
            }
        }
    }
}

static void traversal_node_by_level(NODE *node, int level)
{
    int i;

    if (0 == level)
    {
        //do something and return
        build_node_child_fail(node);
        return;
    }

    for(i = 0; i < POINT_NUM; i ++)
    {
        if (node->next[i] != NULL)
        {
            traversal_node_by_level(node->next[i], level - 1);
        }
    }
}

void ac_machine_compile(void *handle)
{
    TREE *tree = (TREE *)handle;
    NODE *root = tree->root;
    int i;
    for (i = 0; i <= tree->max_depth; i ++)
    {
        traversal_node_by_level(root, i);
    }
}

static void search_point(NODE *root, NODE **tmp, uint8_t point, uint8_t *text, uint64_t offset , uint8_t **result_pattens, uint8_t *patten_lens, int max_size, int *result_num)
{
    NODE *fail;
    if (*result_num >= max_size)
    {
        return;
    }

    if((*tmp)->next[point] != NULL)
    {
        (*tmp) = (*tmp)->next[point];
        if ((*tmp)->ifend == 1)
        {
            result_pattens[*result_num] = text + offset + 1 - (*tmp)->patten_len;
            patten_lens[*result_num] = (*tmp)->patten_len;
            *result_num += 1;
            if (*result_num >= max_size)
                return;
        }
        fail = (*tmp)->fail;
        while (root != fail)
        {
            if (fail->ifend == 1)
            {
                result_pattens[*result_num] = text + offset + 1 - fail->patten_len;
                patten_lens[*result_num] = fail->patten_len;
                *result_num += 1;
                if (*result_num >= max_size)
                    return;
            }
            fail = fail->fail;
        }
    }
    else
    {
        if ((*tmp)->fail != NULL)
        {
            (*tmp) = (*tmp)->fail;
            search_point(root, tmp, point, text, offset, result_pattens, patten_lens, max_size, result_num);
        }
    }
}

int ac_machine_search(void *handle, uint8_t *text, uint64_t len, uint8_t **result_pattens, uint8_t *patten_lens, int max_size)
{
    TREE *tree = (TREE *)handle;
    NODE *root = tree->root;
    NODE *tmp = root;
    int result_num = 0;
    uint64_t i;
    uint8_t point;
    for(i = 0; i < len; i ++)
    {
        point = text[i];
        if (result_num >= max_size)
        {
            return result_num;
        }
        search_point(root, &tmp, point, text, i, result_pattens, patten_lens, max_size, &result_num );
    }
    return result_num;
}

static void destroy_node(NODE *node)
{
    int i;
    for(i = 0; i < POINT_NUM; i ++)
    {
        if(node->next[i] != NULL)
            destroy_node(node->next[i]);
    }
    free(node);
    node = NULL;
}

void ac_machine_release(void *handle)
{
    int i;
    TREE *tree = (TREE *)handle;
    NODE *root = tree->root;
    for(i = 0; i < POINT_NUM; i ++)
    {
        if(root->next[i] != NULL)
            destroy_node(root->next[i]);
    }
    free(root);
    free(tree);
    root = NULL;
    tree = NULL;
}
