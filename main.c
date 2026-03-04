#include "linear_algebra.h"

int main(int argc, char const *argv[])
{
    Ray x = {.position = point(0, 0, 0), .velocity = vector(1, 1, 0)};
    test_linear_algebra();
    return 0;
}