#include <stdio.h>

// a type for this flavor of provider
typedef long (*long_func_ptr)(int param);

// BEGIN "PROVIDER STUFF"
long_func_ptr fibonacci_provider;

long fibonacci(int of_num) {
    printf(__FILE__ ":%2d   fibonacci(%d) called\n", __LINE__, of_num);

    if (of_num < 2)
        return of_num;

    return (*fibonacci_provider)(of_num - 1) + (*fibonacci_provider)(of_num - 2);
}

// BEGIN CACHE STUFF

#define MAX_MEMOIZED 100
#define ARRAY_SIZE (MAX_MEMOIZED + 1)
#define NO_VALUE_YET -1
long_func_ptr _original_provider;
long _memoization_data[ARRAY_SIZE];

long cache_func(int of_num) {
    printf(__FILE__ ":%2d   cache_func(%d) called\n", __LINE__, of_num);

    if (of_num > MAX_MEMOIZED)
        return (*_original_provider)(of_num);

    if (_memoization_data[of_num] == NO_VALUE_YET)
        _memoization_data[of_num] = (*_original_provider)(of_num);

    return _memoization_data[of_num];
}

long_func_ptr init_cache(long_func_ptr real_provider) {
    for (int ix = 0; ix < ARRAY_SIZE; ix++)
        _memoization_data[ix] = NO_VALUE_YET;

    _original_provider = real_provider;
    return cache_func;
}

// BEGIN MAIN

int main(int argc, char *argv[]) {
    int test_val;

    if (argc < 2 || 1 != sscanf(argv[1], "%d", &test_val) || test_val < 0)
        return 1;

    // PROGRAM INITIALIZATION
    fibonacci_provider = init_cache(fibonacci);

    // PROGRAM FUNCTIONALITY

    // how to use it
    for (int ix = test_val; ix > 0; ix--) {
        printf(__FILE__ ":%2d   Fibonacci of %d is %ld\n", __LINE__, ix,
               (*fibonacci_provider)(ix));
    }
    for (int ix = 0; ix <= test_val; ix++) {
        printf(__FILE__ ":%2d   Fibonacci of %d is %ld\n", __LINE__, ix,
               (*fibonacci_provider)(ix));
    }

    return 0;
}
