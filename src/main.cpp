#include "smallvector.h"
#include <algorithm>

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

    auto iter = std::find(std::begin(vec), std::end(vec), 88);
    printf("%d\n", *iter);

    return 0;
}
