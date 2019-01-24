#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint8_t *good;
    uint8_t (*bad)[256];
    uint8_t *patten;
    uint8_t patten_len;
}bm_handle;

static void bm_free(bm_handle *handle)
{
    if (handle)
    {
        if (handle->good)
        {
            free(handle->good);
            handle->good = NULL;
        }
        if (handle->bad)
        {
            free(handle->bad);
            handle->bad = NULL;
        }
        free(handle);
    }
}

static uint8_t search_prev(uint8_t *patten, uint8_t patten_len, uint8_t suffix_len) //返回模式串中尾缀出现的
{
    uint8_t i;
    uint8_t index1, index2, times;
    uint8_t same_len;

    for(i = patten_len-1; i >= suffix_len; i -- )   //i==需要对比前缀长度
    {
        same_len = 0;
        for(index1 = i-1, index2 = patten_len-1, times = 0; times < suffix_len; index1 --, index2 --)
        {
            if (patten[index1] == patten[index2])
            {
                same_len ++;
            }
            else
            {
                break;
            }
        }
        if (same_len == suffix_len)    //说明存在与尾缀相同的子串
        {
            return (index2-index1);
        }
    }
    return 0;

}

static uint8_t get_good_offset(uint8_t *patten, uint8_t patten_len, uint8_t suffix_len)
{
    uint8_t len;    //尾缀长度
    uint8_t offset;

    for (len = suffix_len; len > 0; len --) //先查找最长尾缀，后查找尾缀的子缀
    {
        offset = search_prev(patten, patten_len, len);
        if (offset)
        {
            return offset;
        }
    }
    //len == 0
    return patten_len;
}


static void mk_good_tb(bm_handle *handle)
{
    uint8_t i;
    handle->good[0] = 1;
    for (i = 1; i < handle->patten_len; i ++)
    {
/*----------------------------------------------
        handle->good[i] = get_good_offset(handle->patten, handle->patten_len, i);
----------------------------------------------*/
        handle->good[i] = search_prev(handle->patten, handle->patten_len, i);
        if (0 == handle->good[i])
        {
            handle->good[i] = i == 1?handle->patten_len:handle->good[i-1];
        }
//----------------------------------------------
    }
}

static uint8_t get_bad_offset(uint8_t *patten, uint8_t patten_len, uint8_t index, uint8_t bad)
{
    uint8_t i;
    uint8_t offset = patten_len - 1 - index;
    
    if (0 == offset)
    {
        return 1;
    }

    for (i = offset-1; i >= 0; i -- )
    {
        if (patten[i] == bad)
        {
            return (offset - i);
        }
    }
    return (offset + 1);

}

static void mk_bad_tb(bm_handle *handle)
{
    int i,j;
    uint8_t index;

    for(i = 0; i < handle->patten_len; i ++)
    {
        for (j = 0; j < 256; j ++)
        {
            handle->bad[i][j] = handle->patten_len - i; //按全不存在初始化
        }
    }

    for(index = 0; index < handle->patten_len; index ++)
    {
        for (i = 0; i < handle->patten_len; i ++)
        {
            handle->bad[i][handle->patten[handle->patten_len-1-index]] = get_bad_offset(handle->patten, handle->patten_len, i, handle->patten[handle->patten_len-1-index]);
        }
    }
}


void *bm_compile(uint8_t *patten, uint8_t len)
{
    bm_handle *handle = (bm_handle *)malloc(sizeof(bm_handle));
    if (NULL == handle)
    {
        return handle;
    }
    handle->patten = patten;
    handle->patten_len = len;
    handle->good = (uint8_t *)malloc(sizeof(uint8_t) * len);
    handle->bad = (uint8_t (*)[256])malloc(sizeof(uint8_t) * 256 * len);
    if (NULL == handle->good || NULL == handle->bad)
    {
        bm_free(handle);
        return NULL;
    }

    mk_good_tb(handle);
    mk_bad_tb(handle);
    
    return handle;
}


uint8_t *bm_search(void *tmp, uint8_t *text, int len)
{
    bm_handle *handle = (bm_handle *)tmp;
    uint8_t patten_len = handle->patten_len;

    int offset = 0;
    uint8_t i;
    uint8_t offset_good, offset_bad;
    uint8_t same_len;

    while(offset + patten_len <= len)
    {
        same_len = 0;
        for(i = 0; i < patten_len; i ++)
        {
            if (text[offset+patten_len-1-i] == handle->patten[patten_len-1-i])
            {
                same_len ++;
            }
            else
            {
                break;
            }
        }
        if (same_len == patten_len)
        {
            printf("match at %d!\n",offset);
            return (text + offset);
        }
        else
        {
            offset_good = i >= 1? handle->good[i] : 1;
            offset_bad = handle->bad[i][text[offset+patten_len-1-i]];
            printf("good == %d, bad == %d\n", offset_good, offset_bad);
            offset += offset_good > offset_bad ? offset_good : offset_bad;
        }
    }
    return NULL;
}

void bm_release(void *tmp)
{
    bm_handle *handle = (bm_handle *)tmp;
    bm_free(handle);
}
