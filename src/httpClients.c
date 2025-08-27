#include <httpClients.h>
#include <sys/socket.h>
#include <essentials.h>
#include <unistd.h>
#include <httpHeaders.h>
#include <string.h>


httpClientResponder responder = httpClientRespond_default;

void
setHttpClientRespond(httpClientResponder r)
{
	responder = r;
}

struct HTTP
httpClientRespond_default(struct HTTP request)
{
	struct HTTP response = HTTPResponse("HTTP/1.1","200","OK");
	response.data = "<h1>A</h1>";
	addPair(&response, "Content-Length", " 10");
	return response;
}

void
httpClientWork(int client_fd, int* finished)
{
#ifdef SERVER_DEBUG2
	printf("HTTPCLIENT::CLIENT_FD:%i\n",client_fd);
#endif
	DEB("HTTPCLIENT::HTTP CLIENT WORK\n");
	char buf[2048];
	DEB("HTTPCLIENT::CALLING RECV\n");
	ssize_t readens = read(client_fd, buf, 2048);
	if (readens < 0) {
		DEB("HTTPCLIENT:: READED NON! SKIPPING\n");
		goto httpclient_finish;
	}
	DEB("HTTPCLIENT::OK\n");
#ifdef SERVER_DEBUG
	printf("HTTPCLIENT:: REQUEST '%s'\n", readens, buf);
#endif
	struct HTTP request = parseHTTP(buf);
	DEB("HTTPCLIENT::PARSED\n");
#ifdef SERVER_REQUEST_LOG
	printf("HTTPCLIENT::%s request to %s.\n", request.type, request.page);
#endif
	struct HTTP response = responder(request);	

	char* out = "HTTP/1.0 500\r\n\r\n";
	out = mergeHTTP(response);
	DEB("HTTPCLIENT::MERGE COMPLETE\n");
	send(client_fd, out, strlen(out), 0);
	DEB("HTTPCLIENT::RESPONSE SENT\n");
#ifdef SERVER_DEBUG
	printf("HTTPCLIENT::%s\n",out);
#endif

httpclient_finish:
	*finished = 1;
}
