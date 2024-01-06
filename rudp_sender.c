#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "rudp_api.h"
#include "event.h"

#define MAXPEERS 64		/* Max number of remote peers */
#define MAXPEERNAMELEN 256		/* Max length of peer name */
#define debug 1
/* 
 * Prototypes 
 */

void send_p();
int eventhandler(rudp_socket_t rsocket, rudp_event_t event, struct sockaddr_in *remote);

int main(int argc, char* argv[]) {
    send_p();
}

void send_p() {
    // create socket 
	rudp_socket_t rsocket = rudp_socket(0);
    if (rsocket == NULL) {
		fprintf(stderr, "vs_send: rudp_socket() failed\n");
		exit(1);
	}
	rudp_event_handler(rsocket, eventhandler);

    struct sockaddr_in peer;

    struct hostent* hp;
	struct in_addr *addr;

    int port = 1234;
    char *hoststr = "127.0.0.1\0";

    if (port <= 0) {
        fprintf(stderr, "Bad destination port: %d\n", 
            atoi(strchr(hoststr, ':') + 1));
        exit(1);
    }
    if ((hp = gethostbyname(hoststr)) == NULL || 
        (addr = (struct in_addr *)hp->h_addr) == NULL) {
        fprintf(stderr,"Can't locate host \"%s\"\n", hoststr); 
        return;
    }
    memset((char *)&peer, 0, sizeof(struct sockaddr_in));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    memcpy(&peer.sin_addr, addr, sizeof(struct in_addr));
    /* 
    * Socket termination
    */
    
    /* 
    * Send a datagram 
    */
    char *data_str = "Hello world!\0";
    int len = strlen(data_str);
    if (debug) {
        fprintf(stderr, "sender: send BEGIN \"%s\" (%d bytes) to %s:%d\n",
            "data_str", len, 
            inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
    }
    if (rudp_sendto(rsocket, data_str, len, &peer) < 0) {
        fprintf(stderr,"sender: send failure\n");
        rudp_close(rsocket);		
        return;
    }
    rudp_close(rsocket);

    eventloop(0);
	return;
}

int eventhandler(rudp_socket_t rsocket, rudp_event_t event, struct sockaddr_in *remote) {
	switch (event) {
	case RUDP_EVENT_TIMEOUT:
		if (remote) {
			fprintf(stderr, "sender: time out in communication with %s:%d\n",
				inet_ntoa(remote->sin_addr),
				ntohs(remote->sin_port));
		}
		else {
			fprintf(stderr, "sender: time out\n");
		}
		exit(1);
		break;
	case RUDP_EVENT_CLOSED:
		if (debug) {
			fprintf(stderr, "sender: socket closed\n");
		}
		break;
	}
	return 0;
}
