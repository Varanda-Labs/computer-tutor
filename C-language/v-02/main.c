#include <stdio.h>
#include <stdlib.h>

/*

 =============
             #
             #
             O
            \|/
             |
            / \

*/

#define MEU_TEXTO "Ola como vai\n\
Este eh a proxima linha\n"

#define ERRO_1 " =============\n\
             #\n\
             #\n"

#define ERRO_2 "             O\n"

#define ERRO_3 "            \\|/\n"

#define ERRO_4 "             |\n"

#define ERRO_5 "            / \\\n"

#define NUMERO_DE_ERROS 5

char * erros[NUMERO_DE_ERROS] = { ERRO_1, ERRO_2, ERRO_3, ERRO_4, ERRO_5};

int print_erro(int tentativa)
{
    int i;

    if (tentativa > 5)
    {
        printf("Erro de programacao\n");
        return 0;
    }

    printf("Errado, tentativa = %d\n\n", tentativa);
    for (i=0; i <= tentativa - 1; i++)
    {
        printf("%s", erros[i]);
    }

    if (tentativa >= 5)
    {
        printf("Fim do jogo, tente novamente\n");
        return 0;
    }
    return 1;
}

int main()
{
    int i;

    for (i = 1; i <= 5; i++)
    {
        printf("\n");

        //printf("sizeof(erros) = %d, sizeof(void  *) = %d\n", sizeof(erros), sizeof(void *));
        int res = print_erro(i);

        if (res != 0)
        {
            printf("jogo em andamente\n");
        }
        else
        {
            printf("jogo terminou\n");
        }

        printf("\n");
    }
    return 0;
}
