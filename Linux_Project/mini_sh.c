#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>

#define FALSE 0
#define TRUE 1

#define EOL	1
#define ARG	2
#define AMPERSAND 3

#define FOREGROUND 0
#define BACKGROUND 1

static char	input[512];
static char	tokens[1024];
char		*ptr, *tok;

int get_token(char **outptr)
{
	int	type;

	*outptr = tok;
	while ((*ptr == ' ') || (*ptr == '\t')) ptr++;

	*tok++ = *ptr; // user entire input
	switch (*ptr++) {
		case '\0' : type = EOL; break;
		case '&': type = AMPERSAND; break;
		default : type = ARG;
			while ((*ptr != ' ') && (*ptr != '&') &&
				(*ptr != '\t') && (*ptr != '\0'))
				*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return(type);
}

int execute(char **comm, int how)
{
	char *path = NULL, *symbols = "<>|";
	int redir = 0, i;
	int pid1, pid2;
	int fd[2];
	
	for (i = 1; comm[i] != NULL; i++) {
		 if (strcspn(comm[i], symbols) == 0) {
			redir = i;
			path = (redir == 2) ? comm[redir-1] : "./";
			break;
		}
	}
	
	if (redir != 0) {
		if (strcmp(comm[redir], ">") == 0) {
			if ((pid1 = fork()) < 0) {
				fprintf(stderr, "minish : fork error\n");
				return -1;
			}
			else if (pid1 == 0) {
				fd[0] = open(comm[redir+1], O_RDWR|O_CREAT|S_IROTH, 0644);
				if (fd[0] < 0) {
					perror("> open error");
					exit(-1);
				}
				dup2(fd[0], STDOUT_FILENO);
				close(fd[0]);
				execlp(*comm, *comm, path, (char*)0);
				exit(0);
			}
		} else if (strcmp(comm[redir], "<") == 0) {
			if ((pid1 = fork()) < 0) {
				fprintf(stderr, "minish : fork error\n");
				return -1;
			}
			else if (pid1 == 0) {
				fd[0] = open(comm[redir+1], O_RDONLY);
				if (fd[0] < 0) {
					perror("< open error");
					exit(-1);
				}
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
				if (strcmp(comm[0], "ls") != 0) path = comm[redir+1];
				execlp(*comm, *comm, path, (char*)0);
				exit(0);
			}
		} else if (strcmp(comm[redir], "|") == 0) {
			pipe(fd);
			if ((pid1 = fork()) < 0) {
				fprintf(stderr, "minish : fork error\n");
				return -1;
			}
			else if (pid1 == 0) {
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				execlp(*comm, *comm, path, (char*)0);
				fprintf(stderr, "minish : command not found\n");
				exit(127);
			}

			if ((pid2 = fork()) < 0) {
				fprintf(stderr, "minish : fork error\n");
				return -1;
			}
			else if (pid2 == 0) {
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
				close(fd[1]);
				execvp(comm[redir+1], &comm[redir+1]);
				fprintf(stderr, "minish : command not found\n");
				exit(127);
			}
			close(fd[0]);
			close(fd[1]);
		}
	} else {
		if ((pid1 = fork()) < 0) {
			fprintf(stderr, "minish : fork error\n");
			return -1;
		}
		else if (pid1 == 0) {
			if (strcmp(comm[0], "ls") == 0 && comm[1] == NULL) {
				comm[1] = "./";
				comm[2] = NULL;
			}
			execvp(*comm, comm);
			fprintf(stderr, "minish : command not found\n");
			exit(127);
		}
	}

	if (how == BACKGROUND) {	/* Background execution */
		printf("[%d]\n", pid1);
		return 0;
	}		
	/* Foreground execution */
	// 포그라운드일 경우 부모는 wait
	// 백그라운드일경우 부모는 wait하지않는다.
	while (waitpid(pid1, NULL, 0) < 0)
		if (errno != EINTR) return -1;
	return 0;
}

int parse_and_execute(char *input)
{
	char *arg[1024];
	int	type, how;
	int	quit = FALSE;
	int	narg = 0;
	int	finished = FALSE;

	ptr = input;
	tok = tokens;
	while (!finished) {
		switch (type = get_token(&arg[narg])) {
		case ARG :
			narg++;
			break;
		case EOL :
		case AMPERSAND:
			if (!strcmp(arg[0], "quit")) quit = TRUE;
			else if (!strcmp(arg[0], "exit")) quit = TRUE;
			else if (!strcmp(arg[0], "cd")) {
				/* Do something */
				/* 현재 디렉토리 위치 바꾸기 */
				if (!strcmp(arg[1], "~")) {
					char* uid = getpwuid(getuid())->pw_name;
					if (!strcmp(uid, "root")) arg[1] = "/root";
					else {
						char tmp[100] = "/home/";
						strcat(tmp, uid);
						arg[1] = tmp;
					}
				}
				if (chdir(arg[1]) == -1) {
					fprintf(stderr, "minish [%s]: No such file or directory\n", arg[1]);
					finished = TRUE;
				}
			}
			else if (!strcmp(arg[0], "type")) {
				if (narg > 1) {
					int	i, fid;
					int	readcount;
					char	buf[512];
					/*  학생들이 프로그램 작성할 것
					fid = open(arg[1], ...);
					if (fid >= 0) {
						readcount = read(fid, buf, 512);
						while (readcount > 0) {
							for (i = 0; i < readcount; i++)
								putchar(buf[i]);
							readcount = read(fid, buf, 512);
						}
					}
					close(fid);
					*/
				}
			}
			else { // cd나 type이 아닌 외부명령어 -> forkexec
				how = (type == AMPERSAND) ? BACKGROUND : FOREGROUND;
				arg[narg] = NULL;
				//printf("arg[1] : %s\n", arg[1]);
				//printf("arg[2] : %s\n", arg[2]);
				if (narg != 0)
					execute(arg, how); // how : 뒤에 &의 유무
			}
			narg = 0;
			if (type == EOL)
				finished = TRUE;
			break; 
		}
	}
	return quit;
}


int main()
{
   	char *arg[1024];
	int	quit;

	printf("msh # ");
	while (gets(input)) {
		quit = parse_and_execute(input);
		if (quit) break;
		printf("msh # ");
	}

	return 0;
}
