/**
 * Chris Delaney
 * Project 3
 * simple shell
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	9 /* size of history */

char history[MAX_COMMANDS][MAX_LINE]; //the array used to store history commands.
char display_history [MAX_COMMANDS][MAX_LINE]; 
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

/**
 * Add the most recent command to the history.
 */

void addtohistory(char inputBuffer[]) {

	if(command_count > 9) {
		command_count = 9;
	}
	if(command_count == 0) {
		strcpy(history[command_count], inputBuffer);
		strcpy(display_history[command_count], inputBuffer);
	}
	else {
		for(int i=command_count; i>0; i--) {
			strcpy(history[i], history[i-1]);
			strcpy(display_history[i], display_history[i-1]);
		}
		strcpy(history[0], inputBuffer);
		strcpy(display_history[0], inputBuffer);		
	}
	command_count++;

	// update array"history": add the command to history, strcpy(str1,str2);
	
	// update array"display_history": remove characters like '\n', '\0' in order to display nicely
 

	return;
}

/** 
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */

int setup(char inputBuffer[], char *args[],int *background)
{
    int length,		/* # of characters in the command line */
	i,				/* loop index for accessing inputBuffer array */
	command_number = 0;	/* index of requested command number */

	//define your local varialbes here;
	char tempBuff[MAX_LINE];
	int j = 0;
	
	
    /* read what the user enters on the command line */
	do {
		printf("osh>");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); /* swallow newline characters */


    if (length == 0) {
        exit(0);            /* ^d was entered, end of user command stream */
    }
 
    if ( (length < 0) ) {
		perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
    }
	
	/**
	 * Check if they are using history
	 */
	
	// fill in your code here Part II, if the user input is to repeat some history commands
	if (inputBuffer[0] == '!') {
		if(inputBuffer[i] > command_count) {
			printf("no such command in history\n");
		}
		else {
			switch(inputBuffer[1]) {
				case '2':
					printf("%s", display_history[1]);
					//inputBuffer = history[1];
					strcpy(inputBuffer, history[1]);
					break;
				case '3':
					printf("%s", display_history[2]);
					inputBuffer = history[2];
					break;
				case '4':
					printf("%s", display_history[3]);
					inputBuffer = history[3];
					break;
				case '5':
					printf("%s", display_history[4]);
					inputBuffer = history[4];
					break;
				case '6':
					printf("%s", display_history[5]);
					inputBuffer = history[5];
					break;
				case '7':
					printf("%s", display_history[6]);
					inputBuffer = history[6];
					break;
				case '8':
					printf("%s", display_history[7]);
					inputBuffer = history[7];
					break;
				case '9':
					printf("%s", display_history[8]);
					inputBuffer = history[8];
					break;
				default:
					printf("%s", display_history[0]);
					inputBuffer = history[0];
					break;
			}
		}
	}
	
	

	/**
	 * Add the command to the history
	 */

	if(inputBuffer[0] != '!' && strcmp(inputBuffer, "history\n")) { 
		inputBuffer[length] = '\0';
		addtohistory(inputBuffer); 
	}
	
	/**
	 * Parse the contents of inputBuffer
	 */

    for (i=0;i<length;i++) { 
		/* examine every character in the inputBuffer */
		
        switch (inputBuffer[i]){
			case ' ':
			case '\t' : /* argument separators */
				tempBuff[j] = '\0';
				inputBuffer[i] = '\0';
				char *copier = malloc(sizeof(char)* strlen(tempBuff));
				strcpy(copier,tempBuff);
				args[command_number] = copier;
				j = 0;
				command_number++;		
				break;		
				
			case '\n':  /* should be the final char examined */
				/* no more arguments to this command */	
				tempBuff[j] = '\0';
				inputBuffer[i] = '\0';
				char *copier2 = malloc(sizeof(char)* strlen(tempBuff));
				strcpy(copier2,tempBuff);
				args[command_number] = copier2;
		        	command_number++;		
				args[command_number] = NULL;
				break;
				
	    		default :             /* some other character */
				if (inputBuffer[i] == '&') {
					*background = 1;
					break;
				}
				tempBuff[j] = inputBuffer[i];
				j++;
				break;

				/* args[i] is a pointer to a string, its value is the address of the first charater of that string
				* You want to track the location of the beginning character of each string. 
				* The location is the first character, which is not '\t', not '\t', and not '\n'
				* You also need check "&". If '&' is detected, setup background flag.
				*/  

		} /* end of switch */
	}    /* end of for */

	/**
	 * Here you finish parsing the input. 
	 * There is one more thing to assure. If we get '&', make sure you don't enter it in the args array
	 */
	


	return 1;
	
} /* end of setup routine */


int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	int background;             	/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	pid_t child;            		/* process id of the child process */
	 

	int shouldrun = 1;
	

		
    while (shouldrun){            		/* Program terminates normally inside setup */
		background = 0;
		
		shouldrun = setup(inputBuffer,args,&background);       /* get next command */
		
		/* if the user typed in "exit", the shell program will return (or terminate). 
		* Call strncmp(str1,str1,count). The function call will return 0 if str1 == str2.
		* "count" is the number of characters we use to compare.    
		*/		

		if(!strcmp(args[0], "exit"))
			exit(0);
		
		/* if the user typed in "history", the shell program will display the history commands. 
		* you will use "printf" to print the display_history
		* after you display all the array, this command is done. 
		* Your program should go to read again, which means calling the "setup" function.  
		*/

		if(!strcmp(args[0], "history")) {
			if(command_count == 0)
				printf("no such commands in history\n");
			else {
				for(int k=0; k<command_count; k++) {
					printf("%d        %s", k+1, display_history[k]);
				}
			}		
			setup(inputBuffer,args,&background);
		}
		
	

					
		if (shouldrun) {

			// creates a duplicate process! 
			// pid<0  error
			// pid == 0, it is the child process. use the system call execvp(args[0],args);
			// pid > 0, it is the parent. Here you need consider it is foreground or background
			
	
			child = fork();
			if (child < 0) {
				perror("lsh");
			}
			else if (child == 0) {
				if(execvp(args[0], args) < 0) {
					printf("command not found\n");
				}
			}
			else {
				if (background == 0) {
					int status;
					waitpid(child, &status, 0);
				}
				else {
					setup(inputBuffer, args, &background);
					printf("%d\n", child);
				}
			}
		}
		
    }
	
	return 0;
}

