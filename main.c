#include "codexion.h"




int main(int argc, char **argv)
{
    t_rules rules;

    if (argc != 9 || !init_rules(&rules, argv))
    {
        printf("Error ditected");
        exit(1);
    }

    printf("%d", rules.num_coders);
    printf("%s", "\nok");

}
