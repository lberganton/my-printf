# Uma Implementação de Printf

Uma implementação das funções `printf`, `fprintf`, `vprintf` e `vfprintf`

## Opções de Formatação

Há um suporte as principais flags e especificadores de formato do `printf` padrão.

Um especificador de formato completo segue a seguinte ordem:

```txt
%[flags][largura][.precisão][modificador de tipo]código
```

**Especificadores de Formato Suportados:**

| Código  | Descrição                                   |
| :-----: | :-----------------------------------------  |
| %i      | Para números decimais                       |
| %d      | Para números decimais                       |
| %u      | Para números decimais sem sinal             |
| %x      | Para números hexadecimais (Minúsculo)       |
| %X      | Para números hexadecimais (Maiúsculo)       |
| %o      | Para números octais                         |
| %b      | Para números binários (Minúsculo)           |
| %B      | Para números binários (Maiúsculo)           |
| %f      | Para números de ponto flutuante (Minúsculo) |
| %F      | Para números de ponto flutuante (Maiúsculo) |
| %c      | Para caracteres ASCII                       |
| %s      | Para strings ASCII                          |
| %%      | Para o caractere '%'                        |

**Flags Suportadas:**
| Flag                    | Descrição                                   |
| :--------------------:  | :-----------------------------------------  |
| -                       | Alinhar à Esquerda                          |
| +                       | Forçar Sinal                                |
| (Espaço)                | Preencher com Espaços                       |
| 0                       | Preencher com Zeros                         |
| #                       | Forma Alternativa                           |
| (Inteiro)               | Largura                                     |
| .(Inteiro)              | Precisão                                    |
| *                       | Largura passada como argumento              |
| .*                      | Precisão passada como argumento             |

**Modificadores de Tipo Suportados:**
| Modificador             | Com Sinal     | Sem Sinal             |
| :--------------------:  | :------------ | :-------------------- |
| hh                      | char          | usigned char          |
| h                       | short int     | short unsigned int    |
| l                       | long int      | long unsigned int     |
| ll                      | long long int | long long usigned int |

## Modo de Usar

Após montar a biblioteca inclua-a com:

```c
#include "myprintf.h"
```

As funções são equivalentes as definidas em `stdio.h`

```c
int my_vfprintf(FILE *file, const char *format, va_list args);

int my_fprintf(FILE *file, const char *format, ...);

int my_vprintf(const char *format, va_list args);

int my_printf(const char *format, ...);
```

Também são fornecidas duas macros:

| Nome                        | Descrição                                        | Valor Padrão |
| :-------------------------: | :----------------------------------------------: | :----------: |
| MY_PRINTF_DEFAULT_PRECISION | A precisão padrão dos números de ponto flutuante | 6            |
| MY_PRINTF_MAX_PRECISION     | A precisão máxima dos números de ponto flutuante | 16           |

Se uma precisão maior que `MY_PRINTF_MAX_PRECISION` for passada a `printf`, o número será truncado.

Essas macros podem ser configuradas, para isso basta defini-las antes de incluir `myprintf.h`

```c
#define MY_PRINTF_DEFAULT_PRECISION 3
#define MY_PRINTF_MAX_PRECISION 8
#include "myprintf.h"
```

## Como Montar

Para montar o projeto é necessário um compilador C, de preferência o GCC e um leitor de Makefile.

Então basta usar o comando:

```bash
make
```

Após isso a biblioteca será montada, resultando no arquivo objeto `myprintf.o`

Quando for usa-la compile seu programa linkando com este arquivo.

**Exemplo:**

```bash
gcc seu_arquivo.c myprintf.o
```
