#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "othello.h"
#include "matrix.h"

#include "cJSON.h"

/* Type de données représentant les réponses reçues du serveur */
struct server_message_s {
	char *status;
};
typedef struct server_message_s server_message;

struct server_board_s {
	char *data;
	int width;
	int height;
};
typedef struct server_board_s server_board;

struct server_message_board_s {
	server_message msg;
	server_board board;
	bool your_turn;
	char your_color;
};
typedef struct server_message_board_s server_message_board;

struct server_message_gameover_s {
	server_message msg;
	server_board board;
	char winner;
};
typedef struct server_message_gameover_s server_message_gameover;

struct server_message_wrongmove_s {
	server_message msg;
	server_board board;
};
typedef struct server_message_wrongmove_s server_message_wrongmove;

/* Connexion au serveur.
 * La fonction modifie ses paramètres `chan_in` et `chan_out` afin de fournir
 * deux valeurs de type `FILE *` permettant d'envoyer des données au serveur et
 * en recevoir.
 */
void connect_to_server(const char *hostname, int port, FILE **chan_in, FILE **chan_out)
{
	int sock;

	struct addrinfo hints = {0};
	struct addrinfo *results;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	int s = getaddrinfo(hostname, "42420", &hints, &results);
	printf("got 1\n");

	if(s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	printf("got 2\n");

	struct addrinfo *rp;
	for(rp = results; rp != NULL; rp = rp->ai_next) {
       	
		sock = socket(AF_INET, SOCK_STREAM, 0);
		// printf("got 3 %d\n", sock);
		
		// printf("%d %d %d %d\n", rp -> ai_flags, rp -> ai_family, rp -> ai_socktype, rp -> ai_protocol);

		// if(-1 != sock) {
			
		// 	printf("got 3 %d\n", connect(sock, rp->ai_addr, rp->ai_addrlen));

		// 	if(-1 == connect(sock, rp->ai_addr, rp->ai_addrlen)) {
		// 		close(sock);
		// 	}
		// }

		//sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sock == -1) {
			continue;
		}

		if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) {
			break; // Successfully connected, so break the loop
		}

		close(sock);

	}
	printf("got 3\n");

	freeaddrinfo(results);

	if(NULL == rp) {
		perror("connexion impossible");
		exit(EXIT_FAILURE);
	}

	*chan_out = fdopen(dup(sock), "rb");
	*chan_in = fdopen(dup(sock), "wb");
	close(sock);
}

/* Envoi d'un JSON déjà prêt au serveur */
void send_to_server(FILE *out_chan, cJSON *json)
{
	char *res = cJSON_PrintUnformatted(json);
	fprintf(out_chan, "%s\n", res);
	fflush(out_chan);
	free(res);
}

/* Envoi d'un mouvement au serveur */
void send_move(FILE *out_chan, int x, int y)
{
	cJSON *json = cJSON_CreateObject();
	cJSON *move_array = cJSON_CreateArray();
	cJSON_AddItemToArray(move_array, cJSON_CreateNumber(x));
	cJSON_AddItemToArray(move_array, cJSON_CreateNumber(y));
	cJSON_AddItemToObject(json, "do_move", move_array);
	send_to_server(out_chan, json);
	cJSON_Delete(json);
}

/* Envoi du message de salutation initiale au serveur */
void send_welcome(FILE *out_chan, const char *player_name, const char *game_name)
{
	cJSON *json = cJSON_CreateObject();
	cJSON_AddItemToObject(json, "name", cJSON_CreateString(player_name));
	cJSON_AddItemToObject(json, "game", cJSON_CreateString(game_name));
	send_to_server(out_chan, json);
	cJSON_Delete(json);
}

/* Fonction qui, à partir d'une ligne reçue du serveur et décrivant l'état du
 * plateau, renvoie la structure représentant ce message. */
server_message_board *parse_message_board(const cJSON *json)
{
	server_message_board *res = malloc(sizeof(server_message_board));

	cJSON *item;

	item = cJSON_GetObjectItemCaseSensitive(json, "status");
	if (cJSON_IsString(item) && item->valuestring != NULL) {
		res->msg.status = strdup(item->valuestring);
	}
	else {
		res->msg.status = NULL;
	}

	item = cJSON_GetObjectItemCaseSensitive(json, "board");
	res->board.data = strdup(item->valuestring);

	item = cJSON_GetObjectItemCaseSensitive(json, "width");
	res->board.width = item->valueint;

	item = cJSON_GetObjectItemCaseSensitive(json, "height");
	res->board.height = item->valueint;

	item = cJSON_GetObjectItemCaseSensitive(json, "your_turn");
	res->your_turn = cJSON_IsTrue(item);

	item = cJSON_GetObjectItemCaseSensitive(json, "your_color");
	res->your_color = item->valuestring[0];

	return res;
}

/* Lit un message gameover reçu du serveur */
server_message_gameover *parse_message_gameover(const cJSON *json)
{
	server_message_gameover *res = malloc(sizeof(server_message_gameover));
	
	cJSON *item;

	item = cJSON_GetObjectItemCaseSensitive(json, "status");
	if (cJSON_IsString(item) && item->valuestring != NULL) {
		res->msg.status = strdup(item->valuestring);
	}
	else {
		res->msg.status = NULL;
	}

	item = cJSON_GetObjectItemCaseSensitive(json, "board");
	res->board.data = strdup(item->valuestring);

	item = cJSON_GetObjectItemCaseSensitive(json, "width");
	res->board.width = item->valueint;

	item = cJSON_GetObjectItemCaseSensitive(json, "height");
	res->board.height = item->valueint;

	item = cJSON_GetObjectItemCaseSensitive(json, "winner");
	res->winner = item->valuestring[0];


	return res;
}

server_message *parse_message(const char *message)
{
	cJSON *json = cJSON_Parse(message);
	cJSON *item;

	item = cJSON_GetObjectItemCaseSensitive(json, "status");
	if (cJSON_IsString(item) && item->valuestring != NULL) {
		if(0 == strcmp(item->valuestring, "joined")) {
		}
		if(0 == strcmp(item->valuestring, "your_turn")) {
			server_message_board *res = parse_message_board(json);
			cJSON_Delete(json);
			return (server_message *)res;
		}
		if(0 == strcmp(item->valuestring, "move_done")) {
			server_message_board *res = parse_message_board(json);
			cJSON_Delete(json);
			return (server_message *)res;
		}
		if(0 == strcmp(item->valuestring, "wrong_move")) {
			server_message *res = malloc(sizeof(server_message));
			res->status = strdup(item->valuestring);
			return res;
		}
		if(0 == strcmp(item->valuestring, "game_over")) {
			server_message_gameover *res = parse_message_gameover(json);
			cJSON_Delete(json);
			return (server_message *)res;
		}
		return NULL;
	}
	else {
		server_message *res = malloc(sizeof(server_message));
		res->status = NULL;
		return res;
	}
}

int main() {

	FILE** chan_in = (FILE**)malloc(sizeof(FILE*) * 1000);
	FILE** chan_out = (FILE**)malloc(sizeof(FILE*) * 1000);	

	connect_to_server("c009-01", 42420, chan_in, chan_out);

	char* pls = "helloo";
	char* pls_b = "helloo";
	send_welcome(*chan_out, pls, pls_b);

}