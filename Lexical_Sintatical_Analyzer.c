#define RESERVADA 0
#define ID 1
#define NUM 2
#define OP 3
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<locale.h>
typedef struct Lexema
{
    char Palavra[20];
    int linha, id;
    struct Lexema *link;
} token;

typedef struct
{
    token *inicio;
    int quant;
} LD;

typedef struct
{
    token *inicio;
    int quant;
} Pilha;

void DefinirPilha(Pilha *P)
{
    P->inicio = NULL;
    P->quant = 0;
}

void DefinirList(LD *L)
{
    L->inicio = NULL;
    L->quant = 0;
}

int InserirPilha(Pilha *Pil, char P[])
{
    token *n;
    token *p;
    p=Pil->inicio;
    n=malloc(sizeof(token));
    n->link=p;
    strcpy(n->Palavra,P);
    Pil->inicio=n;
    Pil->quant++;
}

token RemovePilha(Pilha *P)
{
    if(P->quant>=1)
    {
        token *t;
        t=P->inicio;
        P->inicio=t->link;
        P->quant--;
        return (*t);
    }
    if(P->quant==0)
    {
        token *t;
        t=P->inicio;
        return(*t);
    }
    else
    {
        DefinirPilha(P);
    }
}

int InserirList(LD *L, char P[], int idp, int lp)
{
    token *n;
    token *p;
    p=L->inicio;
    n=malloc(sizeof(token));
    if (L->quant==0)
    {
        n->link=NULL;
        strcpy(n->Palavra,P);
        n->id=idp;
        n->linha=lp;
        L->inicio=n;
        L->quant++;
    }
    else
    {
        while (p->link!=NULL)
        {
            p=p->link;
        }
        p->link=n;
        n->link=NULL;
        n->id=idp;
        n->linha=lp;
        strcpy(n->Palavra,P);
        L->quant++;
    }
}

void ImprimePilha(Pilha *P)
{
    token *t;
    t = P->inicio;
    while(t != NULL)
    {

        printf("Palavra:'%s'", t->Palavra);
        t=t->link;
        printf("\n");
    }
}


void ImprimeList(LD *L)
{
    token *t;
    t = L->inicio;
    while(t != NULL)
    {
        printf("Palavra:'%s' Tipo: %d Linha: %d\n", t->Palavra, t->id, t->linha);
        t=t->link;
        printf("\n");
    }
}

int is_Reservada(char Palavra[]) ///inicio -> VERIFICAÇAO DA PALAVRA RESRVADO
{
    char Reservadas[13][10] = {"inicio", "fim", ";", ":", ":=", ",", "leia", "escreva", "num", "id", "int", "(", ")"};
    int i, is=0;
    for(i = 0; i < 13; i++){
        if(strcmp(Reservadas[i], Palavra) == 0){
            is=1;
            break;
        }
    }
    return is;
                            ///FIM -> VERIFICAÇAO DA PALAVRA RESERVADA
}

