/****
* Project 04: Shell
* COSC 208, Introduction to Computer Systems, Spring 2022
****/
// QUESTION: Handle errors when order of command does not satisfy?
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define PROMPT "shell> "
#define MAX_BACKGROUND 50

/*
 * Break input string into an array of strings.
 * @param input the string to tokenize
 * @param delimiters the characters that delimite tokens
 * @return the array of strings with the last element of the array set to NULL
 */
char** tokenize(const char *input, const char *delimiters) {    
    char *token = NULL;

    // make a copy of the input string, because strtok
    // likes to mangle strings.  
    char *input_copy = strdup(input);

    // find out exactly how many tokens we have
    int count = 0;
    for (token = strtok(input_copy, delimiters); token; 
            token = strtok(NULL, delimiters)) {
        count++ ;
    }
    free(input_copy);

    input_copy = strdup(input);

    // allocate the array of char *'s, with one additional
    char **array = (char **)malloc(sizeof(char *)*(count+1));
    int i = 0;
    for (token = strtok(input_copy, delimiters); token;
            token = strtok(NULL, delimiters)) {
        array[i] = strdup(token);
        i++;
    }
    array[i] = NULL;
    free(input_copy);
    return array;
}

/*
 * Free all memory used to store an array of tokens.
 * @param tokens the array of tokens to free
 */
void free_tokens(char **tokens) {    
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

/* signal handler for SIGCHLD */ 
void sigchld_handler(int signum) {
    // This means the parent only calls wait when there is an exited child process to reap, and thus it doesn’t block on a wait call. 
    int status;
    pid_t pid;
    /*
     * reap any and all exited child processes
     * (loop because there could be more than one)
     */
    while( (pid = waitpid(-1, &status, WNOHANG)) > 0) {
    }
}

/* Count number of arguments */
int count(char **command) {
    int n = 0;
    while (command[n] != NULL) {
        n++;
    }
    return n;
}

/* Determine whether in Background shell or Foreground shell */
int shell_mode(char **command, int n) {
    int background = 0;                                // A boolean indicating if the process should run in background, set to false
    
    if (command[n-1][0] == '&') {                      // Set boolean to true if there is an ampersand
        background = 1;   
        command[n-1] = NULL;  
    }
    return background;
}

int main(int argc, char **argv) {    
    printf("%s", PROMPT);
    fflush(stdout);  // Display the prompt immediately
    char buffer[1024];
    int curr_child = 0;   // Keep track of the closest child process

    while (fgets(buffer, 1024, stdin) != NULL) {
        char terminate[] = "exit", foreground[] = "fg";
        char **command = tokenize(buffer, " \t\n");
        int n = count(command);

        if (command[0] == NULL) {
            // Do nothing
        } else if (strcmp(command[0], terminate) == 0) {
            exit(0);          /* built-in command: exit */      
        } else if (strcmp(command[0], foreground) == 0) {
            waitpid(curr_child, NULL, 0); /* built-in command: fg */ 
            
        } else {
            /* implement non built-in commands */ 
            int background = shell_mode(command, n);
            int pid = fork();                                  // Create a child process
            curr_child = pid;                                  // We store the pid of the most recent background process here
      
            if (pid == -1) {                                   /* check and handle error forking */
                printf("fork failed!\n");
                exit(0);
            } else if (pid == 0) {                               // If background: child is left to be scheduled and run          
                if (execv(command[0], command) < 0) {                                  /* only executed if execv fails */
                    printf("Error: execvp failed!!!\n");
                    exit(0);
                }                  
            } else {  
                if (background) {
                    if (signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
                        printf("Error call to signal, SIGINT\n");                  // BACKGROUND SHELL: Non-bloacking parent
                    }
                } else {
                    waitpid(pid, NULL, 0);                                         // FOREGROUND SHELL: Blocking parent
                } 
            }
        }
        free_tokens(command);
        printf("%s", PROMPT);
        fflush(stdout);  // Display the prompt immediately
    }

    return EXIT_SUCCESS;
}
