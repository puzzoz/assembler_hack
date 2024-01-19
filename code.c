
#include "code.h"
#include "parser.h"


void manipulate_mnemonic(char mnemonic[], int indexes[], int arr_lenght){
    for(int i=0; i<arr_lenght; i++){
        mnemonic[indexes[i]]='1';
    }
}

//mi returna il dest, da chiamare solo quando commandType=C_COMMAND
char* dest(char line[]) {        //messo temporaneamente come void, tecnicamente devo returnare una stringa
    int len = strcspn(line, "=;");
    char mnemonic[] = "000";
    if(strchr(line,'=')!=NULL){
        if (len==1) {
            if (line[0] == 'M') {
                mnemonic[2] = '1';      //dest=M
            } else if (line[0] == 'D') {
                mnemonic[1] = '1';      //dest=D
            } else if (line[0] == 'A') {
                mnemonic[0] = '1';      //dest=A
            }
        } else if (len == 2) {
            if (line[0] == 'M' && line[1]=='D') {
                mnemonic[1] = '1';
                mnemonic[2] = '1';      //dest=MD
            } else if (line[0] == 'A' && line[1]=='M') {
                mnemonic[0] = '1';
                mnemonic[2] = '1';      //dest=AM
            } else if (line[0] == 'A' && line[1]=='D') {
                mnemonic[0] = '1';
                mnemonic[1] = '1';      //dest=AD
            }
        }else if (len >= 3){
            if(line[0]=='A' && line[1]=='M' && line[2]=='D'){
                mnemonic[0]='1';
                mnemonic[1]='1';
                mnemonic[2]='1';        //dest=AMD
            }
        }
    }
    //se non trova niente assume dest=null

    char *ret = malloc(sizeof(mnemonic));
    strcpy(ret, mnemonic);
    return ret;
}

//28 possibilita' con questo codice assumo che sia corretta la sintassi
//non mi segnala un errore se presente
char* comp(char line[]){
    int len=strcspn(line, "\n;")-1;
    //a      c1      c2     c3       c4      c5       c6
    char mnemonic[] = "0000000";
    switch(line[len]){
        case '0':{
            int indexes[3]={1,3,5};
            manipulate_mnemonic(mnemonic,indexes, 3);  // comp=0
            break;
        }
        case '1':{
            if(line[len-1]=='-'){
                if(line[len-2]=='D'){
                    int indexes[3]={3,4,5}; // comp=D-1
                    manipulate_mnemonic(mnemonic,indexes,3);
                }else if(line[len-2]=='A' || line[len-2]=='M'){
                    int indexes[3]={1,2,5};
                    manipulate_mnemonic(mnemonic,indexes,3);  // comp=A-1
                    if(line[len-2]=='M'){
                        mnemonic[0]='1';    // comp=M-1
                    }
                }else{
                    int indexes[4]={1,2,3,5};
                    manipulate_mnemonic(mnemonic,indexes,4);  // comp=-1
                }
            }else if(line[len-1]=='+'){
                if(line[len-2]=='D'){
                    int indexes[5]={2,3,4,5,6};
                    manipulate_mnemonic(mnemonic,indexes,5);      // comp=D+1
                }else if(line[len-2]=='A' || line[len-2]=='M'){
                    int indexes[5]={1,2,4,5,6};
                    manipulate_mnemonic(mnemonic,indexes,5);      // comp=A+1
                    if(line[len-2]=='M'){
                        mnemonic[0]='1';            // comp=M+1
                    }
                }
            }else{
                int indexes[6]={1,2,3,4,5,6};   // comp=1
                manipulate_mnemonic(mnemonic,indexes, 6);
            }
        }
            break;
        case 'D':
            if(line[len-1]=='!'){
                int indexes[3]={3,4,6};
                manipulate_mnemonic(mnemonic,indexes,3);  // comp=!D
            }else if(line[len-1]=='-'){
                if(line[len-2]=='A' || line[len-2]=='M'){
                    int indexes[3]={4,5,6};
                    manipulate_mnemonic(mnemonic,indexes,3);  // comp=D-A
                    if(line[len-2]=='M'){
                        mnemonic[0]='1';    // comp=D-M;
                    }
                }else{
                    int indexes[4]={3,4,5,6};
                    manipulate_mnemonic(mnemonic,indexes, 4);  // comp=-D
                }
            }else{
                int indexes[2]={3,4};
                manipulate_mnemonic(mnemonic,indexes, 2);  // comp=D
            }
            break;
        case 'A':
        case 'M':
            if(line[len]=='M'){
                mnemonic[0]='1';
            }
            if(line[len-1]=='!'){
                int indexes[3]={1,2,6};
                manipulate_mnemonic(mnemonic,indexes, 3);  // comp=!A || comp=!M
            }else if(line[len-1]=='-'){
                if(line[len-2]=='D'){
                    int indexes[3]={2,5,6};
                    manipulate_mnemonic(mnemonic,indexes, 3);  // comp=D-A || comp=D-M
                }else if(line[len-2]=='&'){
                    break;      //comp=D&A || comp=D&M
                }else{
                    int indexes[4]={1,2,5,6};
                    manipulate_mnemonic(mnemonic,indexes, 4);  // comp=-A || comp=-M
                }
            }else if(line[len-1]=='+'){
                mnemonic[5]='1';    // comp=D+A || comp=D+M
            }else if(line[len-1]=='|'){
                int indexes[3]={2,4,6};
                manipulate_mnemonic(mnemonic,indexes, 3);      // comp=D|A || comp=D|M
            }else if(line[len-1]=='&') {
                break;      // comp=D&A || comp=D&M
            }else{
                int indexes[2]={1,2};
                manipulate_mnemonic(mnemonic,indexes, 2);  // comp=A || comp=M
            }
    }
    char *ret = malloc(sizeof(mnemonic));
    strcpy(ret, mnemonic);
    return ret;
}


//chiamare solo quando commandType=C_COMMAND,(8 possibilita'..);
char* jump(char line[]){
    int len=strlen(line);
    len--;
    char mnemonic[] = "000";
    if(line[len]=='T'){
        if(line[len-1]=='G'){
            mnemonic[2]='1';    //jump=JGT
        }else if(line[len-1]=='L'){
            mnemonic[0]='1';    //jump=JLT
        }
    }else if(line[len]=='Q'){
        mnemonic[1]='1';        //jump=JEQ
    }else if(line[len]=='E') {
        if (line[len - 1] == 'G') {
            mnemonic[2] = '1';
            mnemonic[1] = '1';  //jump=JGE
        } else if (line[len - 1] == 'N') {
            mnemonic[2] = '1';
            mnemonic[0] = '1';  //jump=JNE
        } else if (line[len - 1] == 'L') {
            mnemonic[0] = '1';
            mnemonic[1] = '1';  //jump=JLE
        }
    }else if(line[len]=='P'){
        mnemonic[0] = '1';
        mnemonic[1] = '1';
        mnemonic[2] = '1';  //jump=JMP
    }
    //se non trova niente assume il caso null

    char *ret = malloc(sizeof(mnemonic));
    strcpy(ret, mnemonic);
    return ret;
}