LD Lexical(FILE *Cod, LD L) ///inicio ANALISADOR LEXICO
{
    char Letra_A, Palavra_A[20], Operadores[]="+-*";
    int i=0,j=0, flag=0, linha=1;
    while((Letra_A = fgetc(Cod)) != EOF)  /// RETIRA-SE O PRIMEIRO CHAR DO ARQUIVO
    {
        if(Letra_A=='/') /// inicio -> ELIMINAÇÃO DE COMENTÁRIO
        {
            if((Letra_A = fgetc(Cod))=='*')
            {
                Letra_A=fgetc(Cod);
                if(Letra_A=='\n')
                    linha++;
                while(Letra_A!= '*')
                {
                    Letra_A=fgetc(Cod);
                    if(Letra_A=='\n')
                        linha++;
                }
                Letra_A=fgetc(Cod);
                if(Letra_A=='\n')
                    linha++;
                if(Letra_A=='/'){
                    Letra_A=fgetc(Cod);
                    if(Letra_A=='\n')
                    linha++;}
                else
                {
                    printf("ERRO LEXICO LINHA %d\n",linha);
                }
            }
        }
                         /// FIM -> ELIMINAÇÃO DE COMENTÁRIO

        for(i = 0; i < 3; ++i)   /// inicio -> VERIFICA SE O CHAR É OPERAÇÃO
        {
            if(Letra_A == Operadores[i])
            {
                Palavra_A[j++]=Letra_A;
                flag=1;
                Palavra_A[j++]='\0';
                InserirList(&L, Palavra_A, OP, linha);
                j=0;
            }
        }
                                /// FIM -> VERIFICA SE O CHAR É OPERAÇÃO

        if(Letra_A==';')        /// inicio -> VERIFICA SE O CHAR É UNICO E PALAVRA RESERVADA
        {
            Palavra_A[j++]=Letra_A;
            flag=1;
            Palavra_A[j++]='\0';
            InserirList(&L, Palavra_A, RESERVADA, linha);
            j=0;
        }
        if(Letra_A==':')
        {
            Palavra_A[j++]=Letra_A;
            Letra_A=fgetc(Cod);
            if(Letra_A=='=')
            {
                Palavra_A[j++]=Letra_A;
            }
            Palavra_A[j++]='\0';
            j=0;
            if(is_Reservada(Palavra_A)==1)
            {
                InserirList(&L, Palavra_A, RESERVADA, linha);
                flag=1;
            }
        }
        if(Letra_A=='(')
        {
            j=0;
            Palavra_A[j++]=Letra_A;
            Palavra_A[j++]='\0';
            InserirList(&L, Palavra_A, RESERVADA, linha);
            flag=1;
            j=0;
        }
        if(Letra_A==')')
        {
            j=0;
            Palavra_A[j++]=Letra_A;
            Palavra_A[j++]='\0';
            InserirList(&L, Palavra_A, RESERVADA, linha);
            flag=1;
            j=0;
        }
        if(Letra_A==',')
        {
            Palavra_A[j++]=Letra_A;
            Palavra_A[j++]='\0';
            InserirList(&L, Palavra_A, RESERVADA, linha);
            flag=1;
            j=0;
        }
                                /// FIM -> VERIFICA SE O CHAR É UNICO E PALAVRA RESERVADA

        if(isalnum(Letra_A))    ///VERIFICA SE O CHAR É ALPHANUMERICAL
        {
            if(isdigit(Letra_A))  ///VERIFICA SE É DIGITO
            {
                int k=0, l=1;
                while((Letra_A!=EOF)&&(Letra_A!=' ')&&(Letra_A!='\n')) ///ENCHE O VETOR COM OS CHAR ATÉ ENCONTRAR UM ESPAÇO
                {
                    if((Letra_A!='\n')&&(Letra_A!=' '))
                    {
                        Palavra_A[j++]=Letra_A;
                    }
                    Letra_A=fgetc(Cod);
                }
                Palavra_A[j++]='\0';
                while(k!=j)                     ///VERIFICA SE TODOS OS CHAR SAO DO TIPO REQUISITADO E SE O VETOR ESTA PREENCHIDO CORRETAMENTE
                {
                    if(isdigit(Palavra_A[k]))
                    {
                        l++;
                    }
                    k++;

                }
                if(j==l)                        ///SE TUDO OCORREU NORMALMENTE INSERE NA LISTA.
                {
                    InserirList(&L, "num", NUM,linha) ;
                    flag=1;
                }
                else
                {
                    printf("ERRO NUMERICO LINHA %d\n", linha); ///CASO NÃO, ERRO!
                }
                j=0;
            }
            else
            {
                int k=0, l=1;
                while((Letra_A!=EOF)&&(Letra_A!=' ')&&(Letra_A!='\n'))  ///INSERE OS CHAR NO VETOR ATÉ ENCORAR ESPAÇO OU QUEBRA DE LINHA
                {
                    if((Letra_A!='\n')&&(Letra_A!=' '))
                    {
                        Palavra_A[j++]=Letra_A;
                    }
                    Letra_A=fgetc(Cod);
                }
                Palavra_A[j++]='\0';
                while(k!=j)
                {
                    if(isalnum(Palavra_A[k]))               ///VERIFICA SE TODOS OS CHAR SAO DO TIPO REQUISITADO E SE O VETOR ESTA PREENCHIDO CORRETAMENTE
                    {
                        l++;
                    }
                    k++;
                }
                if(j==l)
                {
                    if(is_Reservada(Palavra_A) == 1)
                    {
                        InserirList(&L, Palavra_A, RESERVADA, linha);                ///SE TUDO OCORREU NORMALMENTE INSERE NA LISTA.
                        flag=1;
                    }
                    else
                    {
                    InserirList(&L, "id", ID, linha);                           ///SE TUDO OCORREU NORMALMENTE INSERE NA LISTA.
                    flag=1;
                    }
                }
                else
                {
                    printf("ERRO DE IDENTIFICADOR LINHA %d\n", linha);              ///CASO NÃO, ERRO!
                }
                j=0;
            }
        }
        if(Letra_A=='\n')                                           ///CASO /N INCREMENTA A LINHA
            linha++;
        if((flag!=1)&&(Letra_A != ' ')&&(Letra_A != '\n')&&(Letra_A != EOF))               ///VERIFICA SE TODAS AS FLAGS FORAM SETADAS CORRETAMENTE
            printf("ERRO LEXICO LINHA: %d", linha);
        flag=0;
    }
    return L;
}

                            ///FIM DO ANALISADOR LEXICO


