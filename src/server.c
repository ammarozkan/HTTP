#include "essentials.h"
#include <server.h>

#include <sys/socket.h>
#include <unistd.h> // open, close
#include <arpa/inet.h>

int
getServerSocket(uint16_t port)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_fd <= 0) {
		DEB("\tSV::SOCKET FAIL\n");
		return -1;
	}

	int okey=1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				&okey, sizeof(okey)) < 0) {
		DEB("\tSV::SETSOCKOPT FAIL\n");
		return -2;
	}

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		DEB("\tSV::BIND FAIL\n");
		return -3;
	}

	listen(server_fd, 4);
	return server_fd;
}

#include <errno.h>

struct ClientInfo
acceptClientSocket(int server_fd)
{
	struct ClientInfo ci;
	ci.addrSize = sizeof(ci.addr);
	errno = 0;
	ci.client_fd = accept(server_fd, (struct sockaddr*)&(ci.addr), 
			&(ci.addrSize));
	if (ci.client_fd <= 0) {
#ifdef SERVER_DEBUG2
		printf("SV::\t\tACCEPT FAIL %i on server_fd %i\n",errno, server_fd);
#endif
	}
	return ci;
}

void
serverClose(int fd)
{
	close(fd);
}

struct ClientList clients = {.first = 0, .last = 0};


#include <httpClients.h>
#include <stdlib.h>

void*
clientFunction(void* ptr)
{
	struct Client* client = ptr;
#ifdef SERVER_DEBUG
	printf("SV::CLIENT::\t\t PTR -> %x\tFD -> %i\n", ptr,client->ci.client_fd);
#endif
	int f;
	DEB("SV::CLIENT::\t\t WORK CALLING\n");
	httpClientWork(client->ci.client_fd, &f);
	
	DEB("SV::CLIENT::\t\t CLOSE\n");
	serverClose(client->ci.client_fd);

	DEB("SV::CLIENT::\t\t FUNCTION FINISHED.\n");

	client->finished = 1;
}


void
putClient(struct ClientInfo ci)
{
	struct Client* client = malloc(sizeof(struct Client));



	client->finished = 0;
	DEB("SV::\t\tITS NOT FINISHED.\n");
	DEB("SV::\t\tCREATING THREAD.\n");
	pthread_create(&(client->thread), 0, clientFunction, (void*)client);
	DEB("SV::\t\tTHREAD CREATED.\n");
	
	client->ci = ci;
	
	DEB("SV::\t\tLIST STUFF IS NOW\n");

	client->next = 0;
	client->prev = clients.last;

	if (clients.first == 0) {
		clients.first = client;
	}

	DEB("SV::\t\tEDITING LAST\n");
	if (clients.last != 0) {
		clients.last->next = client;
		client->prev = clients.last;
	}
	DEB("SV::\t\tLAST EDITED\n");
	clients.last = client;
	DEB("SV::\t\tNEW LAST IS OURS!\n");
}

void* ClientManager(void* ptr)
{
	while(1) { 
	for(struct Client* client = clients.first ; client != 0 ; ) { 
#ifdef SERVER_DEBUG
		printf("SV::CM::\t\t%x %x -> %x\n",clients.last, clients.first, client);
		printf("SV::CM::\t\tFINISHED %i\n", client->finished);
#endif
	if (client->finished) {
		DEB("SV::CM::\t\tClient Manager detected a finished client.\n");
		DEB("SV::CM::\t\tTHREAD JOINING.\n");
		pthread_join(client->thread, 0);
		DEB("SV::CM::\t\tTHREAD JOINED.\n");
		if (client->prev != 0) client->prev->next = client->next;
		if (client->next != 0) client->next->prev = client->prev;
		if (clients.last == client) clients.last = client->prev;
		if (clients.first == client) clients.first = client->next;
		DEB("SV::CM::\t\tLIST MODIFICATIONS DONE\n");
		client->finished = 0;
		struct Client* next = client->next;
		DEB("SV::CM::\t\tFreeing free client\n");
		free(client);
		DEB("SV::CM::\t\tFreed free client\n");
		client = next;
		continue;
	}
	DEB("SV::CM::\t\t CLIENT SAMPLE OK\n");
#ifdef SERVER_DEBUG
	printf("SV::CM::\t\t NEXT CLIENT -> %x\n", client->next);
#endif
	client = client->next;
#ifdef SERVER_DEBUG
	DEB("SV::CM::\t\t NEXT NOW!\n");
#endif
	}
	//DEB("SV::CM::\t\t CLIENT MANAGER LOOP OUT\n");
	}


	

}

void
serverLoop(int server_fd)
{
	pthread_t clientManager;
	pthread_create(&clientManager, 0, ClientManager, 0);
	while(1) {
		DEB("SV::WOP WOP\n");
		struct ClientInfo ci = acceptClientSocket(server_fd);
		DEB("SV::PUTTING CLIENT\n");
#ifdef SERVER_DEBUG
		printf("SV::\t\tCLIENT FD is '%i'.\n", ci.client_fd);
#endif
		putClient(ci);
	}

	pthread_join(clientManager, 0);
}
