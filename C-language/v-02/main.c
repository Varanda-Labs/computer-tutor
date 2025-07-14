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

#define COR_AMARELO     "\e[33m"
#define COR_NORMAL      "\e[39m"
#define COR_AZUL        "\e[34m"
#define COR_VERMELHO    "\e[31m"

char * erros[NUMERO_DE_ERROS] = { ERRO_1, ERRO_2, ERRO_3, ERRO_4, ERRO_5};

typedef enum
{
    RET_ERRO_PROGRAMACAO    = -1,
    RET_FIM_PERDIDO         = 1000,
    RET_SUCESSO,
    RET_ANDAMENTO,
    RET_ENTRADA_INVALIDA,
    RET_ENTRADA_ACEITA,
} ret_t;

char palavra_secreta[128];
char palavra[128];          // eh mostrado ao jogador, inicialmente : _
int tamanho_palavra = 0;
char entradas[128];

ret_t process_entrada(char c)
{
    int i;
    ret_t ret = RET_ENTRADA_INVALIDA;

    for (i=0; i<tamanho_palavra; i++)
    {
        if (c == palavra_secreta[i])
        {
            palavra[i] = palavra_secreta[i];
            ret = RET_ENTRADA_ACEITA;
        }
    }
    return ret;
}

void print_entradas()
{
    char * p = entradas;
    printf("Entradas: ");
        printf(COR_AZUL);
    while(*p)
    {
        printf("%c ", *p++);
    }
    printf(COR_NORMAL);
    printf("\n");
}

void  print_palavra()
{
    printf("palavra:  ");
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
            while((c = getchar()) != '\n' && c != EOF) ;
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
    ponterio_texto[index++] = 0;

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

    // printf("Errado, tentativa = %d\n\n", tentativa);
    for (i=0; i <= tentativa - 1; i++)
    {
        printf("%s", erros[i]);
    }

    if (tentativa >= 5)
    {
        // printf("Fim do jogo, tente novamente\n");
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

void test_3()
{
    char c = get_letra();
    printf("Letra escolhida foi: %c\n", c);
}

void test_4()
{
    char p[32];
    char c;

    get_palavra("Enter palavra: ", p, 32);
    printf("palavra = %s\n", p);

    c = get_letra();
    printf("letra = %c\n", c);

    get_palavra("Enter palavra: ", p, 32);
    printf("palavra = %s\n", p);

    c = get_letra();
    printf("letra = %c\n", c);

}


#define LINHA_FIM 16
#define COLUNA_FIM 10
#define LINHA_PALAVRA 12
#define COLUNA_PALAVRA 10
#define LINHA_ENTRADAS 14
#define COLUNA_ENTRADAS 10
#define LINHA_PERGUNTA_LETRA 16
#define COLUNA_PERGUNTA_LETRA 10

#define LIMPA_TELA "\e[2J"
#define MOVER_CURSOR "\e[%d;%dH"
void mover_cursor( int linha, int coluna)
{
    printf(MOVER_CURSOR, linha, coluna);
}

void limpa_tela()
{
    printf(LIMPA_TELA);
    printf(MOVER_CURSOR, 1, 1);
}

void jogo()
{
    int num_erros = 0;
    ret_t jogo_estado = RET_ANDAMENTO;
    int num_entradas = 0;
    char c;

    memset(entradas, 0, sizeof(entradas));

    limpa_tela();
    printf("J O G O   D A   F O R C A \n");
    printf("========================= \n\n");
    mover_cursor(LINHA_PALAVRA, COLUNA_PALAVRA);
    get_palavra("Entre com a palavra secreta: ", palavra_secreta, sizeof(palavra_secreta));
    tamanho_palavra = strlen(palavra_secreta);
    memset(palavra, '_', tamanho_palavra);

    while(1) //jogo_estado == RET_ANDAMENTO)
    {
        limpa_tela();
        printf("J O G O   D A   F O R C A \n");
        printf("========================= \n\n");


        if (num_erros > 0)
        {
            if (print_erro(num_erros) != RET_ANDAMENTO)
            {
                mover_cursor(LINHA_FIM, COLUNA_FIM);
                jogo_estado = RET_FIM_PERDIDO;
            }
        }
        mover_cursor(LINHA_PALAVRA, COLUNA_PALAVRA);
        print_palavra();
        mover_cursor(LINHA_ENTRADAS, COLUNA_ENTRADAS);
        print_entradas();
        mover_cursor(LINHA_PERGUNTA_LETRA, COLUNA_PERGUNTA_LETRA);

        if (jogo_estado != RET_ANDAMENTO)
        {
            if (jogo_estado == RET_SUCESSO)
                printf("\n     " COR_AZUL "!!! P A R A B E N S !!!" COR_NORMAL ", voce ganhou o  jogo\n");
            else
                printf("\n" COR_VERMELHO "Jogo perdido :-(\n" COR_NORMAL " Palavra era: " COR_AMARELO
                "%s\n" COR_NORMAL, palavra_secreta);
            break;
        }

        c = get_letra();
        entradas[num_entradas++] = c;
        ret_t r = process_entrada(c);

        if (strcmp(palavra, palavra_secreta) == 0)
        {
            printf("\n     !!! P A R A B E N S !!!, voce ganhou o  jogo\n");
            jogo_estado = RET_SUCESSO;
            continue;
        }

        if (r != RET_ENTRADA_ACEITA)
        {
            num_erros++;
        }

    }
}

//---------------------------- Windows hack -----------------------------
#ifdef _WIN32
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 4
#define STD_OUTPUT_HANDLE 0xFFFFFFF5
#define INVALID_HANDLE_VALUE 0
typedef int DWORD;
int GetConsoleMode(void *, DWORD *);
int SetConsoleMode(void *, DWORD);
void * GetStdHandle(DWORD);
int set_terminal()
{
    DWORD dwMode = 0;
    void * hOut;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        printf("GetStdHandle error\n");
        return 1;
    }

    if (!GetConsoleMode(hOut, &dwMode))
    {
        printf("GetConsoleMode error\n");
        return 1;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        printf("SetConsoleMode error\n");
        return 1;
    }
    return 0;
}
#endif // _WIN32

//---------------------------- Main -----------------------------
int main()
{
    char sim_nao[10];

#ifdef _WIN32
    if(set_terminal() ) return 1;
#endif // _WIN32

    DWORD dwMode = 0;
    void * hOut;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        printf("GetStdHandle error\n");
        return 1;
    }

    if (!GetConsoleMode(hOut, &dwMode))
    {
        printf("GetConsoleMode error\n");
        return 1;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        printf("SetConsoleMode error\n");
        //return 1;
    }

    do
    {
        tamanho_palavra = 0;
        memset(palavra_secreta, 0, sizeof(palavra_secreta));
        memset(palavra, 0, sizeof(palavra));
        memset(entradas, 0, sizeof(entradas));

        jogo();
        get_palavra("\nNovo jogo (sim/nao) ?\n", sim_nao, sizeof(sim_nao));

    } while(strcmp(sim_nao, "sim") == 0);


//    test_4();
//    print_palavra();
    return 0;
}
