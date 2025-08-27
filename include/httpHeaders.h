#ifndef HTTP_HEADERS
#define HTTP_HEADERS

#include <stddef.h>

struct HTTPPair {
	char* key;
	char* value;

	struct HTTPPair* prev;
	struct HTTPPair* next;
};

struct HTTP {
	int rr; // 1 if request, 0 if response
	char type[32]; // request type or response status
	char page[32]; char ver[16];
	struct HTTPPair* first;
	struct HTTPPair* last;
	char* data;
};

struct HTTP parseHTTP(char* value);

// Merges all the http header to a mallocated text for sending it out.
char* mergeHTTP(struct HTTP http);

// adds a header key-value pair to the header.
void addPair(struct HTTP* http, char* key, char* value);

// adds a header key-value pair to the header for integers. For example for 
// Content-Length.
void addPairInt(struct HTTP* http, char* key, int value);

// Gets a basic started for a http response.
struct HTTP HTTPResponse(char* ver, char* statusCode, char* status);

// Whats that key's value?
char* HTTPvalue(struct HTTP http, char* key);

// I have a request line (the first line in http), parse and put it to my 
// HTTP thingy!
void updateRequest(struct HTTP* http, char* request);

// Changes http response codes and values from the parameters.
void updateHTTPResponse(struct HTTP* http, char* ver, char* statusCode, char* status);

// Changes http request codes and values from the parameters.
void updateHTTPStatus(struct HTTP* http, char* statusCode, char* status);


#endif /*HTTP_HEADERS*/
