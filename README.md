# HTTP
Serves Hyper Pages.

## theresponder

When a request cames up, HTTP request will be parsed
and theresponder function will be invoked. Just like
in the default function, it can be used to serve some
stuff. Has a API-like thing to create a HTTP request.
Pairs could be adden with addPair function etc. For 
more information on HTTP request and response thing,
look at the header file include/httpHeaders.h. All
the functions provided are there.

## --nonparallel

The system provided in this creates new thread for
every request and continues to accept new requests
while answering that request without breaking the
connection (of course if response is just one time,
connection is broken after responding). But, if that
option specified while executing the program, it will
execute the second way of handling stuff. Linear
accept-answer-close-accept loop.

## namedpages

Theres also a implementation of auto detecting the
html pages that in a directory and loading them into 
memory by their names.

```C
#include <namedpages.h>
#include <quickFile.h> // for loadFileToMem(char* path) function

///// afterwards in the main function

NamedPages_t storiesPages;
NamedPagesInit(&storiesPages, 32);  // 32 is the maximum size.
                                    // for pages
AddNamedPageFromDir(&storiesPages, "stories/");
AddNamedPage(&storiesPages, "main", loadFileToMem("storiesIndex.html"));

////

And then, when a page needed to be broughten

GetNamedPage(&storiesPages, "main");
```

## Other

For further information, header files have some detailed
explanation as comment lines.
