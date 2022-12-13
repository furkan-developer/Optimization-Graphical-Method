#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
struct target_Function{
   int x_K;
   int y_K;
   int flag;
};
struct constraint{
    double x_K;
    double y_K;
    int state;
    double c;
};
struct corner_point{
    double x_axis;
    double y_axis;
};
struct Corner_Points_Value_On_Target_Function{
    struct corner_point corner_point;
    double Point_Value;
};
struct target_Function Introduction();
void Get_All_Constraint(int total_const,struct constraint *model_const);
int Get_All_Constraint_Begin();
void Print_Constraints(int arr_lenght,struct constraint data[]);
int fact(int n);
bool Is_Intersection(double a1,double a2,double b1, double b2);
double Calculate_Delta(double a,double b,double c,double d);
void Calculate_Intersection_Points(struct constraint datas[],int datas_lenght,struct corner_point *inter_section_points,int end_point_index);
void Calculate_Corner_Points_To_Constraints(struct constraint datas[],int datas_lenght,struct corner_point *corner_points);
void Print_Corner_Points(int arr_lenght,struct corner_point data[]);
double Calculate_Target_Function_Value(double x_axis,double y_axis,struct target_Function z);
void Check_Corner_Points_On_Feasible_Solution_Area(struct corner_point *datas,int datas_lenght,struct constraint *constraints,int constraints_lenght,struct Corner_Points_Value_On_Target_Function *values,struct target_Function z);
void Print_Values(struct Corner_Points_Value_On_Target_Function *array, int size);
void swap(struct Corner_Points_Value_On_Target_Function *a, struct Corner_Points_Value_On_Target_Function *b);
int partition(struct Corner_Points_Value_On_Target_Function array[], int low, int high);
void quickSort(struct Corner_Points_Value_On_Target_Function array[], int low, int high);
struct Corner_Points_Value_On_Target_Function Find_Optimal_Point(struct Corner_Points_Value_On_Target_Function *values,int values_length,struct target_Function z);
void Print_Model(struct target_Function z,struct constraint model_constraints[],int const_arr_length);
int main()
{
    time_t start,end;
    double execution_time;
    struct target_Function z;
    int const_arr_lenght;

    z = Introduction();

    const_arr_lenght = Get_All_Constraint_Begin();
    struct constraint model_constraints[const_arr_lenght];
    Get_All_Constraint(const_arr_lenght,model_constraints);

    //Print_Constraints(const_arr_lenght,model_constraints);

    Print_Model(z,model_constraints,const_arr_lenght);
    int ncr = fact(const_arr_lenght) / (fact(2)*fact(const_arr_lenght-2));

    struct corner_point corner_points[(const_arr_lenght*2)+ncr];

    start = clock();

    Calculate_Corner_Points_To_Constraints(model_constraints,const_arr_lenght,corner_points);

    struct Corner_Points_Value_On_Target_Function values[(const_arr_lenght*2)+ncr];

    Check_Corner_Points_On_Feasible_Solution_Area(corner_points,(const_arr_lenght*2)+ncr,model_constraints,const_arr_lenght,values,z);

    struct Corner_Points_Value_On_Target_Function optimal_point = Find_Optimal_Point(values,(const_arr_lenght*2)+ncr,z);

    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;

    if(optimal_point.Point_Value == -1){
        printf("\nGirilen model cozumsuzdur.");
    }
    else{
        printf("\nOPTIMAL NOKTA : (%.2lf,%.2lf) DEGER : %.2lf",optimal_point.corner_point.x_axis,optimal_point.corner_point.y_axis,optimal_point.Point_Value);
    }

    printf("\n\nAlgoritmanin calisma suresi : %lf ms",execution_time);
    return 0;
}
struct target_Function Introduction(){
    struct target_Function z;
    printf("Hedef fonksiyon ax+by formatinda olmalidir.\n\n");
    printf("X in katsayisini giriniz_");
    scanf("%d", &z.x_K);

    printf("Y in katsayisini giriniz_");
    scanf("%d", &z.y_K);

