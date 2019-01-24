#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>

#include "bm.h"

void *handle1;

char *patten1 = "example";
char *text1 = "here is a simple example";

TEST(test_compile, test)
{
    EXPECT_TRUE(handle1 == NULL);
    handle1 = bm_compile((uint8_t*)patten1, 7);
    EXPECT_TRUE(handle1 != NULL);
}

TEST(test_search, test)
{
    uint8_t *result = bm_search(handle1, (uint8_t*)text1, 24);
    EXPECT_TRUE(result != NULL);
    EXPECT_TRUE(result == (uint8_t*)text1 + 17);
}

TEST(test_release, test)
{
    bm_release(handle1);
}

int main(int argc,char **argv){
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}