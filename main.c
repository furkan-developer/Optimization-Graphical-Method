#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct target_Function{
   int x_K;
   int y_K;
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
int main()
{
    struct target_Function z;
    int const_arr_lenght;

    z = Introduction();

    const_arr_lenght = Get_All_Constraint_Begin();
    struct constraint model_constraints[const_arr_lenght];
    Get_All_Constraint(const_arr_lenght,model_constraints);

    Print_Constraints(const_arr_lenght,model_constraints);

    int ncr = fact(const_arr_lenght) / (fact(2)*fact(const_arr_lenght-2));
    printf("TOPLAM KISIT FONKSIYON KARSILASTIRMASI %d\n",ncr);

    struct corner_point corner_points[(const_arr_lenght*2)+ncr];
    Calculate_Corner_Points_To_Constraints(model_constraints,const_arr_lenght,corner_points);

    Print_Corner_Points((const_arr_lenght*2)+ncr,corner_points);

    struct Corner_Points_Value_On_Target_Function values[(const_arr_lenght*2)+ncr];

    Check_Corner_Points_On_Feasible_Solution_Area(corner_points,(const_arr_lenght*2)+ncr,model_constraints,const_arr_lenght,values,z);

    Print_Values(values,(const_arr_lenght*2)+ncr);

    /*
    double result = Calculate_Delta(1,1,10,6);
    printf("\SONUC = %.2lf",result);
    */
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
        printf("\n\nKisit-%d olusturunuz\n",i+1);
        printf("x in katsayisini (a) giriniz_");
        scanf("%lf", &model_const[i].x_K);

        printf("y in katsayisini (b) giriniz_");
        scanf("%lf", &model_const[i].y_K);

        printf("Kisit kucuk esittir ise : 0\n");
        printf("Kisit buyuk esittir ise : 1\n");
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
void Calculate_Corner_Points_To_Constraints(struct constraint datas[],int datas_lenght,struct corner_point *corner_points){
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

    Print_Constraints(datas_lenght,datas);

    for(i = 0; i < datas_lenght-1; i++){
        for(j = i+1; j < datas_lenght; j++){
            // kesişim noktası varmı kontrol et
            result = Is_Intersection(datas[i].x_K,datas[j].x_K,datas[i].y_K,datas[j].y_K);
            if(result == true){
                printf("KESISIM NOKTASI VAR\n");
                printf("Delta hesap parametreleri : \n");
                delta = Calculate_Delta(datas[i].x_K,datas[i].y_K,datas[j].x_K,datas[j].y_K);
                printf("delta hesaptan gelen delta değeri : %.2lf\n",delta);

                printf("Delta_x hesap parametreleri : \n");
                delta_x = Calculate_Delta(datas[i].c,datas[i].y_K,datas[j].c,datas[j].y_K);
                printf("delta hesaptan gelen delta-x değeri : %.2lf\n",delta_x);

                printf("Delta_y hesap parametreleri : \n");
                delta_y = Calculate_Delta(datas[i].x_K,datas[i].c,datas[j].x_K,datas[j].c);
                printf("delta hesaptan gelen delta_y değeri : %.2lf\n",delta_y);

                printf("delta = %.2lf   delta x = %.2lf delta y = %.2f\n",delta,delta_x,delta_y);

                inter_section_points[k].x_axis = delta_x / delta;
                inter_section_points[k].y_axis = delta_y / delta;

                printf("\nnoktanın yazılan index değeri : %d \nx,y %.2lf,%.2lf",k,inter_section_points[k].x_axis,inter_section_points[k].y_axis);
                k++;
            }
            else{
                printf("KESISIM NOKTASI YOK\n");
                inter_section_points[k].x_axis = -1;
                inter_section_points[k].y_axis = -1;
                k++;
            }
        }
    }
}
double Calculate_Delta(double a,double b,double c,double d){
    printf("a = %lf\nb = %lf\nc = %lf\nd = %lf\n",a,b,c,d);
    double resutl1 = (a*d);
    printf("result1 = a*d = %.2lf\n",resutl1);
    double result2 = (c*b);
    printf("result2 = c*b = %.2lf\n",result2);
    double result3 = resutl1 - result2;
    printf("result3 = result1 - result2 : %.2lf",result3);
    return result3;
}
bool Is_Intersection(double a1,double a2,double b1, double b2){
    printf("a1 = %.2lf a2 = %.2lf b1 = %.2lf b2 = %2.lf",a1,a2,b1,b2);
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
            printf("Buraya girmemesi gerekiyordu");
            continue;
        }
        for(j = 0; j < constraints_lenght; j++){
            result = (constraints[j].x_K * datas[i].x_axis) + (constraints[j].y_K * datas[i].y_axis);
            printf("Nokta %d Kisit %d result = %.2lf\n",i+1,j+1,result);
            if(constraints[j].state == 0){
                if(result <= constraints[j].c){
                // mümkün çözüm noktasında
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