    printf("\nMin. problem icin 0\nMax. problem icin 1 giriniz_");
    scanf("%d",&z.flag);
    return z;
}
void Get_All_Constraint(int total_const,struct constraint *model_const){
    int i;

    printf("\nModelin kisitlari ax + by <= c veya ax + by >= c formatinda olmalidir.\n");
    printf("===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====");

    int state;
    for( i = 0; i < total_const; i++){
        printf("\n\nKisit-%d olusturunuz\n",i+1);
        printf("x in katsayisini (a) giriniz_");
        scanf("%lf", &model_const[i].x_K);

        printf("y in katsayisini (b) giriniz_");
        scanf("%lf", &model_const[i].y_K);

        printf("Kisit kucuk esittir icin 0\n");
        printf("Kisit buyuk esittir icin 1 giriniz_");
        scanf("%d", &model_const[i].state);
        // burada 0-1 kontrolü yap

        printf("c degerini giriniz_");
        scanf("%lf", &model_const[i].c);
    }
}
void Print_Constraints(int arr_lenght,struct constraint data[]){
    int i;
    printf("\n===== ===== ===== ===== ===== Modelin Kisitlari ===== ===== ===== ===== =====\n");
    for(i = 0; i < arr_lenght; i++){
        printf("kisit-%d : %.2lfx + %.2lfy ",i+1,data[i].x_K,data[i].y_K);
        if(data[i].state == 0){
            printf("<");
        }
        else{
            printf(">");
        }
        printf("= %.2lf\n",data[i].c);
    }
    printf("x,y >= 0");
}
int Get_All_Constraint_Begin(){
    int total_const;
    int i;
    while(1 > 0){
        printf("\nModelin toplam kisit sayisini giriniz_");
        scanf("%d", &total_const);
        if(total_const <= 0){
            printf("Modelin kisit sayisi sifir veya sifirdan kucuk olamaz!\n");
            continue;
        }
        break;
    }
    return total_const;
}
void Calculate_Corner_Points_To_Constraints(struct constraint datas[],int datas_lenght,struct corner_point *corner_points){
    int i;
    int j = 0;
    for(i = 0; i < datas_lenght; i++){
        corner_points[j].y_axis = 0;
        if(datas[i].x_K != 0){
            corner_points[j].x_axis = datas[i].c / datas[i].x_K;
        }else{
            corner_points[j].x_axis = 0;
        }
        ++j;
        corner_points[j].x_axis = 0;
        if(datas[i].y_K != 0){
            corner_points[j].y_axis = datas[i].c / datas[i].y_K;
        }else{
            corner_points[j].y_axis = 0;
        }
        ++j;
    }

    Calculate_Intersection_Points(datas,datas_lenght,corner_points,datas_lenght*2);
}
void Print_Corner_Points(int arr_lenght,struct corner_point data[]){
    int i;

    printf("\n===== ===== ===== ===== =====   Kose Noktalar   ===== ===== ===== ===== =====\n");
    for(i = 0; i < arr_lenght; i++){
        printf("(%.2lf,%.2lf)\n",data[i].x_axis,data[i].y_axis);
    }
}
void Calculate_Intersection_Points(struct constraint datas[],int datas_lenght,struct corner_point *inter_section_points,int end_point_index){
    int i,j,k = end_point_index;
    double delta;
    double delta_x;
    double delta_y;
    bool result;

    for(i = 0; i < datas_lenght-1; i++){
        for(j = i+1; j < datas_lenght; j++){
            result = Is_Intersection(datas[i].x_K,datas[j].x_K,datas[i].y_K,datas[j].y_K);
            if(result == true){

                delta = Calculate_Delta(datas[i].x_K,datas[i].y_K,datas[j].x_K,datas[j].y_K);

                delta_x = Calculate_Delta(datas[i].c,datas[i].y_K,datas[j].c,datas[j].y_K);

                delta_y = Calculate_Delta(datas[i].x_K,datas[i].c,datas[j].x_K,datas[j].c);

                inter_section_points[k].x_axis = delta_x / delta;
                inter_section_points[k].y_axis = delta_y / delta;

                k++;
            }
            else{
                inter_section_points[k].x_axis = -1;
                inter_section_points[k].y_axis = -1;
                k++;
            }
        }
    }
}
double Calculate_Delta(double a,double b,double c,double d){
    return (a*d) - (c*b);
}
bool Is_Intersection(double a1,double a2,double b1, double b2){
    double x_axis_ratio = a1/a2;
    double y_axis_ratio = b1/b2;

    if(x_axis_ratio == y_axis_ratio){
        return false;
    }
    return true;
}
int fact(int n){
    int i,f=1;
    for(i=1;i<=n;i++)
    {
        f=f*i;
    }
    return f;
}
void Check_Corner_Points_On_Feasible_Solution_Area(struct corner_point *datas,int datas_lenght,struct constraint *constraints,int constraints_lenght,struct Corner_Points_Value_On_Target_Function *values,struct target_Function z){
    int i,j,k=0;
    double result;
    bool flag = true;
    for(i = 0; i < datas_lenght; i++){
        if((datas[i].x_axis < 0) || (datas[i].y_axis < 0)){
            values[k].corner_point.x_axis = datas[i].x_axis;
            values[k].corner_point.y_axis = datas[i].y_axis;
            values[k].Point_Value = -1;
            k++;
            continue;
        }
        for(j = 0; j < constraints_lenght; j++){
            result = (constraints[j].x_K * datas[i].x_axis) + (constraints[j].y_K * datas[i].y_axis);

            if(constraints[j].state == 0){
                if(result <= constraints[j].c){
                    continue;
                }
                flag = false;
                break;
            }
            else{
                if(result >= constraints[j].c){
                // mümkün çözüm noktasında
                    flag = true;
                    continue;
                }
                flag = false;
                break;
            }
        }
        if(flag == true){
            values[k].corner_point.x_axis = datas[i].x_axis;
            values[k].corner_point.y_axis = datas[i].y_axis;
            values[k].Point_Value = Calculate_Target_Function_Value(datas[i].x_axis,datas[i].y_axis,z);
            k++;
        }
        else{
            values[k].corner_point.x_axis = datas[i].x_axis;
            values[k].corner_point.y_axis = datas[i].y_axis;
            values[k].Point_Value = -1;
            k++;
        }
        flag = true;
    }
}
void Print_Values(struct Corner_Points_Value_On_Target_Function *array, int size) {
  for (int i = 0; i < size; ++i) {
    printf("Kose Nokta : (%.2lf,%.2lf) |=|=|=| Amac foksiyondaki degeri : %.2lf\n",array[i].corner_point.x_axis,array[i].corner_point.y_axis,array[i].Point_Value);
  }
  printf("\n");
}
double Calculate_Target_Function_Value(double x_axis,double y_axis,struct target_Function z){
    return (x_axis * z.x_K) + (y_axis * z.y_K);
}
struct Corner_Points_Value_On_Target_Function Find_Optimal_Point(struct Corner_Points_Value_On_Target_Function values[],int values_length,struct target_Function z){
    int i;

