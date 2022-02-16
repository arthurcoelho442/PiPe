/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: arthur
 *
 * Created on 15 de Fevereiro de 2022, 18:18
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {
    int fd[3][2];
    pid_t pid[3];
    
    for(int i=0; i<3; i++)
        if(pipe(fd[i])<0) return 1;  
    
    //Cria filhos
    for(int i=0; i<3; i++){
       pid[i] = fork();
       
       if(pid[i] > 0)
           continue;
       else if(pid[i] == 0)
           break;
       else
           exit(1);
    }
    
    if(pid[0] > 0 && pid[1] > 0 && pid[2] > 0){
        printf("Processo Pai\n");
        
        int x, y;
        scanf("%d", &x);
        scanf("%d", &y);
        
        //P0 envia para o filho P1 via pipe (pipe1) o valor de X.
        close(fd[0][0]);//fecha a leitura do pipe 1
        write(fd[0][1], &x, sizeof(int));//envia para o pipe 1 o valor de x
        
        //P0 envia para o filho P2 via pipe (pipe2) o valor de Y.
        close(fd[1][0]);//fecha a leitura do pipe 2
        write(fd[1][1], &y, sizeof(int));//envia para o pipe 1 o valor de y
        
        sleep(5);
        //P0 envia para P1, pelo mesmo pipe1, a seguinte mensagem ms1
        char* ms1 = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor X";
        int tam1 = strlen(ms1)+1;
        write(fd[0][1], &tam1, sizeof(int));//envia para o pipe 1 o tamanho de msg1
        write(fd[0][1], ms1, sizeof(char)*tam1);//envia para o pipe 1 a msg1
        
        //P0 envia para P2, pelo mesmo pipe1, a seguinte mensagem ms2
        char* ms2 = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor Y";
        int tam2 = strlen(ms2)+1;
        write(fd[1][1], &tam2, sizeof(int));//envia para o pipe 2 o tamanho demsg2
        write(fd[1][1], ms2, sizeof(char)*tam2);//envia para o pipe 2 a msg2
        
        sleep(10);
    }
    else if(pid[0] == 0 && pid[1] > 0 && pid[2] > 0){
        printf("Processo Filho 1\n");
        sleep(10);
        int x;
        close(fd[0][1]);//fecha a entrada de escrita do pipe 1
        read(fd[0][0], &x, sizeof(int));//le o que foi escrito no pipe 1
        printf("O valor de x me passado é %d\n", x);
        
        int tam;
        char* msg;
        read(fd[0][0], &tam, sizeof(int));//recebe a msg escrita no pipe 1
        read(fd[0][0], msg, sizeof(char)*tam);//recebe a msg escrita no pipe 1
        
        printf("P1 aqui imprimindo msg:\n%s\n", msg);
        
        //Cria array
        int* array;
        int n = 100 + rand() % 100;
        for(int i =0; i< n; i++)
            array[i] = x;
        close(fd[2][0]);
        write(fd[2][1], &n, sizeof(int));
        write(fd[2][1], array, sizeof(int)*n);
    }else if(pid[0] > 0 && pid[1] == 0 && pid[2] > 0){
        printf("Processo Filho 2\n");
        
        int y;
        close(fd[1][1]);//fecha a entrada de escrita do pipe 2
        read(fd[1][0], &y, sizeof(int));//le o que foi escrito no pipe 2
        printf("O valor de y me passado é %d\n", y);
        
        int tam;
        char* msg;
        read(fd[1][0], &tam, sizeof(int));//recebe a msg escrita no pipe 1
        read(fd[1][0], msg, sizeof(char)*tam);//recebe a msg escrita no pipe 1
        
        printf("P2 aqui imprimindo msg:\n%s\n", msg);
        
        //Cria array
        int* array;
        for(int i =0; i< (100+rand() % 10); i++)
            array[i] = y;
    }else if(pid[0] > 0 && pid[1] > 0 && pid[2] == 0){
        printf("Processo Filho 3\n");
        sleep(10);
        int* array;
        int n;
        close(fd[2][0]);//fecha a entrada de escrita do pipe 2
        sleep(5);
        
        write(fd[2][1], &n, sizeof(int));
        write(fd[2][1], array, sizeof(int)*n);
        printf("Imprimindo array recebido de P1:\n");
        for(int i=0;i<n;i++)
            printf("%d ", array[i]);
        printf("\n");
    }

    
    return (EXIT_SUCCESS);
}

