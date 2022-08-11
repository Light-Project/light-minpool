/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include "minpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TEST_SIZE 0x10000
#define TEST_LOOP 64

struct minpool_test {
    uint8_t pool[TEST_SIZE];
    void *result[TEST_LOOP];
};

static int minpool_testing(struct minpool_test *mtest)
{
    struct minpool_head minpool_test;
    unsigned int count, index;
    size_t size;

    minpool_setup(&minpool_test, &mtest->pool, TEST_SIZE);
    for (count = 0; count < TEST_LOOP; ++count) {
        size = random() % (TEST_SIZE / TEST_LOOP);
        mtest->result[count] = minpool_alloc(&minpool_test, size);
        printf("minpool random alloc%02u: %p\n",
               count, mtest->result[count]);
        if (!mtest->result[count])
            return -ENOMEM;
    }

    for (count = 0; count < TEST_LOOP;) {
        index = random() % TEST_LOOP;
        if (mtest->result[index]) {
            printf("minpool random free%02d: %p %d\n",
                   count, mtest->result[index], index);
            minpool_free(&minpool_test, mtest->result[index]);
            mtest->result[index] = NULL;
            count++;
        }
    }

    return 0;
}

int main(void)
{
    struct minpool_test *mtest;
    int retval;

    mtest = malloc(sizeof(struct minpool_test));
    if (!mtest)
        return -1;

    retval = minpool_testing(mtest);
    free(mtest);

    return retval;
}
