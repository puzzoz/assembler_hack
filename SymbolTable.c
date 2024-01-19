#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>

struct predefined{
    char symbol[100];
    int address;
};
struct tablepairs pairs[10000]={'\0'};
struct Lcommands lcomandi[1000]={'\0'};

struct predefined pre[23]={
    "SP", 0, "LCL", 1, "ARG", 2, "THIS", 3,
    "THAT", 4, "R0", 0, "R1", 1, "R2", 2,
    "R3", 3, "R4", 4, "R5", 5, "R6", 6,
    "R7",7,"R8", 8, "R9", 9, "R10", 10,
    "R11", 11, "R12", 12, "R13", 13, "R14", 14,
    "R15", 15, "SCREEN", 16384, "KBD", 24576
};


int firstaddress=16;

void table_initializer(){
    for(int i=0; i<23; i++){
        strcpy(pairs[i].symbol, pre[i].symbol);
        pairs[i].address=pre[i].address;
    }
}

bool contains(char et[]){
    int i=0;
    while(i<999){
        if(strcmp(pairs[i].symbol, et)==0){
            return true;
        }
        i++;
    }
    return false;
}

bool containsL(char et[]){      //controllo che il comando L sia contenuto nella tabella
    int i=0;
    while(i<999){
        if(strcmp(lcomandi[i].symbol, et)==0){
            return true;
        }
        i++;
    }
    return false;
}

void insertL(char et[], int n){     //n e' la riga a cui siamo
    int i=0;
    while(strcmp(lcomandi[i].symbol, "\0")!=0){      //mi scorre finche' non trova un buco all'interno della tabella
        i++;
    }
    strcpy(lcomandi[i].symbol, et);
    lcomandi[i].linea=n;

}

void insert(char et[]){
    int i=0;
    int j=0;
    while(strcmp(pairs[i].symbol, "\0")!=0 && i<1000){      //mi scorre finche' non trova un buco all'interno della tabella
        i++;
    }
    if(containsL(et)){
        strcpy(pairs[i].symbol, et);
        while(strcmp(lcomandi[j].symbol, et)!=0){      //mi scorre finche' non trova un buco all'interno della tabella
            j++;
        }
        pairs[i].address=lcomandi[j].linea;
    }else{
        strcpy(pairs[i].symbol, et);
        pairs[i].address=firstaddress;      // appena lo trova me lo piazza dentro il primo address disponibile
        firstaddress++;                 // aumenta il valore di address per la prossima volta che ci serve
    }
}

int getaddress(char et[]) {
    int i = 0;
    int endIndex = strcspn(et,"\n\r");
    et[endIndex]='\0';
    while (i < 999) {
        if (strcmp(pairs[i].symbol, et) == 0) {
            return pairs[i].address;
        }
        i++;
    }
    printf("errore, simbolo non e' presente in tabella");
    exit(-1);
}


void reader(FILE *asmbly){
    table_initializer();
    rewind(asmbly);
    if(asmbly!=NULL) {
        int lin=0;
        char line[1000];
        while(fgets(line, sizeof(line), asmbly)!=NULL){     //faccio una prima passata per cercare L commands
            int endIndex = strcspn(line,"\n\r"); //trova la lunghezza della riga riferendosi al primo \r,\n o \0 che trova
            line[endIndex] = '\0'; //mettiamo lo \0 per eliminare eventuali schifezze alla fine della stringa
            int n= commandType(line);
            if(n==2){       //ora mi prendo l'etichetta, controllo non sia nella tabella
                char comando[100];
                char *symbol1 = symbol(line, 'l');
                strcpy(comando, symbol1);  //ottengo l'etichetta
                free(symbol1);
                if(containsL(comando)){
                    printf("la tabella puo' contenere solo 1 tipo di Lcommand per tipo\n");
                    exit(-1);
                }else{
                    insertL(comando, lin--);
                }
            }
            lin++;        //aumento i per aumentare il numero della riga
        }
        rewind(asmbly);
        //legge una riga
        while (fgets(line, sizeof line, asmbly) != NULL) {
            int endIndex = strcspn(line,"\n\r"); //trova la lunghezza della riga riferendosi al primo \r,\n o \0 che trova
            line[endIndex] = '\0'; //mettiamo lo \0 per eliminare eventuali schifezze alla fine della stringa
            int n= commandType(line);
            if(n==1){           //command type returna 1 in caso ci sia una a_command
                char comando[100];
                char *symbol1 = symbol(line, 'a');
                strcpy(comando, symbol1);  //ottengo l'etichetta
                free(symbol1);
                if(comando[0]<'0' || comando[0]>'9'){     //le etichette date da noi NON iniziano con i numeri!
                    if(!contains(comando)){
                        insert(comando);
                    }
                }
            }
        }
    }
    else{
        perror("errore");
    }
}

