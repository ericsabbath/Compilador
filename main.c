#include <stdio.h>
#include <stdlib.h>
#include "analex.h"

int main(int argc, char *argv[])
{
    num_linhas = 1;
    ind_lt = 0;
    aux_fim = 0;

    TabelaPR();
    TabelaSN();

    arquivo();

    token tk;

    while(1)
    {
      tk = analex();
      imprime(tk);
    }

    prog();
    system("PAUSE");
    return 0;
}
