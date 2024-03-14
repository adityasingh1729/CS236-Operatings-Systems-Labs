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


	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		//printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */
		if (strcmp(line,"") == 0) {
		  continue;
		}
		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
       //do whatever you want with the commands, here we just print them

		//for(i=0;tokens[i]!=NULL;i++){
		  //printf("found token %s (remove this debug output later)\n", tokens[i]);
		//}
		if (strcmp(tokens[0],"ls") == 0) {
		  if (tokens[1] && strcmp(tokens[1], "&") == 0) {
		    int rc = fork();
		    if (rc < 0) {
		      fprintf(stderr, "fork failed\n");
		      exit(1);
		    } else if (rc == 0) {
		      char *myargs[3];
		      myargs[0] = strdup("ls");
		      myargs[1] = NULL;
		      myargs[2] = strdup("&");
		      execvp(myargs[0], myargs);
		      printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		    } else {
		      int wc = wait(NULL);
		      printf("Shell: Background process finished\n");
		    }
		  } else {
		    int rc = fork();
		    if (rc < 0) {
		      fprintf(stderr, "fork failed\n");
		      exit(1);
		    } else if (rc == 0) {
		      char *myargs[3];
		      myargs[0] = strdup("ls");
		      myargs[1] = NULL;
		      myargs[2] = NULL;
		      execvp(myargs[0], myargs);
		      printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		    } else {
		      int wc = wait(NULL);
		    }
		  }
		} else if (strcmp(tokens[0],"cat") == 0) {
		  if (tokens[2] && strcmp(tokens[2], "&") == 0) {
		    int rc = fork();
		    if (rc < 0) {
		      fprintf(stderr, "fork failed\n");
		      exit(1);
		    } else if (rc == 0) {
		      char *myargs[3];
		      myargs[0] = strdup(tokens[0]);
		      myargs[1] = strdup(tokens[2]);
		      myargs[2] = strdup(tokens[1]);
		      execvp(myargs[0], tokens);
		      printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		    } else {
		      int wc = wait(NULL);
		      printf("Shell: Background process finished\n");
		    }
		  } else {
		    int rc = fork();
		    if (rc < 0) {
		      fprintf(stderr, "fork failed\n");
		      exit(1);
		    } else if (rc == 0) {
		      char *myargs[3];
		      myargs[0] = strdup(tokens[0]);
		      myargs[1] = strdup(tokens[1]);
		      myargs[2] = NULL;
		      execvp(myargs[0], myargs);
		      printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		    } else {
		      int wc = wait(NULL);
		    }
		  }
		}
		else if (strcmp(tokens[0],"echo") == 0) {
		  int rc = fork();
		  if (rc < 0) {
		    fprintf(stderr, "fork failed\n");
		    exit(1);
		  } else if (rc == 0) {
		    char *myargs[3];
		    myargs[0] = strdup(tokens[0]);
		    myargs[1] = strdup(tokens[1]);
		    myargs[2] = NULL;
		    execvp(myargs[0], tokens);
		    printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		  } else {
		    int wc = wait(NULL);
		  }
		} else if (strcmp(tokens[0],"c") == 0) {
		  int rc = fork();
		  if (rc < 0) {
		    fprintf(stderr, "fork failed\n");
		    exit(1);
		  } else if (rc == 0) {
		    char *myargs[3];
		    myargs[0] = strdup("clear");
		    myargs[1] = NULL;
		    myargs[2] = NULL;
		    execvp(myargs[0], myargs);
		    printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		  } else {
		    int wc = wait(NULL);
		  }
		} else if (strcmp(tokens[0],"sleep") == 0) {
		  if (tokens[2] && strcmp(tokens[2], "&") == 0) {
		    int rc = fork();
		    if (rc < 0) {
		      fprintf(stderr, "fork failed\n");
		      exit(1);
		    } else if (rc == 0) {
		      char *myargs[3];
		      myargs[0] = strdup(tokens[0]);
		      myargs[1] = strdup(tokens[1]);
		      myargs[2] = strdup(tokens[2]);
		      execvp(myargs[0], myargs);
		      printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		    } else {
		      int wc = wait(NULL);
		      printf("Shell: Background process finished\n");
		    }
		  } else {
		    int rc = fork();
		    if (rc < 0) {
		      fprintf(stderr, "fork failed\n");
		      exit(1);
		    } else if (rc == 0) {
		      char *myargs[3];
		      myargs[0] = strdup(tokens[0]);
		      myargs[1] = strdup(tokens[1]);
		      myargs[2] = NULL;
		      execvp(myargs[0], tokens);
		      printf("WRONG WRONG -- SHOULDN'T PRINT\n");
		    } else {
		      int wc = wait(NULL);
		    }
		  }
		} else if (strcmp(tokens[0],"cd") == 0) {
		  if (chdir(tokens[1]) < 0) {
		    printf ("Shell: Incorrect Command\n");
		  }
		} else {
		  printf("COMMAND DOESN'T EXIST!!!\n");
		}
		
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
		  free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
