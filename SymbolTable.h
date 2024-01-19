
#ifndef INC_48_ASSEMBLER_SYMBOLTABLE_H
#define INC_48_ASSEMBLER_SYMBOLTABLE_H

#endif //INC_48_ASSEMBLER_SYMBOLTABLE_H
#include "code.h"
#include "parser.h"


struct tablepairs{      //tabella che raccoglie etichetta e indirizzi di memoria
    char symbol[100];
    int address;
};

struct Lcommands{      //mi permette di raggruppare i miei Lcommands
    char symbol[100];
    int linea;
};

//inserisco i valori nella tabella
void insert(char []);

//in base al nome dell'etichetta questo mi cerca l'indirizzo e lo returna
int getaddress(char []);

//crea la tabella
void table_initializer();

//mi fa il file
void reader(FILE *);

//la symbol table contiene il simbolo che stiamo cercando??
bool contains(char []);

//la nostra tabella ha la nostra etichetta Lcommand??
bool containsL(char []);

//inserisce nella tabella il comando L nel caso ne trovi uno, e salva il suo indirizzo
void insertL(char[], int);
