//
//  main.c
//  AlgoritmaAnalizi_Odev-4
//
//  Created by Onat Duman on 17.12.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DATA{
    int id;
    char name[30];
    char surname[30];
    struct DATA *next;
    struct DATA *back;
}data;

void printMatrix(int matrix[50][50], int counter){
    int i, j;
    printf("\n");
    for (i = 0; i<counter; i++) {
        for (j=0; j<counter; j++) {
            printf("%2d |", matrix[i][j]);
        }
        printf("\n");
    }
}

void Split_Data(data* holder,char buff[255],char delimiter[2],int matrix[50][50]){
    char *token;
    token=strtok(buff,delimiter);
    holder->id=atoi(token);
    token=strtok(NULL,delimiter);
    strcpy(holder->name,token);
    token=strtok(NULL,delimiter);
    strcpy(holder->surname,token);
}

data* fileRead(char File_Name[20],char delimiter[2],int matrix[50][50],data* tail, int* counter){
    FILE *fp=fopen(File_Name,"r");
    char buf[255];
    char *token;
    data* head=(data*)malloc(sizeof(data));
    data* curr_data;
    data* prev_data;
    if(fgets(buf,255,fp)!=NULL){
        printf("%s",buf);
        Split_Data(head,buf,delimiter,matrix);
        *counter = 1;
        if(fgets(buf,255,fp)!=NULL){
            printf("%s",buf);
            token=strtok(buf,delimiter);
            while (token != NULL && atoi(token) != 0) {
                matrix[head->id - 1][atoi(token)-1] = 1;
                token=strtok(NULL,delimiter);
            }
        }
    }
    prev_data=head;
    while(fgets(buf,255,fp)!=NULL){
        printf("%s",buf);
        curr_data=(data*)malloc(sizeof(data));
        prev_data->next=curr_data;
        Split_Data(curr_data,buf,delimiter,matrix);
        *counter  = *counter + 1;
        prev_data=curr_data;
        if(fgets(buf,255,fp)!=NULL){
            printf("%s",buf);
            token=strtok(buf,delimiter);
            while (token != NULL && atoi(token) != 0) {
                matrix[curr_data->id - 1][atoi(token)-1] = 1;
                token=strtok(NULL,delimiter);
            }
        }
    }
    prev_data->next=NULL;
    tail = prev_data;
    fclose(fp);
    return head;
}

void eliminate(int matrix[50][50], int counter, int m){
    int i,j,tmp,unique = 0;
    while (unique == 0) {
        unique = 1;
        for (i=0; i<counter; i++) {
            tmp = 0;
            for (j=0; j<counter; j++) {
                if(matrix[j][i] != 0){
                    if(matrix[j][i] == 1){
                        tmp = tmp + 1;
                    }
                }
            }
            if(tmp < m){
                for (j = 0; j<counter; j++) {
                    if(matrix[j][i] != 0){
                        unique = 0;
                        matrix[j][i] = 0;
                        matrix[i][j] = 0;
                    }
                }
            }
        }
    }
}

int findFollower(int matrix[50][50], int visited[50], int counter, int who, int follower, int traveler){
    int i, j;
    j = who;
    if (traveler < counter) {
        visited[j] = 1;
        for (i=0; i<counter; i++) {
            if(matrix[i][j] == 1){
                if(visited[i] == -1){
                    visited[i] = 1;
                    follower = findFollower(matrix, visited, counter, i, ++follower, ++traveler);
                }
            }
        }
    }
    return follower;
}

int findRealFollower(int matrix[50][50], int counter, int who){
    int follower = 0;
    int i;
    for (i=0; i<counter; i++) {
        if (matrix[i][who] == 1){
            follower++;
        }
    }
    return  follower;
}

data* findMember(data* head, int id){
    data* tmp = head;
    while (tmp != NULL) {
        if(tmp->id == id){
            return tmp;
        }
        tmp = tmp->next;
    }
    return head;
}

int main(int argc, const char * argv[]) {
    int m = 1;
    int y = 4;
    int x = 2;
    char File_Name[20] = "SocialNET.txt";
    char Delimeter[2] = ",";
    int matrix[50][50];
    int counter = 0;
    int i, j;
    int visited[50];
    data* head = NULL;
    data* tmp = NULL;
    int choice = 1;
    while (choice != 3) {
        printf("Lutfen programi nasil calistirmak istediginiz seciniz.\n");
        printf("(1)-Normal Mod\n(2)-Detay Mod\n(3)-Cikis\n");
        scanf(" %d", &choice);
        for (i = 0; i<50; i++) {
            for (j = 0; j<50; j++) {
                matrix[i][j] = -1;
            }
            visited[i] = -1;
        }
        if (choice == 1){
            head = fileRead(File_Name, Delimeter, matrix,tmp,&counter);
            eliminate(matrix, counter, m);
            printf("\n\n\n\n");
            for (i=0; i<counter; i++) {
                if (x <= findRealFollower(matrix, counter, i)){
                    if (y <= findFollower(matrix, visited, counter, i, 0, 0)){
                        tmp = findMember(head, i+1);
                        printf("\n%d - %s %s", tmp->id, tmp->name,tmp->surname);
                    }
                    for (j=0; j<counter; j++) {
                        visited[j] = -1;
                    }
                }
            }
            printf("\n\n");
        }else if (choice == 2){
            head = fileRead(File_Name, Delimeter, matrix,tmp,&counter);
            eliminate(matrix, counter, m);
            printMatrix(matrix, counter);
            printf("\n\n\n\n");
            printf("ilk ayristirma isleminden sonra kalan kisiler: \n");
            for (i = 0; i < counter; i++) {
                if (matrix[i][0] != 0){
                    tmp = findMember(head, i+1);
                    printf("\n%d - %s %s", tmp->id, tmp->name,tmp->surname);
                }
            }
            for (j=0; j<counter; j++) {
                visited[j] = -1;
            }
            for (i=0; i<counter; i++) {
                if (x <= findRealFollower(matrix, counter, i)){
                    if (y <= findFollower(matrix, visited, counter, i, 0, 0)){
                        tmp = findMember(head, i+1);
                        printf("\n%d - %s %s", tmp->id, tmp->name,tmp->surname);
                        printf("\nDogrudan baglanti sayisi: %d", findRealFollower(matrix, counter, i));
                        for (j=0; j<counter; j++) {
                            visited[j] = -1;
                        }
                        printf("\nDolayli baglanti sayisi: %d", findFollower(matrix, visited, counter, i, 0, 0) - findRealFollower(matrix, counter, i));
                        printf("\n");
                    }
                    for (j=0; j<counter; j++) {
                        visited[j] = -1;
                    }
                }
            }
            printf("\n\n");
        }
    }
    return 0;
}
