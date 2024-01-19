
#include "code.h"
#include "parser.h"
#include "SymbolTable.h"
int main(int argc, char *argv[]) {
    if(argc >= 2) {
        FILE *asmfile;
        asmfile = fopen(argv[1], "r");

        if (asmfile == NULL) {
            printf("Errore in apertura del file da leggere: %s\n", argv[1]);
            exit(-1);
        }
        FILE *hackfile;
        char filename[64];

        strcpy(filename,argv[1]);

        filename[strlen(filename)-3] = '\0';
        strcat(filename, "hack"); //concatenazione

        hackfile = fopen(filename, "w");
        initializer(asmfile, hackfile);
    }else{
        printf("Usage: %s [filename]\n", argv[0]);
    }
    return 0;
}
