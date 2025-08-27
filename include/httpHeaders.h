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
char* mergeHTTP(struct HTTP http);
void addPair(struct HTTP* http, char* key, char* value);
void addPairInt(struct HTTP* http, char* key, int value);
struct HTTP HTTPResponse(char* ver, char* statusCode, char* status);
char* HTTPvalue(struct HTTP http, char* key);
void updateRequest(struct HTTP* http, char* request);
void updateHTTPResponse(struct HTTP* http, char* ver, char* statusCode, char* status);
void superCopys(char* target, char* src);
void updateHTTPStatus(struct HTTP* http, char* statusCode, char* status);


#endif /*HTTP_HEADERS*/
