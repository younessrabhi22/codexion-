#include <stdio.h>

typedef struct t_car
{
    char *type;
    int model;

} s_car;

void overwrite(s_car *car2)
{
    car2->type = "mercedes";
    car2->model = 2024;
}



void    main(void)
{
    s_car car1;

    car1.type = "BMW";
    car1.model = 2020;

    printf("%s", car1.type);
    printf("%d", car1.model);

    overwrite(&car1);

    printf("\n%s", car1.type);
    printf("%d", car1.model);


}
