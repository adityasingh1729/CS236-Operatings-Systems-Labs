#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;
  
  for(i =0; i < strlen(line); i++){
    
    char readChar = line[i];
    
    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
        tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
        strcpy(tokens[tokenNo++], token);
        tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
  
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

int main(int argc, char* argv[]) {
  char  line[MAX_INPUT_SIZE];            
  char  **tokens;              
  int i;
  
  int backP[64] = {0};
  for (int i = 0; i < 64; i++) {
    backP[i] = -1;
  }
  
  while(1) {			
    /* BEGIN: TAKING INPUT */    
    bzero(line, sizeof(line));
    printf("$ ");
    scanf("%[^\n]", line);
    getchar();
    
    if (strcmp(line,"") == 0) {
      continue;
    }
    line[strlen(line)] = '\n'; //terminate with new line
    tokens = tokenize(line);
    int i = 0;
    while (tokens[i]) {
      i++;
    }
    if (strcmp(tokens[i-1], "&") == 0) {
      int rc = fork();
      if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
      } else if (rc == 0) {
        int j = 0;
        while (j < 64 && backP[j] != -1) {
          j++;
        }
        if (j < 64) {
          backP[j] =  (int) getpid();
          printf("Putting child in background %d %d\n", j, backP[j]);
        }
        tokens[i - 1] = NULL; // I was not able to think that we have to NULL this
        execvp(tokens[0], tokens);
        printf("WRONG WRONG -- SHOULDN'T PRINT\n");
      }
      continue;
    }
    if (strcmp(tokens[0],"cd") == 0) {
      if (chdir(tokens[1]) < 0) {
        printf ("Shell: Incorrect Command\n");
      }
    } else if (strcmp(tokens[0], "ls") == 0 || strcmp(tokens[0], "echo") == 0 || strcmp(tokens[0], "cat") == 0 ||
               strcmp(tokens[0], "mv") == 0 || strcmp(tokens[0], "sleep") == 0 || strcmp(tokens[0], "clear") == 0) {
      int rc = fork();
      if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
      } else if (rc == 0) {
        execvp(tokens[0], tokens);
        printf("WRONG WRONG -- SHOULDN'T PRINT\n");
      } else {
        int wc = wait(NULL);
      }
    } else {
      printf("COMMAND DOESN'T EXIST!!!\n");
    }

    int j = 0;
    while (j < 64 && backP[j] == -1) {
      printf("backP[%d] = %d\n", j, backP[j]);
      j++;
    }
    printf("j = %d\n", j);
    if (j < 64) {
      printf("Child in background %d %d\n", j, backP[j]);
      int wc = waitpid(backP[j], NULL, WNOHANG);
      if (wc > 0) {
        printf("Shell: Background process finished\n");
      }
    } else {
      printf("No child in background\n");
    }
    
    // Freeing the allocated memory	
    for(i=0;tokens[i]!=NULL;i++){
      free(tokens[i]);
    }
    free(tokens);
  }
  return 0;
}
