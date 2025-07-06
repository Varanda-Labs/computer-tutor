
#include <stdio.h>

// Comentario:

void minha_funcao(float x);

int main()
{
    /* printf:
        PT: imprime caracteres formatados no dispositivo padrao de saida (stdout). Imprime no console quando o programa
            eh lancado atraves do mesmo.

        EN: prints formated characters to the stardard output device (stdout). Prints to the console when the program
            is lauched from it. */

    printf("  PT: Ola amigos\n  EN: Hello world!\n\n");

    minha_funcao(15);

    printf("tchau\n");
    return 0;
}

void minha_funcao(float x)
{
  x = x + 5.5;
  printf("esta eh a minha funcao, o valor de x = %f\n", x);
}







#if 0
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main()
{
    /* printf:
        PT: imprime caracteres formatados no dispositivo padrao de saida (stdout). Imprime no console quando o programa
            eh lancado atraves do mesmo.

        EN: prints formated characters to the stardard output device (stdout). Prints to the console when the program
            is lauched from it. */

    printf("PT: Ola amigos\n, EN: Hello world!\n\n");

    /* \n
        PT: a sequencia '\n' representa um caracter especial do comando "nova linha". caracteres que seguem o \n serao
            impressos na linha de seguinte.

            O '\n' tambem age como "flush". A operacao flush transfere
            imediatamente os caracteres anteriores para o dispositivo de saida; Do contrario somente quando o programa termina
            ou quando o numero de caracteres armazenados estao proximos a exceder a memoria reservada para o transferencia
            de caracteres para o stdout

        EN: the '\n' sequence represents a special "Newline" character. It means that the following characters will be
            printed in the following line.

            The '\n' also triggers the flush operation. Flush transfers all cached characters to the stdout.
            Otherwise, the characters would only be sent to the stdout when the program is ends or when
            for this kind of storage is running out of space.
    */

    printf("flush test");

    sleep(5);

    printf("done\n");
    return 0;
}
#endif
