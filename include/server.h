#include <arpa/inet.h>
#include <pthread.h>


struct ClientInfo {
	int client_fd;
	struct sockaddr_in addr;
	socklen_t addrSize;
};

struct Client {
	struct ClientInfo ci;
	pthread_t thread;
	int finished;
	struct Client* prev;
	struct Client* next;
};

struct ClientList {
	struct Client* first;
	struct Client* last;
};

// Returns a server socket file definitor for that port, accepting
// any connection. (tcp://0.0.0.0:port)
int getServerSocket(uint16_t port);

// Accepts a new client and returns all the possible details that
// could getten from the accepting phase.
struct ClientInfo acceptClientSocket(int server_fd);

// Accepts clients and creates seperated threads for them.
// Creates a client list managing thread.
void serverLoop(int server_fd);

// Close a file definitor
void serverClose(int fd);
