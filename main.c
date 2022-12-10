#include <stdio.h>
#include <stdlib.h>

struct target_Function{
   int x_K;
   int y_K;
};
struct target_Function Introduction();
int main()
{
    struct target_Function z;
    z = Introduction();

    return 0;
}
struct target_Function Introduction(){
    struct target_Function z;
    printf("Hedef fonksiyon ax+by formatinda olmalidir.\n");
    printf("X in katsayisini giriniz_");
    scanf("%d", &z.x_K);

    printf("Y in katsayisini giriniz_");
    scanf("%d", &z.y_K);

    return z;
}
