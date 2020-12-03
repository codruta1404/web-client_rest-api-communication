#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *cookies, char *token) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "GET %s HTTP/1.1", url);

    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "HOST: %s ", host);
    compute_message(message, line);

    if (token != NULL) {
    	sprintf(line,"Authorization: Bearer %s", token);
    	compute_message(message, line);
	}

    if (cookies != NULL) {
        compute_message(message, cookies);   
	}

    compute_message(message, "");
    
    free(line);
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                            int body_data_fields_count, char *cookies, char *token) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    memset(line, 0, LINELEN);
    sprintf(line, "HOST: %s ", host);
    compute_message(message, line);
    
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %d", body_data_fields_count);
    compute_message(message, line);
    
    if (token != NULL) {
    	sprintf(line,"Authorization: Bearer %s", token);
    	compute_message(message, line);
	}

    if (cookies != NULL) {
        compute_message(message, cookies);   
	}

    compute_message(message, "");
   
    memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *token) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);
    
    memset(line, 0, LINELEN);
    sprintf(line, "HOST: %s ", host);
    compute_message(message, line);

    if (token != NULL) {
    	sprintf(line,"Authorization: Bearer %s", token);
    	compute_message(message, line);
	}

    compute_message(message, "");
    
    free(line);
    return message;
}
