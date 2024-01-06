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
/* 
 * Prototypes 
 */

#define debug 1

int rudp_receiver(rudp_socket_t rsocket, struct sockaddr_in *remote, char *buf, int len);
int eventhandler(rudp_socket_t rsocket, rudp_event_t event, struct sockaddr_in *remote);


int main(int argc, char* argv[]) {
    rudp_socket_t rsock;
	int port = 1234;

	
    if (port <= 0) {
        fprintf(stderr, "Bad destination port: %s\n", argv[optind]);
        exit(1);
	}

	if (debug) {
		printf("RUDP receiver waiting on port %i.\n",port);
	}

	/*
	 * Create RUDP listener socket
	 */

	if ((rsock = rudp_socket(port)) == NULL) {
		fprintf(stderr,"vs_recv: rudp_socket() failed\n");
		exit(1);
	}

	/*
	 * Register receiver callback function
	 */

	rudp_recvfrom_handler(rsock, rudp_receiver);

	/*
	 * Register event handler callback function
	 */

	rudp_event_handler(rsock, eventhandler);

	/*
	 * Hand over control to event manager
	 */

	eventloop(0);

	return (0);
}

int rudp_receiver(rudp_socket_t rsocket, struct sockaddr_in *remote, char *buf, int len) {
    // if()
    fprintf(stdout, "buf length: %d\n", len);
    fprintf(stdout, "Receiver: %s\n", buf);

    return 0;

    // vsftp
	// struct rxfile *rx;
	// int namelen;
	// int i;

	// struct vsftp *vs = (struct vsftp *) buf;
	// if (len < VS_MINLEN) {
	// 	fprintf(stderr, "vs_recv: Too short VSFTP packet (%d bytes)\n",
	// 		len);
	// 	return 0;
	// }
	// rx = rxfind(remote);
	// switch (ntohl(vs->vs_type)) {
	// case VS_TYPE_BEGIN:
	// 	namelen = len - sizeof(vs->vs_type);
	// 	if (namelen > VS_FILENAMELENGTH)
	// 		namelen = VS_FILENAMELENGTH;
	// 	strncpy(rx->name, vs->vs_info.vs_filename, namelen);
	// 	rx->name[namelen] = '\0'; /* Null terminated */

	// 	/* Verify that file name is valid
	// 	 * Only alpha-numerical, period, dash and
	// 	 * underscore are allowed */
	// 	for (i = 0; i < namelen; i++) {
	// 		char c = rx->name[i];
	// 		if (!(isalnum(c) || c == '.' || c == '_' || c == '-')) {
	// 			fprintf(stderr, "vs_recv: Illegal file name \"%s\"\n", 
	// 				rx->name);
	// 			rudp_close(rsocket);
	// 			return 0;
	// 		}
	// 	}

	// 	if (debug) {
	// 		fprintf(stderr, "vs_recv: BEGIN \"%s\" (%d bytes) from %s:%d\n", rx->name, len,
	// 			inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));
	// 	}
	// 	if ((rx->fd = creat(rx->name, 0644)) < 0) {
	// 		perror("vs_recv: create");
	// 		rudp_close(rsocket);
	// 	}
	// 	else {
	// 		rx->fileopen = 1;
	// 	}
	// 	break;
	// case VS_TYPE_DATA:
	// 	if (debug) {
	// 		fprintf(stderr, "vs_recv: DATA (%d bytes) from %s:%d\n", 
	// 			len, 
	// 			inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));
	// 	}
	// 	len -= sizeof(vs->vs_type);
	// 	/* len now is length of payload (data or file name) */
	// 	if (rx->fileopen) {
	// 		if ((write(rx->fd, vs->vs_info.vs_filename, len)) < 0) {
	// 			perror("vs_recv: write");
	// 		}
	// 	}
	// 	else {
	// 		fprintf(stderr, "vs_recv: DATA ignored (file not open)\n");
	// 	}
	// 	break;
	// case VS_TYPE_END:
	// 	if (debug) {
	// 		fprintf(stderr, "vs_recv: END (%d bytes) from %s:%d\n",
	// 			len, inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));
	// 	}
	// 	printf("vs_recv: received end of file \"%s\"\n", rx->name);
	// 	if (rx->fileopen) {
	// 		close(rx->fd);
	// 		rxdel(rx);
	// 	}
	// 	/* else ignore */
	// 	break;
	// default:
	// 	fprintf(stderr, "vs_recv: bad vsftp type %d from %s:%d\n",
	// 		vs->vs_type, inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));
	// }
	// return 0;
}

int eventhandler(rudp_socket_t rsocket, rudp_event_t event, struct sockaddr_in *remote) {

	switch (event) {
        case RUDP_EVENT_TIMEOUT:
            if (remote) {
                fprintf(stderr, "recver: time out in communication with %s:%d\n",
                    inet_ntoa(remote->sin_addr),
                    ntohs(remote->sin_port));
            }
            else {
                fprintf(stderr, "recver: time out\n");  
            }
            break;
        case RUDP_EVENT_CLOSED:
            if (remote) {
                fprintf(stderr, "recver: close communication with %s:%d\n",
                    inet_ntoa(remote->sin_addr),
                    ntohs(remote->sin_port));
            } /* else ignore */
            break;
        default:
            fprintf(stderr, "recver: unknown event %d\n", event);
            break;
    }
	return 0;
}
