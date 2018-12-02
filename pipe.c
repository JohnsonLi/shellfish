#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

int pipes(char *input){
    printf("GONNA PIPE\n");
    
    char **pipees = malloc(1024 * sizeof(char *));

    int counter = 0;
    while(input){
      pipees[counter] = strsep(&input, "|");
      counter++;
  }

    // printf("%s\n", pipees[0]);
    // printf("%s\n", pipees[1]);

    // int temp_stdout = dup(STDOUT_FILENO);

  int p[2];
  pipe(p);

  int a = fork();
  if(a == 0){
    int temp_stdout = dup(STDOUT_FILENO);
    close(p[PIPE_READ]);
    dup2(p[PIPE_WRITE], STDOUT_FILENO);
        // close(p[PIPE_WRITE]);

    char **args_parsed = malloc(1024 * sizeof(char *));
    parse_args(pipees[0], args_parsed);
    execvp(args_parsed[0], args_parsed);

    dup2(temp_stdout, STDOUT_FILENO);

    return 0;
} else {
    int b = fork();
    if(b == 0){
        int temp_stdin = dup(STDIN_FILENO);
        close(p[PIPE_WRITE]);
        dup2(p[PIPE_READ], STDIN_FILENO);
            // close(p[PIPE_READ]);

        char **args_parsed = malloc(1024 * sizeof(char *));
        parse_args(pipees[1], args_parsed);
        printf("running %s\n", args_parsed[0]);
        execvp(args_parsed[0], args_parsed);

        dup2(temp_stdin, STDIN_FILENO);

        return 0;
    } 
}

int s, b;
int c, d;
s = wait(&b);
c = wait(&d);

fflush(stdout);

free(pipees);

}

int pipes2(char* input){
    char **pipees = malloc(1024 * sizeof(char *));
    char *buf = malloc(1024*sizeof(char*));
    FILE *temp;
    FILE *tempout;
    int counter = 0;
    while(input){
        pipees[counter] = strsep(&input, "|");
        counter++;
    }
    temp = popen(pipees[0] , "r");
    tempout = popen(pipees[1], "w");
    fgets(buf, 1024, temp);
    fputs(buf,tempout);
    pclose(temp);
    pclose(tempout);
    return 0;
}