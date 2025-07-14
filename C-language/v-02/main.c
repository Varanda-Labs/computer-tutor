#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

 =============
             |
             |
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

#define COR_AMARELO  "\e[33m"
#define COR_NORMAL   "\e[39m"

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

char palavra_secreta[128];
char palavra[128];          // eh mostrado ao jogador, inicialmente : _

void  print_palavra()
{
    //printf("pavalvra = %s\n", palavra);
    int len = strlen(palavra);
    int i;
    printf(COR_AMARELO);

    for (i=0; i<len; i++)
    {
        printf("%c ", palavra[i]);
    }
    printf(COR_NORMAL);
    printf("\n");

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

ret_t get_palavra( char * pergunta, char * ponterio_texto, int numero_maximo_letras)
{
    char c;
    int index = 0;
    printf("%s", pergunta);

    while(1)
    {
        if (index == numero_maximo_letras)
        {
            printf("Voce digitou mais letras do que esperado...\nTente novamente\n%s", pergunta);
            while((c = getchar()) != '\n' && c != EOF) ;  // limpar buffer de entrada
            index = 0;
            continue;
        }
        c = getchar();
        if (c == '\n')
        {
            if (index == 0)
            {
                printf("Nenhuma letra presente...\n%s", pergunta);
                continue;
            }
            break;
        }

        if (c == '\r')
            continue;       // ignore Carrier Return

        if (c >= 'A' && c <= 'Z')
        {
            // caracter eh valido e maiusculo
            // vamos tranformar para minusculo
            c = c + 32;
            ponterio_texto[index++] = c;
            continue;
        }

        if (c >= 'a' && c <= 'z')
        {
            ponterio_texto[index++] = c;
            continue;
        }

        printf("Voce digitou caracter invalido...\nTente novamente\n%s:\n", pergunta);
        while((c = getchar()) != '\n' && c != EOF) ;
        index = 0;

    }
    ponterio_texto[index] = 0;

    return RET_ENTRADA_ACEITA;
}

char get_letra()
{
    char t[2];

    get_palavra( "Entre com a letra desejada: ", t, 2);
    return t[0];

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
    r = get_palavra("Digite a palavra secreta: ", palavra_secreta, sizeof(palavra_secreta));
    if (r == RET_ENTRADA_ACEITA)
    {
        printf("A Palavra eh: %s, comprimento = %d caracteres\n", palavra_secreta, (int) strlen(palavra_secreta));
        r = para_minusculo(palavra_secreta);
        if (r == RET_ENTRADA_ACEITA)
        {
            printf("resultado limpo = %s\n", palavra_secreta);
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
    printf("Letra escolhida foi: %c\n", c);
}

void test_4()
{
    char p[32];
    char c;

    get_palavra("Enter palavra: ", p, sizeof(p));
    printf("palavra = %s\n", p);

    c = get_letra();
    printf("letra = %c\n", c);

    get_palavra("Enter palavra: ", p, 32);
    printf("palavra = %s\n", p);

    c = get_letra();
    printf("letra = %c\n", c);

}

//---------------------------- Main -----------------------------

int main()
{
    test_4();
    print_palavra();
    return 0;
}
