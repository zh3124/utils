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
    ac_machine_insert(handle, (uint8_t *)"abdk", 4);
    ac_machine_insert(handle, (uint8_t *)"abchijn", 7);
    ac_machine_insert(handle, (uint8_t *)"chnit", 5);
    ac_machine_insert(handle, (uint8_t *)"ijabdf", 6);
    ac_machine_insert(handle, (uint8_t *)"ijaij", 5);
}

TEST(test_compile, test)
{
    ac_machine_compile(handle);
}

uint8_t *text = (uint8_t *)"abchnijabdfk";
TEST(test_search, test)
{
    int max_num = 10;
    uint8_t **result_pattens = (uint8_t **)malloc(sizeof(uint8_t *) * max_num);
    uint8_t *patten_lens = (uint8_t *)malloc(sizeof(uint8_t) * max_num);
    int ret = ac_machine_search(handle, text, 12, result_pattens, patten_lens, max_num);
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

int main(int argc,char **argv){
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}