#include <stdio.h>
#include <stdlib.h>

struct target_Function{
   int x_K;
   int y_K;
};
struct constraint{
    float x_K;
    float y_K;
    int state;
    float c;
};
struct corner_point{
    float x_axis;
    float y_axis;
};
struct target_Function Introduction();
void Get_All_Constraint(int total_const,struct constraint *model_const);
int Get_All_Constraint_Begin();
void Print_Constraints(int arr_lenght,struct constraint data[]);
void calculate_corner_points_to_constraints(struct constraint datas[],int datas_lenght,struct corner_point *corner_points);
void Print_Corner_Points(int arr_lenght,struct corner_point data[]);
int main()
{
    struct target_Function z;
    int const_arr_lenght;

    z = Introduction();

    const_arr_lenght = Get_All_Constraint_Begin();
    struct constraint model_constraints[const_arr_lenght];
    Get_All_Constraint(const_arr_lenght,model_constraints);

    //Print_Constraints(const_arr_lenght,model_constraints);

    struct corner_point corner_points[const_arr_lenght*2];
    calculate_corner_points_to_constraints(model_constraints,const_arr_lenght,corner_points);

    //Print_Corner_Points(const_arr_lenght*2,corner_points);

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
void Get_All_Constraint(int total_const,struct constraint *model_const){
    int i;

    printf("\nModelin kisitlari ax + by <= c veya ax + by >= c formatinda olmalidir.\n");
    printf("===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====");

    int state;
    for( i = 0; i < total_const; i++){
        printf("\n\nKisit-%d olusturunuz\n",i);
        printf("x in katsayisini (a) giriniz_");
        scanf("%f", &model_const[i].x_K);

        printf("y in katsayisini (b) giriniz_");
        scanf("%f", &model_const[i].y_K);

        printf("Kisit kucuk esittir ise : 0\n");
        printf("Kisit buyuk esittir ise : 1\n");
        scanf("%d", &model_const[i].state);
        // burada 0-1 kontrolü yap

        printf("c degerini giriniz_");
        scanf("%f", &model_const[i].c);
    }
}
void Print_Constraints(int arr_lenght,struct constraint data[]){
    int i;
    printf("\n===== ===== ===== ===== ===== Modelin Kisitlari ===== ===== ===== ===== =====\n");
    for(i = 0; i < arr_lenght; i++){
        printf("kisit-%d : %.2fx + %.2fy ",i+1,data[i].x_K,data[i].y_K);
        if(data[i].state == 0){
            printf("<");
        }
        else{
            printf(">");
        }
        printf("= %.2f\n",data[i].c);
    }
    printf("x,y >= 0");
}
int Get_All_Constraint_Begin(){
    int total_const;
    int i;
    while(1 > 0){
        printf("Modelin toplam kisit sayisini giriniz_");
        scanf("%d", &total_const);
        if(total_const <= 0){
            printf("Modelin kisit sayisi sifir veya sifirdan kucuk olamaz!\n");
            continue;
        }
        break;
    }
    return total_const;
}
void calculate_corner_points_to_constraints(struct constraint datas[],int datas_lenght,struct corner_point *corner_points){
    int i;
    int j = 0;
    for(i = 0; i < datas_lenght; i++){
        corner_points[j].x_axis = datas[i].c / datas[i].x_K;
        corner_points[j].y_axis = 0;
        ++j;
        corner_points[j].x_axis = 0;
        corner_points[j].y_axis = datas[i].c / datas[i].y_K;
        ++j;
    }
}
void Print_Corner_Points(int arr_lenght,struct corner_point data[]){
    int i;

    printf("\n===== ===== ===== ===== =====   Kose Noktalar   ===== ===== ===== ===== =====\n");
    for(i = 0; i < arr_lenght; i++){
        printf("(%.2f,%.2f)\n",data[i].x_axis,data[i].y_axis);
    }
}
