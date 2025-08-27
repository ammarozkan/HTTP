#include <quickFile.h>
#include <stdio.h>
#include <stdlib.h>

char*
loadFileToMem(char* filepath)
{
	FILE* fp;
	long lSize;

	fp = fopen(filepath, "rb");
	if ( !fp ) return 0;

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	char* buffer = calloc(1, lSize+1);
	if(!buffer) fclose(fp);

	if(1!=fread(buffer, lSize, 1, fp)) {
		fclose(fp);
		free(buffer);
		return 0;
	}

	fclose(fp);
	return buffer;
}
