

#ifndef INC_48_ASSEMBLER_PARSER_H
#define INC_48_ASSEMBLER_PARSER_H

#endif //INC_48_ASSEMBLER_PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


// mi returna il simbolo o decimale xxx del comando attuale, viene richiamata solo se commandType=A_COMMAND o L_COMMAND
char* symbol(char[], char);

// mi ritorna A_COMMAND, C_COMMAND, L_COMMAND, qundi con questo posso sapere il tipo di comando
// 1=A_COMMAND, 2=L_COMMAND, 3=C_COMMAND
int commandType(char[]);


// mi pulisce il file da spazi, a capo e commenti
// chiama commandType e da li' prosegue il resto del programma
void cleanfile(FILE *, FILE *);

void parafrasi(FILE *, FILE*);


// scrive il codice binario all'interno del file nuovo
void writeBinary(char [], FILE *, FILE *);

//apre il file e ne crea uno nuovo pronto per scriverci sopra
void initializer(FILE *, FILE *);