

#ifdef SERVER_DEBUG
#include <stdio.h>
#define DEB(x) printf(x);
#define SERVER_DEBUG2
#else
#define DEB(x) ;
#endif


#ifdef SERVER_DEBUG2
#include <stdio.h>
#define DEB2(x) printf(x);
#define SERVER_TALK
#define SERVER_REQUEST_LOG
#else
#define DEB2(x) ;
#endif

#ifdef SERVER_TALK
#include <stdio.h>
#define TALK(x) printf(x)
#else
#define TALK(X) ;
#endif

#ifdef SERVER_REQUEST_LOG
#include <stdio.h>
#endif
