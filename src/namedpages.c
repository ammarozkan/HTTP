#include <namedpages.h>
#include <stdlib.h>
#include <dirent.h>


void NamedPagesInit(NamedPages_t* namedPages, size_t count)
{
    namedPages.pages = malloc(sizeof(NamedPage_t)*count);
	for (unsigned int i = 0 ; i < count ; i += 1) namedPages.pages[i].assigned = 0;
    namedPages.count = count;
}

char* GetNamedPage(NamedPages_t* namedPages, char* page)
{
	for(unsigned int i = 0 ; i < namedPages.count && namedPages[i].assigned; i += 1) {
		if(strcmp(namedPages[i].name, page) == 0)
			return namedPages[i].page;
	}
	return 0;
}

void AddNamedPage(NamedPages_t* namedPages, char* name, char* page)
{
	unsigned int i;
	for(i = 0 ; (i < namedPages.count) && (namedPages[i].assigned == 1); i += 1);
	namedPages[i].assigned = 1;
	namedPages[i].name = name;
	namedPages[i].page = page;
	printf("ADD NAMED PAGE:'%s' '%s'\n", name, page);
}

void AddNamedPagesFromDir(NamedPages_t* namedPages, char* path)
{
	DIR* d;
	struct dirent* dir;
	d = opendir(path);
	if(d) {
		while((dir = readdir(d)) != NULL) {
			if (dir->d_type != DT_REG) continue;
			char* name = malloc(strlen(dir->d_name)+1);
			memcpy(name, dir->d_name, strlen(dir->d_name)+1);
			char _path[64];
			sprintf(_path, "%s%s\0", path, dir->d_name);
			AddNamedPage(namedPages, name, loadFileToMem(_path));
		}
		closedir(d);
	}
}