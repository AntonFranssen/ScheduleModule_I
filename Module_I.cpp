#include <iostream>
#include "http_server_interface.h"

Server_interface server("127.0.0.1", 8080);

int main() {
	setlocale(LC_ALL, "Russian");
	server.accept_connections();
}
