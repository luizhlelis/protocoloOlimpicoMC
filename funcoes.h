#ifndef _FUNCOES_H
#define _FUNCOES_H

# define MAX 1000

typedef struct {
    int Key; //A chave corresponde ao tempo do atleta em ms
} Record;

typedef struct {
    Record Item[MAX + 1];  //Vetor onde os tempos ficam armazenados
    int n;  //Tamanho da tabela
} Table;

//Cabecalhos

void InsertTable(Record Reg, Table *T);

int BinarySearch(int x, Table *T);

void InsertionSort(Record *A, int n);

int ChangeToMilisec(char MsgRead[], int StringSize);

void ReadAndEditStr(char *Str, int StringSize);

#endif // _FUNCOES_H
