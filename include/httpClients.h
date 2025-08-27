#include <httpHeaders.h>

typedef struct HTTP (*httpClientResponder)(struct HTTP request);

// set the client responder, change it.
void setHttpClientRespond(httpClientResponder responder);

// The default responder
struct HTTP httpClientRespond_default(struct HTTP request);

// Works with a http client. recommended to execute it in a different
// thread. Sets finished to 1 when the process is finished.
void httpClientWork(int client_fd, int* finished);
