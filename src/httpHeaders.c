#include <httpHeaders.h>
#include <stdio.h>
#include <stdlib.h> // malloc
#include <essentials.h>

int
superStrCompare(char* str1, char* str2)
{
	int res = 0;
	unsigned int i;
	for (i = 0 ; ; i += 1) {
		if (str1[i] == '\0') goto str1_hit;
		if (str2[i] == '\0') goto str2_hit;
		
		res += ((int)str1[i]) - ((int)str2[i]);
	}
str1_hit:
	for( ; str2[i] != '\0' ; i += 1) res += -((int)str2[i]);
	goto ret;
str2_hit:
	for( ; str1[i] != '\0' ; i += 1) res += ((int)str1[i]);
	goto ret;
ret:
	return res;
}

void
superMemCopy(void* target, void* src, size_t size)
{
	char* t = target;
	char* sr = src;
	for (size_t s = 0 ; s < size ; s += 1) {
		t[s] = sr[s];
	}
}

size_t
superStrLen(char* str)
{
	size_t s;
	for (s = 0 ; str[s] != '\0' ; s += 1);
	return s;
}

char*
currCopy(char* curr, char* str)
{
	size_t len = superStrLen(str);
	superMemCopy(curr, str, len);
	curr += len;
	return curr;
}

char*
currCopy2(char* curr, char* str1, char* str2)
{
	curr = currCopy(curr, str1);
	curr = currCopy(curr, str2);
	return curr;
}

void superCopys(char* dest, char* str)
{
	superMemCopy(dest, str, superStrLen(str)+1);
}

char* superCopy(char* str)
{
	size_t len = superStrLen(str);
	char* result = malloc(len+1);
	superMemCopy(result, str, len+1);
	return result;
}


void
updateRequest(struct HTTP* http, char* request)
{
	char typebuf[8];
	char pagebuf[32];
	char verbuf[16];
	char* bufs[3] = {typebuf, pagebuf, verbuf};

	char* curr = request;
	char* bufcurr = typebuf;
	int on = 0;

	while (1) {
		if (*curr == '\0') {
			break;
		} else if (*curr == ' ') {
			on += 1;
			*(bufcurr) = '\0';
			bufcurr = bufs[on];
		} else {
			*(bufcurr++) = *curr;
		}

		curr += 1;
	}

	superMemCopy(http->type, typebuf, sizeof(typebuf));
	superMemCopy(http->page, pagebuf, sizeof(pagebuf));
	superMemCopy(http->ver, verbuf, sizeof(verbuf));
}

void
addPair(struct HTTP* http, char* key, char* value)
{
	size_t keylen = superStrLen(key);
	size_t valuelen = superStrLen(value);

	// setting the pair up
	struct HTTPPair* pair = malloc(sizeof(struct HTTPPair));
	pair->key = superCopy(key);
	pair->value = superCopy(value);

	// adding the pair
	if (http->first == 0) http->first = pair;
	if (http->last != 0) http->last->next = pair;
	pair->prev = http->last;
	pair->next = 0;
	http->last = pair;
}

void 
addPairInt(struct HTTP* http, char* key, int value)
{
	char valstr[64];
	sprintf(valstr, " %i\0",value);
	addPair(http, key, valstr);
}

struct HTTP
parseHTTP(char* buffer)
{
	struct HTTP http;
	http.first = 0;
	http.last = 0;
	http.data = 0;

	int rcame = 0;
	char* curr = buffer;

	char keybuf[256];
	char valuebuf[256];
	char requestbuf[256];

	char* keycurr = keybuf;
	char* valuecurr = valuebuf;

	char* requestcurr = requestbuf;

	int onvalue = -1;

	while (1) {
		if (*curr == '\r') rcame = 1;
		else if(rcame && *curr == '\n') {
			// is it the end of the header?
			if (*(curr+1) == '\r' && *(curr+2) == '\n') {
				curr = curr+3; // moving it to the content start
				break;
			}

			if (onvalue == -1) {
				*(requestcurr+1) = '\0';
				updateRequest(&http, requestbuf);
				onvalue = 0; 
			} else {
				*keycurr = '\0';
				*valuecurr = '\0';	
				addPair(&http, keybuf, valuebuf);
				keycurr = keybuf;
				valuecurr = valuebuf;
				onvalue = 0;
			}
		} else if (onvalue == -1) *(requestcurr++) = *curr;
		else if (onvalue) *(valuecurr++) = *curr;
		else if (*curr == ':') onvalue = 1;
		else *(keycurr++) = *curr;

		curr++;
	}
	return http;
}


char*
mergeHTTP(struct HTTP http)
{
	char* result; 
	if(http.data != 0) result = malloc(2048+superStrLen(http.data));
	else result = malloc(2048);
	char* curr = result;
	char lineEnder[3] = "aa";
	lineEnder[0] = 0x0d;
	lineEnder[1] = 0x0a;
	if(http.rr == 1) {
		curr = currCopy2(curr, http.type, " ");
		curr = currCopy2(curr, http.page, " ");
		curr = currCopy(curr, http.ver);
		curr = currCopy(curr, lineEnder);
	} else {
		curr = currCopy2(curr, http.ver, " ");
		curr = currCopy2(curr, http.page, " ");
		curr = currCopy(curr, http.type);
		curr = currCopy(curr, lineEnder);
	}

	for (struct HTTPPair* pair = http.first ; pair != 0 ; pair = pair->next) {
		curr = currCopy(curr, pair->key);
		curr = currCopy(curr, ":");
		curr = currCopy(curr, pair->value);
		curr = currCopy(curr, lineEnder);
		if (pair == http.last) break;
	}
	curr = currCopy(curr, lineEnder);
	if (http.data != 0) curr = currCopy(curr, http.data);
	return result;
}

void updateHTTPStatus(struct HTTP* http, char* statusCode, char* status)
{
	superCopys(http->type, status);
	superCopys(http->page, statusCode);
}

void updateHTTPResponse(struct HTTP* http, char* ver, char* statusCode, char* status)
{
	superCopys(http->ver, ver);
	superCopys(http->type, status);
	superCopys(http->page, statusCode);
}

struct HTTP
HTTPResponse(char* ver, char* statusCode, char* status)
{
	struct HTTP response;
	response.rr = 0;
	response.first = 0;
	response.last = 0;
	response.data = 0;
	superCopys(response.ver, ver);
	superCopys(response.type, status);
	superCopys(response.page, statusCode);
	return response;
}

char*
HTTPvalue(struct HTTP http, char* key)
{
	for (struct HTTPPair* pair = http.first ; ; pair = pair->next) {
		if(superStrCompare(pair->key, key) == 0) return pair->value;
	}

	return 0;
}
