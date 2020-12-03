#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "parson.c"

char *register_login_helper(char *username, char *password) {
    char temp1, temp2;

    printf("username=");
	scanf("%c",&temp1); // temp imi face clear la buffer
	scanf("%[^\n]",username);

    printf("password=");
	scanf("%c",&temp2); // temp imi face clear la buffer
	scanf("%[^\n]",password);

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *user = NULL;
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);
    user = json_serialize_to_string_pretty(root_value);
    json_value_free(root_value);

    return user;
}

char *add_book_helper(char *title, char *author, char *genre, 
                        char *publisher, int page_count) {
    char temp1, temp2, temp3, temp4;

    printf("title=");
    scanf("%c", &temp1); // temp imi face clear la buffer
	scanf("%[^\n]", title);

    printf("author=");
    scanf("%c", &temp2); 
	scanf("%[^\n]", author);

    printf("genre=");
    scanf("%c", &temp3); 
	scanf("%[^\n]", genre);

    printf("publisher=");
    scanf("%c", &temp4); 
	scanf("%[^\n]", publisher);

    printf("page_count=");
    scanf("%d", &page_count);

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *book_payload = NULL;
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_number(root_object, "page_count", page_count);
    json_object_set_string(root_object, "publisher", publisher);
    book_payload = json_serialize_to_string_pretty(root_value);
    json_value_free(root_value);

    return book_payload;
}

char *get_cookie(char *response) {
	char *str, *str1, *my_cookie;
	my_cookie =  calloc(1000, sizeof(char));
	str = strstr(response, "Set-");
    str = str + strlen("Set-");
    str1 = strtok(str, ";");
    strcpy(my_cookie, str1);

	return my_cookie;
}

char *get_token(char *response) {
    char *str, *str1, *my_token;
	my_token =  calloc(1000, sizeof(char));
	str = strstr(response, "{\"token\":\"");
    str = str + strlen("{\"token\":\"");
    str1 = strtok(str, "\"");
    strcpy(my_token, str1);

    return my_token;
}

char *get_server_id(char *response) {
    char *str, *str1, *my_message;
	my_message =  calloc(10, sizeof(char));
	str = strstr(response, "HTTP/1.1 ");
    str = str + strlen("HTTP/1.1 ");
    str1 = strtok(str, "0");
    strcpy(my_message, str1);

	return my_message;
}

char *get_server_error(char *response) {
    char *str, *str1, *my_message;
	my_message =  calloc(100, sizeof(char));
	str = strstr(response, "error\":\"");
    str = str + strlen("error\":\"");
    str1 = strtok(str, "\"");
    strcpy(my_message, str1);

	return my_message;
}

char *get_list_of_books(char *response) {
    char *str, *str1, *my_message;
	my_message =  calloc(1000, sizeof(char));
    strcpy(my_message, "[");
	str = strstr(response, "[");
    str = str + strlen("[");
    str1 = strtok(str, "]");

    /* Pentru cazul in care biblioteca este goala */
    if(str1 != NULL) {
        strcat(my_message, str1); 
    }

    strcat(my_message, "]");
    
	return my_message;
}

void get_id_title_books(char *response) {
    JSON_Value *root_value;
    JSON_Array *arrays;
    JSON_Object *array;
    size_t i;
    
    root_value = json_parse_string(get_list_of_books(response));
    if (json_value_get_type(root_value) != JSONArray) {
        printf("Incorrect json format!\n");
        return;
    }
    
    arrays = json_value_get_array(root_value);
    for (i = 0; i < json_array_get_count(arrays); i++) {
        array = json_array_get_object(arrays, i);
        printf("id=%d, title=%s\n",
               (int)json_object_get_number(array, "id"),
               json_object_get_string(array, "title"));
    }
    
    json_value_free(root_value);
}

/* Intoarce detaliile din raspunsul server-ului despre o carte */
char *get_book_details(char *response) {
    char *str, *str1, *my_message;
	my_message =  calloc(100, sizeof(char));
    strcpy(my_message, "[");
	str = strstr(response, "[");
    str = str + strlen("[");
    str1 = strtok(str, "]");
    strcat(my_message, str1); 
    strcat(my_message, "]");
    
	return my_message;
}

