//kotopoulos stergios AM 1867,monastirli chrysanthi AM 1716

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3

#define MIN 0
#define MAX 1

struct state{
    char board[N][N+1];
    int player;  //poios paiktis paizei
    int numberOfChilds; //poia paidia eftiaksa,na ftiaksw ola ta pithana paidia,16
    int winner;
    int value;
    int bestChild;
    struct state **childs;
};

struct state *stateNode(char board[N][N+1]); //sinartisi pou dimiourgei dentro, apo askisi1
void create(struct state *node);  //dimiourgei min-max dentro:kaleitai anaddromika
void findValue(struct state *node);
void play(struct state *node); //paizei to paixnidi

int main(){
    char board[N][N+1];
    int i, j;
    struct state *node;
    
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            board[i][j] = '-';  //to keno to simbolisoume me paula
        }
        board[i][j] = '\0';
    }
    board[1][0] = 'O';
    node = stateNode(board);
    (*node).player = MAX;
    create(node);
    findValue(node);
    play(node);
}

void create(struct state *node){
    int i,j;
    struct state *temp;
    
    for(i = 0; i < N; i++){
        if(strcmp((*node).board[i], "SOS") == 0){
            (*node).winner = !(*node).player;
            if((*node).winner == MAX){
                (*node).value = 100;
            }
            else{
                (*node).value = -100;
            }
            return;
        }
    }
    
    for(i = 0; i < N; i++){
        if((*node).board[0][i] == 'S'){
            if((*node).board[1][i] == 'O'){
                if((*node).board[2][i] == 'S'){
                    (*node).winner = !(*node).player;
                    if((*node).winner == MAX){
                        (*node).value = 100;
                    }
                    else{
                        (*node).value = -100;
                    }
                    return;
                }
            }
        }
    }
    
    if((*node).board[0][0] == 'S'){
        if((*node).board[1][1] == 'O'){
            if((*node).board[2][2] == 'S'){
                (*node).winner = !(*node).player;
                return;
            }
        }
    }
    
    if((*node).board[0][2] == 'S'){
        if((*node).board[1][1] == 'O'){
            if((*node).board[2][0] == 'S'){
                (*node).winner = !(*node).player;
                return;
            }
        }
    }
        
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if((*node).board[i][j] == '-'){ //paula einai to keno
                (*node).board[i][j] = 'S';
                temp = stateNode((*node).board);
				//auksanoume tis theseis kata 1 me realloc
                (*node).childs = (struct state **)realloc((*node).childs, ((*node).numberOfChilds + 1)*sizeof(struct state*));
                (*node).childs[(*node).numberOfChilds] = temp;
                (*node).numberOfChilds++;
                (*temp).player = !(*node).player;
				//gia na epikeinw kombo den prepei na exei sos
				//prepei na kalesoume tin create gia na baloume temp,anadromika, epekteinoume gia ta paidia
                create(temp);
                
				//to ksanakanoume gia omikron
                (*node).board[i][j] = 'O';
                temp = stateNode((*node).board);
                (*node).childs = (struct state **)realloc((*node).childs, ((*node).numberOfChilds + 1)*sizeof(struct state *));
                (*node).childs[(*node).numberOfChilds] = temp;
                (*node).numberOfChilds++;
                (*temp).player = !(*node).player;
                
                (*node).board[i][j] = '-';
                
                create(temp);
            }
        }
    }
    if((*node).numberOfChilds == 0){
        (*node).value = 0;
    }
}

//tha prepei na brei tin mikroteri timi metaksi twn paidiwn an einai i min
//k tn megaliteri metaksi twn paidiwn an einai max
void findValue(struct state *node){
    int i;
    if((*node).numberOfChilds == 0)
        return;
    
    if((*node).player == MAX){
        (*node).value = (*(*node).childs[0]).value; //i prwti timi tou paidiou
        (*node).bestChild = 0;
        for(i = 0; i < (*node).numberOfChilds; i++){
            if((*node).value < (*(*node).childs[0]).value){
                (*node).value = (*(*node).childs[0]).value;
                (*node).bestChild = i;
            }
        }
    }
    else if((*node).player == MIN){
        (*node).value = (*(*node).childs[0]).value;
        (*node).bestChild = 0;
        for(i = 0; i < (*node).numberOfChilds; i++){
            if((*node).value > (*(*node).childs[0]).value){
                (*node).value = (*(*node).childs[0]).value;
                (*node).bestChild = i;
            }
        }
    }
}

struct state *stateNode(char board[N][N+1]){
    struct state *temp;
    int i;
    temp = (struct state *)malloc(sizeof(struct state));
    if(temp == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }
    
    for(i = 0; i <  N; i++){
        strcpy((*temp).board[i], board[i]);
    }
    (*temp).childs = NULL;
    (*temp).numberOfChilds = 0;
    
    return temp;
}

//kathe fora paw se ena ena ta epipeda
//stamatw otan den exw pou na paw
void play(struct state *node){
    
    struct state *temp = node;
    int player = MAX; //arxika paizei to programma
    int i, j;
    char ch;
    int col, row;
    
    while(1){
        for(i = 0; i < N; i++){
            printf("%s\n", (*temp).board[i]);
        }
        if(player == MAX){
            printf("MAX plays\n"); 
            temp = (*temp).childs[(*temp).bestChild];
        }
        else{
            printf("Give row, column, letter seperated with space: ");
            scanf("%d %d %c", &row, &col, &ch);
            for(i = 0; i < (*temp).numberOfChilds; i++){
                if((*(*temp).childs[i]).board[row][col] == ch){
                    temp = (*temp).childs[i];  
                }
            }
        }
        player = !player;
        
        if((*temp).numberOfChilds == 0){ //stamatame ekei pou exoume miden paidia tote exoume nikiti i isopalia
            for(i = 0; i < N; i++){
                printf("%s\n", (*temp).board[i]);
            }
            if((*temp).value == -100){
                printf("MIN wins!!!\n");
            }
            else if((*temp).value == 100){
                printf("MAX wins!!!\n");
            }
            else{
                printf("Draw!!!\n");
            }
            break;  //teleiwnei to paixnidi
        } 
    }
}


