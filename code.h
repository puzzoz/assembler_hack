
#ifndef INC_48_ASSEMBLER_CODE_H
#define INC_48_ASSEMBLER_CODE_H

#endif //INC_48_ASSEMBLER_CODE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


// funzione che mi permette di modificare la posizione x di mnemonic grazie ad un array di interi, i numeri contenuti
// all'interno dell'array sono la posizione di indexes da mettere a '1'
void manipulate_mnemonic(char [], int [], int);

//mi trasforma il dest in binario
char* dest(char []);

//mi trasforma il comp in binario
char* comp(char []);

//mi trasforma il jump in binario
char* jump(char []);