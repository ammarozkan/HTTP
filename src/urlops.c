#include <urlops.h>

int urlStartsWith(char* url, char* sw)
{
	return urlGetAfter(url, sw) != 0;
}

char* urlGetAfter(char* url, char* sw)
{
	int a = 0;
	for( ; *sw != '\0' ;) {
		if(*url != *sw) return 0;
		url += 1; sw += 1;
		if (*url == '\0' && *sw != '\0') return 0;
	}
	return url;
}
