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
#include <time.h>

/*
 * 
 */
int main(int argc, char** argv) {
    int fd[3][2];
    int pid[3];
    
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
    
    if(pid[0] == 0){//Filho 1
        sleep(10);
        int x;
        srand(time(NULL));
        close(fd[0][1]);//fecha a escrita do pipe 1
        close(fd[1][1]);//fecha a escrita do pipe 2
        close(fd[1][0]);//fecha a leitura do pipe 2
        close(fd[2][0]);//fecha a leitura do pipe 3
        read(fd[0][0], &x, sizeof(int));//le o que foi escrito no pipe 1
        
        int tam;
        read(fd[0][0], &tam, sizeof(int));//recebe a msg escrita no pipe 1
        char msg[tam];
        read(fd[0][0], msg, sizeof(char)*tam);//recebe a msg escrita no pipe 1
        
        printf("\nP1: P1 aqui imprimindo msg:\n%s\n", msg);
        sleep(1);
        //Cria array
        int n = (rand() % 100) + 100;
        write(fd[2][1], &n, sizeof(int));
        int array[n];
        for(int i =0; i< n; i++)
            array[i] = x;
        write(fd[2][1], array, sizeof(int)*n);
        
        close(fd[0][0]);//fecha a leitura do pipe 1
        close(fd[2][1]);//fecha a escrita do pipe 3
    }else if(pid[1] == 0){//Filho 2
        sleep(11);
        srand(time(NULL));
        int y;
        close(fd[0][0]);//fecha a leitura do pipe 1
        close(fd[0][1]);//fecha a escrita do pipe 1
        close(fd[1][1]);//fecha a escrita do pipe 2
        close(fd[2][0]);//fecha a leitura do pipe 3
        read(fd[1][0], &y, sizeof(int));//le o que foi escrito no pipe 2
        
        int tam;
        read(fd[1][0], &tam, sizeof(int));//recebe a msg escrita no pipe 1
        char msg[tam];
        read(fd[1][0], msg, sizeof(char)*tam);//recebe a msg escrita no pipe 1
        
        printf("P2: P2 aqui imprimindo msg:\n%s\n", msg);
        sleep(1);
        //Cria array
        int n = (rand() % 100) + 100;
        write(fd[2][1], &n, sizeof(int));
        int array[n];
        for(int i =0; i< n; i++)
            array[i] = y;
        write(fd[2][1], array, sizeof(int)*n);
        
        close(fd[1][0]);//fecha a leitura do pipe 2
        close(fd[2][1]);//fecha a escrita do pipe 3
    }else if(pid[2] == 0){//Filho 3
        sleep(14);
        close(fd[0][0]);//fecha a leitura do pipe 1
        close(fd[0][1]);//fecha a escrita do pipe 1
        close(fd[1][0]);//fecha a leitura do pipe 2
        close(fd[1][1]);//fecha a escrita do pipe 2
        close(fd[2][1]);//fecha a escrita do pipe 3
        
        int n1;
        read(fd[2][0], &n1, sizeof(int));
        int array[n1];
        read(fd[2][0], array, sizeof(int)*n1);
        printf("\nP3:Imprimindo array recebido de P1:\n");
        for(int i=0;i<n1;i++)
            printf("%d ", array[i]);
        
        int n2;
        read(fd[2][0], &n2, sizeof(int));
        read(fd[2][0], array, sizeof(int)*n2);
        printf("\nP3:Imprimindo array recebido de P2:\n");
        for(int i=0;i<n2;i++)
            printf("%d ", array[i]);
        printf("\n");
        
        close(fd[2][0]);//fecha a leitura do pipe 3
    }else {//Pai        
        int x, y;
        printf("PO:Digite o valor de x: ");
        scanf("%d", &x);
        printf("PO:Digite o valor de y: ");
        scanf("%d", &y);
        
        //P0 envia para o filho P1 via pipe (pipe1) o valor de X.
        close(fd[0][0]);//fecha a leitura do pipe 1
        close(fd[1][0]);//fecha a leitura do pipe 2
        close(fd[2][0]);//fecha a leitura do pipe 3
        close(fd[2][1]);//fecha a escrita do pipe 3
        
        write(fd[0][1], &x, sizeof(int));//envia para o pipe 1 o valor de x
        
        //P0 envia para o filho P2 via pipe (pipe2) o valor de Y.
        write(fd[1][1], &y, sizeof(int));//envia para o pipe 1 o valor de y
        
        //P0 envia para P1, pelo mesmo pipe1, a seguinte mensagem ms1
        char* ms1 = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor X";
        int tam1 = strlen(ms1)+1;
        write(fd[0][1], &tam1, sizeof(int));//envia para o pipe 1 o tamanho de msg1
        write(fd[0][1], ms1, sizeof(char)*tam1);//envia para o pipe 1 a msg1
        
        //P0 envia para P2, pelo mesmo pipe1, a seguinte mensagem ms2
        char* ms2 = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor Y";
        int tam2 = strlen(ms2)+1;
        write(fd[1][1], &tam2, sizeof(int));//envia para o pipe 2 o tamanho demsg2
        write(fd[1][1], ms2, sizeof(char)*tam2);//envia para o pipe 2 a msg2*/
        
        waitpid(pid[0], NULL, 0);
        waitpid(pid[1], NULL, 0);
        waitpid(pid[2], NULL, 0);
    }

    
    return (EXIT_SUCCESS);
}

