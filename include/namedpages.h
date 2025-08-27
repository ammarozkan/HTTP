struct NamedPage {
	char* name;
	char* page;
	int assigned;
}; typedef struct NamedPage NamedPage_t;

struct NamedPages {
    NamedPage_t* namedPages;
    size_t count;
}; typedef struct NamedPages NamedPages_t;

void NamedPagesInit(NamedPages_t* namedPages, size_t count);
char* GetNamedPage(NamedPages_t* namedPages, char* page);
void AddNamedPage(NamedPages_t* namedPages, char* name, char* page);
void AddNamedPagesFromDir(NamedPages_t* namedPages, char* path);