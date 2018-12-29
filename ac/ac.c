#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ac.h"

#define POINT_NUM 26

typedef struct node
{
    struct node *fail;
    struct node *next[POINT_NUM];
    int ifend;
    int patten_len;
    char *key;
}NODE;

void *ac_machine_init()
{
    int i;
    NODE *root = (NODE *)malloc(sizeof(*root));
    if(root == NULL)
        return NULL;
    root->fail = NULL;
    for(i = 0; i < POINT_NUM; i ++)
        root->next[i] = NULL;
    root->ifend = 0;
    root->patten_len = 0;
    root->key = NULL;
    return root;
}

static NODE* new_node(NODE* root)
{
    int i;
    NODE *node = (NODE *)malloc(sizeof(*node));
    if(node == NULL)
        return NULL;
    node->fail = root;
    for(i = 0; i < POINT_NUM; i ++)
        node->next[i] = NULL;
    node->ifend = 0;
    node->key = NULL;
    return node;
}

int ac_machine_insert(void *handle, char *patten)
{
    int len = strlen(patten),i;
    NODE *root = (NODE *)handle;
    NODE *cur = root;
    for(i = 0; i < len; i ++)
    {
        if(cur->next[patten[i] - 'a'] == NULL)
        {
            cur->next[patten[i] - 'a'] = new_node(root);
        }
        cur = cur->next[patten[i] - 'a'];
    }
    cur->ifend = 1;
    cur->patten_len = strlen(patten);
    cur->key = patten;

    return 0;
}

static void build_node_child_fail(NODE *node)
{
    int i;
    NODE *child;
    for(i = 0; i < POINT_NUM; i ++)
    {
        if(node->next[i] != NULL)
        {
            child = node->next[i];
            if (node->fail != NULL && node->fail->next[i] != NULL)
            {
                child->fail = node->fail->next[i];
            }

            build_node_child_fail(child);
        }
    } 
}

void ac_machine_compile(void *handle)
{
    NODE *root = (NODE *)handle;
    build_node_child_fail(root);
}

static void search_point(NODE *root, NODE **tmp, int point, char *text, uint64_t offset , char **result_pattens, int *patten_lens, int max_size, int *result_num)
{
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
        if ((*tmp)->fail != root && (*tmp)->fail->ifend == 1)
        {
            result_pattens[*result_num] = text + offset + 1 - (*tmp)->fail->patten_len;
            patten_lens[*result_num] = (*tmp)->fail->patten_len;
            *result_num += 1;
            if (*result_num >= max_size)
                return;
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

int ac_machine_search(void *handle, char *text, uint64_t len, char **result_pattens, int *patten_lens, int max_size)
{
    NODE *root = (NODE *)handle;
    NODE *tmp = root;
    int result_num = 0;
    uint64_t i;
    int point;
    for(i = 0; i < len; i ++)
    {
        point = text[i] - 'a';
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
    NODE *root = (NODE *)handle;
    for(i = 0; i < POINT_NUM; i ++)
    {
        if(root->next[i] != NULL)
            destroy_node(root->next[i]);
    }
    free(root);
    root = NULL;
}
