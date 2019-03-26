#include "smallvector.h"

int main()
{
    lxd::SmallVector<int, 64> vec;

    int i = 0;
    do {
        vec.push_back(i);
        i++;
    } while (i < 100);

    for (size_t n=0; n < vec.size(); ++n) {
        printf("%d\n", vec[n]);
    }
    return 0;
}
