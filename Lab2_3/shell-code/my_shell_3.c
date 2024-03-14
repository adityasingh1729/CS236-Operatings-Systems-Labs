#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

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

void free_mem(char **tokens) {
	for (int i = 0; tokens[i] != NULL; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

int fore_pgid = -1;
int back_num = 0;
int back_pgid = -1;

void signal_handler(int sig) {
	if (fore_pgid == -1) {
		return;
	}
	
	kill(-fore_pgid, SIGTERM);
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	signal(SIGINT, signal_handler);

	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		if (tokens[0] && !strcmp(tokens[0], "exit") && !tokens[1]) {
			free_mem(tokens);
			if (fore_pgid != -1) {
				kill(-fore_pgid, SIGINT);
			}
			if (back_pgid != -1) {
				kill(-back_pgid, SIGINT);
			}
			kill(-getpid(), SIGTERM);
			exit(0);
		}

		int check;

		while (check = waitpid(-1, NULL, WNOHANG) > 0) {
			printf("Shell: Background process finished\n");
			back_num--;
		}

		if (back_num == 0) {
			back_pgid = -1;
		}

		if (!tokens[0]) {
			free_mem(tokens);
			continue;
		}

		int background = -1;
		for (int i = 0; i < MAX_NUM_TOKENS; i++) {
			if (!tokens[i]) {
				break;
			}
			if (!strcmp(tokens[i], "&")) {
				if (tokens[i + 1] == NULL) {
					background = i;
				} else {
					background = -2;
					printf("Shell : Multiple Commands after &\n");
					break;
				}
			}
		}

		if (background == -2) {
			free_mem(tokens);
			continue;
		}

		if (strcmp(tokens[0], "cd")) {
			if (background > 0) {
				tokens[background] = NULL;
				back_num++;
			}
			int pid = fork();
			if (pid == 0) {

				if (background > 0) {
					if (back_num == 0) {
						setpgid(0,0);
					} else {
						setpgid(0,back_pgid);
					}
				} else {
					setpgid(0,0);
				}
				execvp(tokens[0], tokens);
				printf("Shell: Incorrect Command : %s\n",tokens[0]);
				exit(0);
			} else {
				if (background == -1) {
					fore_pgid = pid;
					waitpid(pid, NULL, 0);
				} else if (back_num == 1) {
					back_pgid = pid;
				}
				fore_pgid = -1;
			}
		} else {
			if (!tokens[1] || tokens[2]) {
				printf("Shell: Incorrect Command : %s\n", tokens[0]);
				free_mem(tokens);
				continue;
			} else {
				if (background > 0) {
					printf("Shell: cd cannot be executed in background :\n");
					free_mem(tokens);
					continue;
				}
				int ret = chdir(tokens[1]); // normally it returns 0
				if (ret) {
					printf("Shell: Incorrect Command : %s\n",tokens[0]);
					free_mem(tokens);
					continue;
				}
			}
		}

		// Freeing the allocated memory	
		free_mem(tokens);

	}
	return 0;
}