int Sintatical(LD *L, Pilha *P)
{
    token *n;
    n=L->inicio;
    DefinirPilha(P);
    InserirPilha(P, "$");
    InserirPilha(P, "<PROGRAMA>");

    while(n!=NULL || (strcmp(P->inicio->Palavra,"$")!=0))
    {
        if(strcmp(P->inicio->Palavra,n->Palavra)==0)
        {
            RemovePilha(P);
            n=n->link;
        }
        else if(strcmp(P->inicio->Palavra, "<PROGRAMA>")==0 && strcmp(n->Palavra, "inicio")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "fim");
            InserirPilha(P, ";");
            InserirPilha(P, "<COMANDOS>");
            InserirPilha(P, ";");
            InserirPilha(P, "<DECL_SEQ>");
            InserirPilha(P, "inicio");
        }
        else if(strcmp(P->inicio->Palavra, "<DECL_SEQ>")==0 && strcmp(n->Palavra, "int")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<DECL>");
            InserirPilha(P, ":");
            InserirPilha(P, "int");
        }
        else if(strcmp(P->inicio->Palavra, "<DECL>")==0 && strcmp(n->Palavra, "id")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<DECL'>");
            InserirPilha(P, "id");
        }
        else if(strcmp(P->inicio->Palavra, "<DECL'>")==0 && strcmp(n->Palavra, ";")==0)
        {
            RemovePilha(P);
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDOS>")==0 && strcmp(n->Palavra, "id")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<COMANDOS'>");
            InserirPilha(P, "<COMANDO>");
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDO>")==0 && strcmp(n->Palavra, "id")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP1>");
            InserirPilha(P, ":=");
            InserirPilha(P, "id");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1>")==0 && strcmp(n->Palavra, "num")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP1'>");
            InserirPilha(P, "<EXP2>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2>")==0 && strcmp(n->Palavra, "num")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP2'>");
            InserirPilha(P, "<EXP3>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP3>")==0 && strcmp(n->Palavra, "num")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "num");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2'>")==0 && strcmp(n->Palavra, ";")==0)
        {
            RemovePilha(P);
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1'>")==0 && strcmp(n->Palavra, ";")==0)
        {
            RemovePilha(P);
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDOS'>")==0 && strcmp(n->Palavra, ";")==0)
        {
            if((strcmp(n->link->Palavra,"leia")==0) || (strcmp(n->link->Palavra,"escreva")==0) || (strcmp(n->link->Palavra,"id")==0))
            {
                InserirPilha(P, "<COMANDOS'>");
                InserirPilha(P, "<COMANDO>");
                InserirPilha(P, ";");
            }
            else
            {
                RemovePilha(P);
            }
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDO>")==0 && strcmp(n->Palavra, "escreva")==0)
        {
            RemovePilha(P);
            InserirPilha(P, ")");
            InserirPilha(P, "id");
            InserirPilha(P, "(");
            InserirPilha(P, "escreva");
        }
        else if(strcmp(P->inicio->Palavra, "<DECL'>")==0 && strcmp(n->Palavra, ",")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<DECL>");
            InserirPilha(P, ",");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2'>")==0 && strcmp(n->Palavra, "+")==0)
        {
            RemovePilha(P);
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1'>")==0 && strcmp(n->Palavra, "+")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP1'>");
            InserirPilha(P, "<EXP2>");
            InserirPilha(P, "+");
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDO>")==0 && strcmp(n->Palavra, "leia")==0)
        {
            RemovePilha(P);
            InserirPilha(P, ")");
            InserirPilha(P, "id");
            InserirPilha(P, "(");
            InserirPilha(P, "leia");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2'>")==0 && strcmp(n->Palavra, "-")==0)
        {
            RemovePilha(P);
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1'>")==0 && strcmp(n->Palavra, "-")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP1'>");
            InserirPilha(P, "<EXP2>");
            InserirPilha(P, "-");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2'>")==0 && strcmp(n->Palavra, "*")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP2'>");
            InserirPilha(P, "<EXP3>");
            InserirPilha(P, "*");
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDOS>")==0 && strcmp(n->Palavra, "escreva")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<COMANDOS'>");
            InserirPilha(P, "<COMANDO>");
        }
        else if(strcmp(P->inicio->Palavra, "<COMANDOS>")==0 && strcmp(n->Palavra, "leia")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<COMANDOS'>");
            InserirPilha(P, "<COMANDO>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1>")==0 && strcmp(n->Palavra, "id")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP1'>");
            InserirPilha(P, "<EXP2>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2>")==0 && strcmp(n->Palavra, "id")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP2'>");
            InserirPilha(P, "<EXP3>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP3>")==0 && strcmp(n->Palavra, "id")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "id");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1>")==0 && strcmp(n->Palavra, "(")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP1'>");
            InserirPilha(P, "<EXP2>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2>")==0 && strcmp(n->Palavra, "(")==0)
        {
            RemovePilha(P);
            InserirPilha(P, "<EXP2'>");
            InserirPilha(P, "<EXP3>");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP3>")==0 && strcmp(n->Palavra, "(")==0)
        {
            RemovePilha(P);
            InserirPilha(P, ")");
            InserirPilha(P, "<EXP1>");
            InserirPilha(P, "(");
        }
        else if(strcmp(P->inicio->Palavra, "<EXP2'>")==0 && strcmp(n->Palavra, ")")==0)
        {
            RemovePilha(P);
        }
        else if(strcmp(P->inicio->Palavra, "<EXP1'>")==0 && strcmp(n->Palavra, ")")==0)
        {
            RemovePilha(P);
        }
        else{
            return(-1);
        }
    }
    return(0);

}

int main(void)
{
    setlocale(LC_ALL, "Portuguese");
    char Letra_A, Palavra_A[20], Operadores[]="+-*";
    FILE *Cod;
    int i=0,j=0, linha=1, flag=0;
    LD L, LEX;
    Pilha P;
    DefinirPilha(&P);
    DefinirList(&L);
    DefinirList(&LEX);

    Cod=fopen("Cod.txt","r"); /// ABERTURA DO ARQUIVO .TXT

    if(Cod == NULL) /// VERIFICAÇAO DO ARQUIVO VAZIO
    {
        printf("Arquivo Inválido\n");
        return 0;
    }

    LEX=Lexical(Cod, L);
    if(Sintatical(&LEX, &P)==0)
        printf("SUCESSO !!!!!!!!!");
    else
        printf("ERRO SINTÁTICO");
}
