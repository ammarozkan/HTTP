#include "server.h"
#include <httpClients.h>
#include <essentials.h>
#include <string.h>
#include <quickFile.h>
#include <urlops.h>
#include <stdlib.h>

#define MAINPAGE 0
#define NOTFOUND 1

char* pages[16];

char* getLorePage(char* lore)
{
	char path[128];
	sprintf(path, "lores/%s\0",lore);
	return loadFileToMem(path);
}

struct HTTP
theresponder(struct HTTP request)
{
	char* page = pages[MAINPAGE];
	struct HTTP response = HTTPResponse("HTTP/1.1", "200", "OK");
	
	if (strcmp(request.page,"/")==0) {
		page = pages[MAINPAGE];
	} else {
		updateHTTPStatus(&response, "404", "Not Found");
		page = pages[NOTFOUND]; 
	}	
	
	addPairInt(&response, "Content-Length", strlen(page));
	response.data = page;
	return response;
}

int main(int argc, char** argv)
{
	int parallel = 1;
	
	for (unsigned int i = 0 ; i < argc ; i += 1) {
		if (strcmp(argv[i],"--nonparallel")) parallel = 0;
	}

#ifdef SERVER_TALKING
	printf("HTTP Server Settings\n");
	printf("parallel:%i\n",parallel);
	printf("\n\n");
#endif

	DEB("MAIN START\n");

	int server_fd = getServerSocket(4444);
	TALK("Server Socket Generated.\n");

	setHttpClientRespond(theresponder);
	pages[MAINPAGE] = loadFileToMem("pages/main.html");
	pages[NOTFOUND] = loadFileToMem("pages/notfound.html");
	TALK("Pages Loaded\n");


	if (parallel) {
		TALK("Starting Parallel Loop.\n");
		serverLoop(server_fd);
	} else {
		TALK("Starting Serial Loop.\n");
		while(1) {
			DEB("ACCEPTING\n");
			struct ClientInfo ci = acceptClientSocket(server_fd);
			int finished = 0;
			DEB("HTTP CLIENT WORK IS CALLING\n");
			httpClientWork(ci.client_fd, &finished);
			serverClose(ci.client_fd);
		}
	}

	TALK("Server is cloosing. Bye!\n");

	serverClose(server_fd);
	return 0;
}
