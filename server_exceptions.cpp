#include "server_exceptions.hpp"

http::response<http::string_body> Exception::what() {
	http::response<http::string_body> response{
		st, version
	};
	response.set(http::field::server, "ScheduleServer.com");
	response.set(http::field::content_type, "text/html");
	response.keep_alive(this->is_keep_alive);
	response.body() = this->error;
	response.prepare_payload();

	return response;
}

