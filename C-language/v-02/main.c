#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//#define RET_ERRO_PROGRAMACAO    -1
//#define RET_FIM_PERDIDO         1000
//#define RET_ANDAMENTO           1001
//#define RET_ENTRADA_INVALIDA    1002
//#define RET_ENTRADA_ACEITA      1003

typedef enum
{
    RET_ERRO_PROGRAMACAO    = -1,
    RET_FIM_PERDIDO         = 1000,
    RET_ANDAMENTO,
    RET_ENTRADA_INVALIDA,
    RET_ENTRADA_ACEITA,
} ret_t;

char palavra[128];

char get_letra()
{
    char letra;
    int r;

    printf("Entre com a letra desejada: ");
    while(1)
    {
        int letra_valida = 0; // zero significa invalida
        r = scanf("%c", &letra);
        if (r == 1)
        {
            if (letra == '\n')
                continue;

            if (letra >= 'A' && letra <= 'Z')
            {
                // caracter eh valido e maiusculo
                // vamos tranformar para minusculo
                letra = letra + 32;
                letra_valida = 1;
            }

            if (letra >= 'a' && letra <= 'z')
            {
                letra_valida = 1;
            }

            if (letra_valida == 1)
            {
                return letra;
            }

            printf("letra invalida, Entre com a letra desejada: \n");
        }

    }

}

// aceita apenas caracteres de A~Z ou a~z
ret_t para_minusculo(char * text)
{
    while(*text != 0)
    {
        if (*text >= 'A' && *text <= 'Z')
        {
            // caracter eh valido e maiusculo
            // vamos tranformar para minusculo
            *text = (*text) + 32;
            text++;
            continue;
        }

        if (*text >= 'a' && *text <= 'z')
        {
            // caracter eh valido e minusculo
            text++;
            continue;
        }
        return RET_ENTRADA_INVALIDA;
    }
    return RET_ENTRADA_ACEITA;
}

ret_t get_palavra( char * pergunta)
{
    int r;
    printf("%s: ", pergunta);
    r = scanf("%s", palavra);
    if (r == 1)
    {
        return RET_ENTRADA_ACEITA;
    }
    return RET_ENTRADA_INVALIDA;
}

ret_t print_erro(int tentativa)
{
    int i;

    if (tentativa > 5)
    {
        printf("Erro de programacao\n");
        return RET_ERRO_PROGRAMACAO;
    }

    printf("Errado, tentativa = %d\n\n", tentativa);
    for (i=0; i <= tentativa - 1; i++)
    {
        printf("%s", erros[i]);
    }

    if (tentativa >= 5)
    {
        printf("Fim do jogo, tente novamente\n");
        return RET_FIM_PERDIDO;
    }
    return RET_ANDAMENTO;
}

//-------------------- Unit Test
int test_1()
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

int test_2()
{
    ret_t r;
    r = get_palavra("Digite a palavra secreta");
    if (r == RET_ENTRADA_ACEITA)
    {
        printf("A Palavra eh: %s, comprimento = %d caracteres\n", palavra, (int) strlen(palavra));
        r = para_minusculo(palavra);
        if (r == RET_ENTRADA_ACEITA)
        {
            printf("resultado limpo = %s\n", palavra);
        }
        else
        {
            printf("palavra com caracteres nao validos\n");
        }


    }
    else
    {
        printf("Entrada invalida\n");
    }

    return 0;
}

void test_3()
{
    char c = get_letra();
    printf("Letra escolhida foi: %c", c);
}



//---------------------------- Main -----------------------------

int main()
{
    test_3("ABCDEF");
    return 0;
}
