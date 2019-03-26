#include "smallvector.h"

int main()
{
    lxd::SmallVector<int, 64> vec;

    int i = 0;
    do {
        vec.push_back(i);
        i++;
    } while (i < 100);

    for (auto const v : vec) {
        printf("%d\n", v);
    }
    return 0;
}
