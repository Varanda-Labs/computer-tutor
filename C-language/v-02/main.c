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

#define ERRO_1 " =============\n\
             #\n\
             #\n"

#define ERRO_2 "             O\n"

#define ERRO_3 "            \\|/\n"

#define ERRO_4 "             |\n"

#define ERRO_5 "            / \\\n"

#define NUMERO_DE_ERROS 5

//------ comandos escape para o terminal
#define COR_AMARELO   "\e[33m"
#define COR_VERMELHO  "\e[31m"
#define COR_AZUL      "\e[34m"
#define COR_NORMAL    "\e[39m"
#define LIMPA_TELA    "\e[2J"
#define MOVER_CURSOR "\e[%d;%dH"

char * erros[NUMERO_DE_ERROS] = { ERRO_1, ERRO_2, ERRO_3, ERRO_4, ERRO_5};


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

//---------------------------- Windows hack -----------------------------
#ifdef _WIN32 // mesmo que #if defined(_WIN32)
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

void mover_cursor( int linha, int coluna)
{
    printf(MOVER_CURSOR, linha, coluna);
}

void limpa_tela()
{
    printf(LIMPA_TELA);
    mover_cursor( 1,1);
}

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





//---------------------------- Main -----------------------------
#define NAO 0
#define SIM 1

#define OCULTO '*'

void jogo()
{
    int num_erros = 0;
    int num_tentativas = 0;
    char c;
    char tentativas[128];
    int i;
    ret_t r;
    int sair = NAO;
    int tamanho_da_palavra;
    int achou_letra;

    memset(tentativas, 0, sizeof(tentativas));
    memset(palavra_secreta, 0, sizeof(palavra_secreta));
    memset(palavra, 0, sizeof(palavra));

    get_palavra("Entre com a palavra secreta: ", palavra_secreta, sizeof(palavra_secreta));
    tamanho_da_palavra = strlen(palavra_secreta);
    memset(palavra, OCULTO, tamanho_da_palavra);

    while(sair == NAO)
    {
        limpa_tela();
        //------ imprimir erros (se tiver) ------
        if (num_erros > 0)
        {
            r = print_erro(num_erros);
            if (r != RET_ANDAMENTO)
            {
                sair = SIM;
            }
        }

        //----- imprimir resultados -------
        mover_cursor(12,10);
        printf("Palavra: ");
        print_palavra();
        if (num_tentativas > 0)
        {
            printf("Tentativas: ");
            for (i=0; i<num_tentativas; i++)
            {
                printf("%c ", tentativas[i]);
            }
            printf("\n");
        }
        printf("Numero de tentativas: %d\n", num_tentativas);

        //---- caso sair == SIM significa que o jogo foi perdido
        if (sair == SIM)
        {
            printf(COR_VERMELHO "Jogo perdido :-( "  COR_NORMAL "\n");
            continue;
        }

        //------- usuario entra letra e verificamos o resultado.
        achou_letra = NAO;
        c = get_letra();
        tentativas[num_tentativas++] = c;

        //---- verifica se letra eh valida ---
        for (i=0; i<tamanho_da_palavra; i++)
        {
            if (c == palavra_secreta[i])
            {
                palavra[i] = c;
                achou_letra = SIM;
            }
        }

        if (achou_letra == NAO)
        {
            num_erros++;
        }

        if (strcmp(palavra, palavra_secreta) == 0)
        {
            sair = SIM;
            printf(COR_AZUL "!!! PARABENS !!! voce ganhou o jogo" COR_NORMAL " \n");
        }
    }

}


int main()
{
#ifdef _WIN32
    if (set_terminal() != 0)
    {
        return 1;
    }
#endif // _WIN32
    char resposta[32];

    do
    {
        limpa_tela();
        printf("        JOGO DA FORCA\n\n");
        jogo();
        get_palavra("\nDeseja jogar novamente (sim ou nao) ?\n", resposta, sizeof(resposta));
    } while (strcmp(resposta, "sim") == 0);

    return 0;
}
