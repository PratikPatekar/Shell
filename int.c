  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   
#include <errno.h>    
#include <dirent.h>
#include <sys/wait.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <readline/readline.h>
#include <readline/history.h>

	char *input = NULL;
	size_t capline = 0;
	int i;
	char *token;
	char *array[512]; 
	
	void displayPrompt(){
		printf("%s",array[2]);
	}


	void displayHistory(){
		HISTORY_STATE *hist = history_get_history_state();
		HIST_ENTRY **list = history_list();
		for(int i=0;i<hist->length;i++){
			printf("%d %2s %s\n",i,list[i]->line,list[i]->timestamp);
		}
	}

	
	int socketClient(char *ipAddress, int portno) {
	    int sockfd, n,s;
	    char buffer[256];
	    struct sockaddr_in serv_addr;
	    struct hostent *server;
	
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0)
	        printf("No opening socket");
	
	    server = gethostbyname(ipAddress);
	
	    if (server == NULL) {
	        printf("No such host\n");
	        exit(0);
	    }
	
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	
	    bcopy((char *) server->h_addr,
	          (char *) &serv_addr.sin_addr.s_addr,
	          server->h_length);
	
	    serv_addr.sin_port = htons(portno);
	
	    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
	        printf("ERROR connecting");
		}else{
	    while(1){
	        printf("RemotePC:$ ");
	        FILE *fp;
			fp = popen(array[0],"w");
			fp = popen(buffer,"r");	
	        bzero(buffer, 256);
	        fgets(buffer, 255, stdin);
	        s = write(atoi(array[0]), buffer, strlen(buffer));
	        if (n < 0)
	            printf("Could not write to socket");
	   		 }
	    	n = read(s,buffer,strlen(buffer));
	    }
	    return 0;
	}

	int socketServer(int portNum) {
	
	    int server, new_server;
	    socklen_t client_address_length;
	    char buffer[256];
	    struct sockaddr_in server_address, client_address;
	    int n,s;
	
	    server = socket(AF_INET, SOCK_STREAM, 0);
	    if (server < 0){
	        printf("ERROR: opening socket");
		}
		
	    bzero((char *) &server_address, sizeof(server_address));
	    server_address.sin_family = AF_INET;
	    server_address.sin_addr.s_addr = INADDR_ANY;
	    server_address.sin_port = htons(portNum);
	
	    if (bind(server, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
	        printf("ERROR: on binding");
		}
	    listen(server, 5);
	    client_address_length = sizeof(client_address);
	    new_server = accept(server, (struct sockaddr *) &client_address, &client_address_length);
	
	    if (new_server < 0)
	        printf("ERROR: on accept");
	
	    while(1){
	        bzero(buffer, 256);
	        FILE *fp;
	        fp = popen(buffer,"r");
	        fp = popen(buffer,"w");
	        n = read(new_server, buffer, 255);
	        if (n < 0)
	            printf("ERROR: reading from socket");
	        printf("%s\n", buffer);
	   		}
	   		s=write(n,buffer,strlen(buffer));
	
	    return 0;
	}

	
	void intMath(char *s, char *a, char *b){
		int c;
		if (strcmp(s,"+")==0) {
			c = atoi(a)+atoi(b);
			printf("%d\n",c);
		}
		else if (strcmp(s,"-")==0) {
			c = atoi(a)-atoi(b);
			printf("%d\n",c);
		}
		else if (strcmp(s,"*")==0) {
			c = atoi(a)*atoi(b);
			printf("%d\n",c);
		}
		else if (strcmp(s,"/")==0) {
			c = atoi(a)/atoi(b);
			printf("%d\n",c);
		}
		else if (strcmp(s,"%")==0) {
			c = atoi(a)%atoi(b);
			printf("%d\n",c);
		}
	}

/*
		void listdir(char *d){
			DIR *dir;
			struct dirent *directory;
			dir = opendir("d");
			if(dir){
				while((directory=readdir(d))!=NULL){
					printf("%s ",directory->d_name);
				}
				closedir(dir);
			}
		}
*/	

	void tokens(char *input){
		i = 0;
		token = strtok(input, "\n ");
			while (token != NULL) { 
				array[i++] = token;
				token = strtok(NULL, "\n ");
			}
		array[i] = NULL;
	}

   void execute(){
	   int pid = fork();
				if (pid != 0) { 
					int s;
					waitpid(-1, &s, 0);
			} else {
				
					if(strcmp(input,"cd")==0) {
						chdir(array[1]);
						//input=readline(array[1]);						
					}
					else if(strcmp(input,"history")==0) {
						displayHistory();
					}
					else if(strcmp(input,"math")==0){
						intMath(array[2],array[1],array[3]);
					}
					else if(strcmp(input,"client")==0){
						socketClient(array[1],atoi(array[2]));
					}
					else if(strcmp(input,"server")==0){
						socketServer(atoi(array[1]));
					}
					else if(execvp(array[0],array)==-1){
						perror("Wrong Command");
						exit(errno);
					}			
			}
   }
   
	
	int main(){
		while(1){
				//displayPrompt();			
				input=readline("_>");
				 
				if(strcmp(input,"\n")==0){
					perror("Please type in a command " );
					continue;
				}
				
			tokens(input); 
			add_history(input);

			if (strcmp(array[0], "q") == 0) {
				printf("Quitting Shell\n");
				exit(0);
			}
			execute();
	}
	
	
}