    quickSort(values, 0, values_length- 1);

    if(z.flag == 1){
        return values[values_length-1];
    }
    else{
        int i = 0;
        while(i < values_length){
           if(values[i].Point_Value != -1){
                return values[i];
           }
           i++;
        }
        return values[values_length-1];
    }
}
void quickSort(struct Corner_Points_Value_On_Target_Function array[], int low, int high){
    int i;

    if (low < high) {
        int pi = partition(array, low, high);

        quickSort(array, low, pi - 1);

        quickSort(array, pi + 1, high);
    }
}
int partition(struct Corner_Points_Value_On_Target_Function array[], int low, int high) {
    double pivot = array[high].Point_Value;

    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (array[j].Point_Value <= pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);

    return (i + 1);
}
void swap(struct Corner_Points_Value_On_Target_Function *a, struct Corner_Points_Value_On_Target_Function *b) {
    struct Corner_Points_Value_On_Target_Function t = *a;
    *a = *b;
    *b = t;
}
void Print_Model(struct target_Function z,struct constraint model_constraints[],int const_arr_length){
    int i;

    printf("\nOlusturulan Model\n");
    if(z.flag == 1){
        printf("\nMax. ");
    }
    else{
        printf("\nMin. ");
    }
    printf("Z = %.2lfx + %.2lfy\n",z.x_K,z.y_K);
    for(i = 0; i < const_arr_length; i++){
        printf("Kisit-%d : %.2lfx + %.2lfy ",i+1,model_constraints[i].x_K,model_constraints[i].y_K);
        if(model_constraints[i].state == 0){
            printf("<");
        }
        else{
            printf(">");
        }
        printf("= %.2lf\n",model_constraints[i].c);
    }
}

