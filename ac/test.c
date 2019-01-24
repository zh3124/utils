#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>

#include "ac.h"

void *handle;

TEST(test_init, test)
{
    EXPECT_TRUE(handle == NULL);
    handle = ac_machine_init();
    EXPECT_TRUE(handle != NULL);
}

TEST(test_insert, test)
{
    ac_machine_insert(handle, (uint8_t *)"abd", 3);
    printf("patten:%s\n", "abd");
    ac_machine_insert(handle, (uint8_t *)"abdk", 4);
    printf("patten:%s\n", "abdk");
    ac_machine_insert(handle, (uint8_t *)"abchijn", 7);
    printf("patten:%s\n", "abchijn");
    ac_machine_insert(handle, (uint8_t *)"chnit", 5);
    printf("patten:%s\n", "chnit");
    ac_machine_insert(handle, (uint8_t *)"ijabdf", 6);
    printf("patten:%s\n", "ijabdf");
    ac_machine_insert(handle, (uint8_t *)"ijaij", 5);
    printf("patten:%s\n", "ijaij");
}

TEST(test_compile, test)
{
    ac_machine_compile(handle);
}

uint8_t *text = (uint8_t *)"abchnijabdfk";
TEST(test_search, test)
{
    printf("text:%s\n", "abchnijabdfk");
    int max_num = 10;
    int i;
    uint8_t **result_pattens = (uint8_t **)malloc(sizeof(uint8_t *) * max_num);
    uint8_t *patten_lens = (uint8_t *)malloc(sizeof(uint8_t) * max_num);
    int ret = ac_machine_search(handle, text, 12, result_pattens, patten_lens, max_num);
    for (i = 0; i < ret; i ++)
    {
        printf("hit:%.*s,offset:%d\n", patten_lens[i], result_pattens[i], result_pattens[i]-text);
    }
    EXPECT_TRUE(ret == 2);
    EXPECT_TRUE(patten_lens[0] == 3);
    EXPECT_TRUE(patten_lens[1] == 6);
    EXPECT_TRUE(memcmp("abd", result_pattens[0], 3) == 0);
    EXPECT_TRUE(memcmp("ijabdf", result_pattens[1], 6) == 0);
}

TEST(test_release, test)
{
    ac_machine_release(handle);
}

TEST(test_init2, test)
{
    handle = ac_machine_init();
    EXPECT_TRUE(handle != NULL);
}

TEST(test_insert2, test)
{
    ac_machine_insert(handle, (uint8_t *)"abcd", 4);
    printf("patten:%s\n", "abcd");
    ac_machine_insert(handle, (uint8_t *)"cdefg", 5);
    printf("patten:%s\n", "cdefg");
    ac_machine_insert(handle, (uint8_t *)"d", 1);
    printf("patten:%s\n", "d");
    ac_machine_insert(handle, (uint8_t *)"ef", 2);
    printf("patten:%s\n", "ef");
    ac_machine_insert(handle, (uint8_t *)"fg", 2);
    printf("patten:%s\n", "fg");
    
}

TEST(test_compile2, test)
{
    ac_machine_compile(handle);
}

uint8_t *text2 = (uint8_t *)"abcdefg";
TEST(test_search2, test)
{
    printf("text:%s\n", "abcdefg");
    int max_num = 10;
    int i;
    uint8_t **result_pattens = (uint8_t **)malloc(sizeof(uint8_t *) * max_num);
    uint8_t *patten_lens = (uint8_t *)malloc(sizeof(uint8_t) * max_num);
    int ret = ac_machine_search(handle, text2, 7, result_pattens, patten_lens, max_num);
    for (i = 0; i < ret; i ++)
    {
        printf("hit:%.*s,offset:%d\n", patten_lens[i], result_pattens[i], result_pattens[i]-text2);
    }
    EXPECT_TRUE(ret == 5);
    EXPECT_TRUE(patten_lens[0] == 4);
    EXPECT_TRUE(patten_lens[1] == 1);
    EXPECT_TRUE(patten_lens[2] == 2);
    EXPECT_TRUE(patten_lens[3] == 5);
    EXPECT_TRUE(patten_lens[4] == 2);
    EXPECT_TRUE(memcmp("abcd", result_pattens[0], 4) == 0);
    EXPECT_TRUE(memcmp("d", result_pattens[1], 1) == 0);
    EXPECT_TRUE(memcmp("ef", result_pattens[2], 2) == 0);
    EXPECT_TRUE(memcmp("cdefg", result_pattens[3], 5) == 0);
    EXPECT_TRUE(memcmp("fg", result_pattens[4], 2) == 0);
}

TEST(test_release2, test)
{
    ac_machine_release(handle);
}

int main(int argc,char **argv){
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}