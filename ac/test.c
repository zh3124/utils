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
    ac_machine_insert(handle, "abd");
    ac_machine_insert(handle, "abdk");
    ac_machine_insert(handle, "abchijn");
    ac_machine_insert(handle, "chnit");
    ac_machine_insert(handle, "ijabdf");
    ac_machine_insert(handle, "ijaij");
}

TEST(test_compile, test)
{
    ac_machine_compile(handle);
}

char *text = "abchnijabdfk";
TEST(test_search, test)
{
    int max_num = 10;
    char **result_pattens = (char **)malloc(sizeof(char *) * max_num);
    int *patten_lens = (int *)malloc(sizeof(int) * max_num);
    int ret = ac_machine_search(handle, text, strlen(text), result_pattens, patten_lens, max_num);
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