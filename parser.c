#include "parser.h"
#include "code.h"
#include "SymbolTable.h"
//mi returna il simbolo o decimale xxx del comando attuale, va richiamata solo se commandType=A_COMMAND o L_COMMAND
char* symbol(char line[], char command){    
    int len=strlen(line);
    int offset=0;
    switch(command){
        case 'a':
            offset=-1;
            break;
        case 'l':
            offset=-2;
            break;
        default:
            printf("errore input symbol errato %s", line);
            exit(-1);
    }
    char *etichetta=(char*)malloc(sizeof(char) * len);
    for(int i=1; i<len; i++) {
        etichetta[i - 1] = line[i];
    }
    etichetta[len+offset]='\0';
    return etichetta;
}


//mi ritorna A_COMMAND, C_COMMAND, L_COMMAND, qundi con questo posso sapere il tipo di comando
int commandType(char line[]){     // 1=A_COMMAND, 2=L_COMMAND, 3=C_COMMAND
    char firstchar=line[0];
    if (firstchar == '@') {
        return 1;
    } else if (firstchar == '(') {
        return 2;
    } else if (firstchar == 'A' || firstchar == 'M' || firstchar == 'D' || firstchar=='0') {
        return 3;
    } else {
        printf("comando sbagliato, errore sintassi %s",line);
        exit(-1);
    }
}

char* tobinary(int n){
    int maxI = 16;
    char* bin = malloc(sizeof(char) * maxI);
    int i;
    for(i=0;n>0;i++){
        if (i >= maxI) {
            printf("Number too large!");
            exit(-1);
        }
        if (n % 2 == 0) {
            bin[i] = '0';
        } else {
            bin[i] = '1';
        }
        n=n/2;
    }
    bin[i]='\0';
    return bin;
}

void writeBinary(char line[], FILE *copia, FILE *par){
    int endIndex = strcspn(line,"\n\r"); //trova la lunghezza della riga riferendosi al primo \r,\n o \0 che trova
    line[endIndex] = '\0';
    int n=commandType(line);
    char binary[17]="0000000000000000";
    if(n==1){       //nel caso sia a instruction
        char bin[100];
        char tmp2[100];
        char *symbol1 = symbol(line, 'a');
        strcpy(tmp2, symbol1);
        free(symbol1);
        if(tmp2[0]>='0' && tmp2[0]<='9') {      //etichetta numerica!
            strcpy(bin, tobinary(atoi(tmp2)));
        }else{
            int con= getaddress(tmp2);          // etichetta non numerica!!
            char *binary1 = tobinary(con);
            strcpy(bin, binary1);
            free(binary1);
        }
        int m=strlen(bin);
        for(int i=0; i<m; i++){
            binary[15-i]=bin[i];
        }
        binary[16]='\0';
        fprintf(par,"%s\n",binary);

    }else if(n==3){

        //metto su binary jump
        char* jmp = jump(line);
        for(int i=0; i<3; i++){
            binary[15-i]=jmp[2-i];
        }
        free(jmp);
        //metto su binary dest
        char* dst = dest(line);
        for(int i=0; i<3; i++){
            binary[12-i]=dst[2-i];
        }
        free(dst);
        //metto su binary comp
        char *cmp = comp(line);
        for(int i=0; i<7; i++){
            binary[9-i]=cmp[6-i];
        }
        free(cmp);
        binary[0]='1';
        binary[1]='1';
        binary[2]='1';
        binary[16]='\0';
        fprintf(par,"%s\n", binary);
    }
}

void cleanfile(FILE *asmbly, FILE *copia){
    if(asmbly!=NULL) {
        char line[1000];
        char nocomments[1000];
        //legge una riga
        rewind(copia);
        while (fgets(line, sizeof line, asmbly) != NULL) {
            int endIndex = strcspn(line,"\n\r"); //trova la lunghezza della riga riferendosi al primo \r,\n o \0 che trova
            line[endIndex] = '\0'; //mettiamo lo \0 per eliminare eventuali schifezze alla fine della stringa
            int i = 0;
            int j = 0;
            while (i <= endIndex) {     //leggo tutti i caratteri fino allo \0 finale
                char currentChar = line[i];
                char nextChar;
                if (i < endIndex) {
                    nextChar = line[i + 1];
                } else {      //se siamo gia' all'ultimo carattere, non prendiamo il carattere dopo e facciamo finta ci sia un altro \0 alla fine
                    nextChar = '\0';
                }
                if (currentChar == '/' && nextChar == '/') {
                    nocomments[j] = '\0';
                    break;  //se mi trova i commenti smette di leggere e finisce
                }
                //salta lo spazio
                if (currentChar != ' ') {
                    nocomments[j] = currentChar;
                    j++;
                }
                i++;
            }
            //salvo la riga sul file, solo se non e' vuota dopo la pulizia
            if(nocomments[0]!='\0' && nocomments[0]!=' '){
                fprintf(copia, "%s\n", nocomments);
            }
        }
    }
    else{
        perror("errore");
    }
}

void parafrasi(FILE *copia, FILE *par){
    if(copia!=NULL) {
        rewind(copia);
        char line[1000];
        //legge una riga
        while (fgets(line, sizeof line, copia) != NULL) {
            writeBinary(line, copia, par);
        }

    }
    else{
        perror("errore");
    }
}




void initializer(FILE *asmbly, FILE *hack){
    FILE *copia=fopen("nospace.txt","w+");
    cleanfile(asmbly, copia);
    reader(copia);
    parafrasi(copia, hack);
    fclose(copia);
    fclose(asmbly);
    fclose(hack);
    remove("nospace.txt");
}