/* Converteste detaliile despre carte din json in string */
void get_book_helper(char *response) {
    JSON_Value *root_value;
    JSON_Array *arrays;
    JSON_Object *array;
    size_t i;
    
    root_value = json_parse_string(get_book_details(response));
    if (json_value_get_type(root_value) != JSONArray) {
        return;
    }
    
    arrays = json_value_get_array(root_value);
    for (i = 0; i < json_array_get_count(arrays); i++) {
        array = json_array_get_object(arrays, i);
        printf("title=%s\nauthor=%s\npublisher=%s\ngenre=%s\npage_count=%d\n",
               json_object_get_string(array, "title"),
               json_object_get_string(array, "author"),
               json_object_get_string(array, "publisher"),
               json_object_get_string(array, "genre"),
               (int)json_object_get_number(array, "page_count"));
    }

    json_value_free(root_value);
}

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    char *cookie = NULL;
    char *token = NULL;
    int sockfd;

    char *command = (char*)calloc(100, sizeof(char));

    while (1) {
        
        sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        scanf("%s", command); 

        /*----------REGISTER----------*/
        if (strcmp(command, "register") == 0) {
            char *username = (char*)calloc(20, sizeof(char));
            char *password = (char*)calloc(20, sizeof(char));
            char *payload = (char*)calloc(100, sizeof(char));
            payload = register_login_helper(username, password);
            int message_length = strlen(payload);

            message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", 
                "/api/v1/tema/auth/register", "application/json", payload, message_length, NULL, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *temp_id = (char*)calloc(5000, sizeof(char));
            strcpy(temp_id, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp_id);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
            } 
            else {
                printf("User created!\n");
            }

            free(response);
            free(message);
            free(username);
            free(password);
            free(temp_id);
            json_free_serialized_string(payload);
            
            continue;
        }

        /*----------LOGIN----------*/
        if (strcmp(command, "login") == 0) {
            char *username = (char*)calloc(20, sizeof(char));
            char *password = (char*)calloc(20, sizeof(char));
            char *payload = (char*)calloc(100, sizeof(char));
            payload = register_login_helper(username, password);
            int message_length = strlen(payload);

            message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", 
                "/api/v1/tema/auth/login", "application/json", payload, message_length, NULL, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            if (strstr(response, "Cookie") != NULL) {
                char *temp = (char*)calloc(5000, sizeof(char));
                strcpy(temp, response);
                cookie = get_cookie(temp);
            }

            char *temp_id = (char*)calloc(5000, sizeof(char));
            strcpy(temp_id, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp_id);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
            } 
            else {
                printf("You are logged in!\n");
            }
            
            free(response);
            free(message);
            free(username);
            free(password);
            free(temp_id);
            json_free_serialized_string(payload);

            continue;
        }

        /*----------ENTER_LIBRARY----------*/
        if (strcmp(command, "enter_library") == 0) {
            if (cookie == NULL) {
                printf("You are not logged in!\n");
                continue;
            }

            message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                "/api/v1/tema/library/access", cookie, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            
            char *temp1 = (char*)calloc(5000, sizeof(char));
            strcpy(temp1, response);
            token = get_token(temp1);
            
            char *temp_id = (char*)calloc(5000, sizeof(char));
            strcpy(temp_id, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp_id);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
                continue;
            } 
            else {
                printf("You are in the library!\n");
            }
            
            free(response);
            free(message);
            free(temp_id);

            continue;
        }

        /*----------GET_BOOKS----------*/
        if (strcmp(command, "get_books") == 0) {
            if (cookie == NULL) {
                printf("You are not logged in!\n");
                continue;
            }

            if (token == NULL) {
                printf("You are not in the library!\n");
                continue;
            }
            message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                "/api/v1/tema/library/books", cookie, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *temp_id = (char*)calloc(5000, sizeof(char));
            strcpy(temp_id, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp_id = get_server_id(temp_id);

            if (strcmp(resp_id, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
                continue;
            } 

            char *temp_id_title_book = (char*)calloc(5000, sizeof(char));
            strcpy(temp_id_title_book, response);
            char *resp = get_list_of_books(temp_id_title_book);

            if (strcmp(resp, "[]") == 0) {
                printf("Biblioteca este goala!\n");
            }

            /* Converteste id si title din json in string */
            get_id_title_books(temp_id_title_book);
            
            free(response);
            free(message);
            free(temp_id);
            free(temp_id_title_book);

            continue;
        }

        /*----------GET_BOOK----------*/
        if (strcmp(command, "get_book") == 0) {
            if (cookie == NULL) {
                printf("You are not logged in!\n");
                continue;
            }

            if (token == NULL) {
                printf("You are not in the library!\n");
                continue;
            }

            int id;
            printf("id=");
            scanf("%d", &id);
            char *str_id = (char*)calloc(20, sizeof(char));
            sprintf(str_id, "%d", id);
            char old_url[50] = "/api/v1/tema/library/books/";
            char *url_id = (char*)calloc(100, sizeof(char));
            url_id = strcat(old_url, str_id);

            message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                url_id, cookie, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *temp = (char*)calloc(5000, sizeof(char));
            strcpy(temp, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
                continue;
            } 

            char *temp_book = (char*)calloc(5000, sizeof(char));
            strcpy(temp_book, response);
            /* Afisare detalii despre carte */
            get_book_helper(temp_book);

            free(response);
            free(temp);
            free(temp_book);
            free(message);

            continue;
        }

        /*----------ADD_BOOK----------*/
        if (strcmp(command, "add_book") == 0) {
            if (cookie == NULL) {
                printf("You are not logged in!\n");
                continue;
            }

            if (token == NULL) {
                printf("You are not in the library!\n");
                continue;
            }

            char *title = (char*)calloc(50, sizeof(char));
            char *author = (char*)calloc(100, sizeof(char));
            char *genre = (char*)calloc(50, sizeof(char));
            char *publisher = (char*)calloc(100, sizeof(char));
            char *payload_add_book = (char*)calloc(1000, sizeof(char));
            int page_count = 0;
            payload_add_book = add_book_helper(title, author, genre, publisher, page_count);
            int book_length = strlen(payload_add_book);

            message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", 
                "/api/v1/tema/library/books", "application/json", payload_add_book, book_length, cookie, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *temp = (char*)calloc(5000, sizeof(char));
            strcpy(temp, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
            }
            else {
                printf("Book added!\n");
            }

            free(temp);
            free(response);
            free(message);
            free(title);
            free(author);
            free(genre);
            free(publisher);
            json_free_serialized_string(payload_add_book);

            continue;
        }

        /*----------DELETE_BOOK----------*/
        if (strcmp(command, "delete_book") == 0) {
            if (cookie == NULL) {
                printf("You are not logged in!\n");
                continue;
            }

            if (token == NULL) {
                printf("You are not in the library!\n");
                continue;
            }

            int id_del;
            printf("id=");
            scanf("%d", &id_del);
            char *str_id_del = (char*)calloc(20, sizeof(char));
            sprintf(str_id_del, "%d", id_del);
            char old_url_del[50] = "/api/v1/tema/library/books/";
            char *url_id_del = (char*)calloc(100, sizeof(char));
            url_id_del = strcat(old_url_del, str_id_del);

            message = compute_delete_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                url_id_del, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *temp = (char*)calloc(5000, sizeof(char));
            strcpy(temp, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
            }
            else {
                printf("Book deleted!\n");
            }            

            free(response);
            free(message);
            free(temp);
            
            continue;
        }
        
        /*----------LOGOUT----------*/
        if (strcmp(command, "logout") == 0) {
            if (cookie == NULL) {
                printf("You are not logged in!\n");
                continue;
            }

            message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                "/api/v1/tema/auth/logout", cookie, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
        
            char *temp = (char*)calloc(5000, sizeof(char));
            strcpy(temp, response);
            /* Iau prima cifra din statusul raspunsului*/
            char *resp = get_server_id(temp);

            if (strcmp(resp, "4") == 0) {
                char *err = get_server_error(response);
                printf("Error: %s\n", err);
            }
            else {
                printf("You are logged out! Bye-bye, baby!\n");
            }

            free(response);
            free(message);
            free(temp);

            continue;
        }

        /*----------EXIT----------*/
        if (strcmp(command, "exit") == 0) {
            exit(0);
        }

        printf("Command not found! Type another command!\n");

        close(sockfd);
    }

    free(command);

    if (cookie != NULL) {
        free(cookie);
    }

    if (token != NULL) {
        free(token);
    }

    return 0;
}
