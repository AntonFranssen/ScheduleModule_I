#include "http_server_interface.h"

Server_interface::Server_interface(std::string host, unsigned short port) {
	this->host = net::ip::make_address(host);
	this->port = port;
}

template <class Body, class Allocator>
http::message_generator Server_interface::handle_request(http::request<Body, http::basic_fields<Allocator>>&& request) {
	std::vector request_tokens = parse_request_string(request.target());
	
	try {
		if (request.target().empty() ||
			request.target()[0] != '/' ||
			request.target().find("..") != beast::string_view::npos)
			throw Bad_request_error{ request.version(), request.keep_alive(), "Illegal request-target" };

		switch (request.method()) {
			case http::verb::get:
			{
				std::string response_body;
				if (request_tokens.size() == 3)
					response_body = mongo_schedule_db.query_document(request_tokens[PARITY], "day", request_tokens[DAY]);
				else if (request_tokens.size() < 3)
					response_body = mongo_schedule_db.query_collection(request_tokens[PARITY]);

				if (response_body == "-1")
					throw Resource_not_found_error{ request.version(), request.keep_alive(), "Requested week type not found" };

				http::response<http::string_body> response{
					std::piecewise_construct,
					std::make_tuple(std::move(response_body)),
					std::make_tuple(http::status::ok, request.version()),
				};
				response.set(http::field::server, "RedArmy.il");
				response.prepare_payload();
				response.keep_alive(response.keep_alive());
				return response;
			}
			break;

			case http::verb::post:
			{
				if (request_tokens[2] == "add") {
					std::cout << request.body() << std::endl;

					try {
						mongo_schedule_db.insert_collection(request_tokens[PARITY], request.body());
					} 
					catch (std::exception &e) {
						throw Internal_server_error{ request.version(), request.keep_alive(), "Failed to load data from string" };
					}
					
					http::response<http::string_body> response{
						http::status::ok, request.version(),
					};
					response.set(http::field::server, "RedArmy.il");
					response.body() = "it is all OK!";
					response.prepare_payload();
					response.keep_alive(response.keep_alive());
					return response;
				}
				else if (request_tokens.size() == 5 && request_tokens[4] == "comment") {
					try {
						mongo_schedule_db.add_comment(request_tokens[PARITY], request_tokens[DAY], request_tokens[CLASS_NUM], request.body());
					}
					catch (std::exception& e) {
						throw Internal_server_error{ request.version(), request.keep_alive(), "Failed to load data from string" };
					}

					http::response<http::string_body> response{
						http::status::ok, request.version(),
					};
					response.set(http::field::server, "RedArmy.il");
					response.body() = "it is all OK!";
					response.prepare_payload();
					response.keep_alive(response.keep_alive());
					return response;
				}
				else if (request_tokens[1] == "drop") {
					if (request_tokens.size() == 3) {
						mongo_schedule_db.drop_schedule(request_tokens[2]);
					}
					else {
						mongo_schedule_db.drop_schedule("odd", "even");
					}
					http::response<http::string_body> response{
						http::status::ok, request.version(),
					};
					response.set(http::field::server, "RedArmy.il");
					response.body() = "it is all OK!";
					response.prepare_payload();
					response.keep_alive(response.keep_alive());
					return response;
				}
				else {
					throw Internal_server_error{ request.version(), request.keep_alive(), "OOPS! The developer is probably a dummy :3" };
				}
			}
			break;
		}
	} 
	catch (Exception e) {
		return e.what();
	}
}

void fail(boost::beast::error_code ec, std::string what) {
	std::cout << what << ":" << ec.message() << std::endl;
}

void Server_interface::manage_session(tcp::socket&& connection) {
	boost::beast::flat_buffer read_req_buffer;
	boost::beast::error_code ec;

	while (true) {
		http::request<http::string_body> request;
		http::read(connection, read_req_buffer, request, ec);

		if (ec == http::error::end_of_stream) break;
		if (ec) return fail(ec, "read");

		http::message_generator response_message = handle_request(std::move(request));

		bool keepalive = response_message.keep_alive();
		boost::beast::write(connection, std::move(response_message), ec);

		if (ec) return fail(ec, "write");

		if (!keepalive) break;
	}
	connection.shutdown(tcp::socket::shutdown_send);
}

void Server_interface::accept_connections() {
	tcp::acceptor connections_acceptor{ this->ioc, {this->host, this->port} };
	while (true) {
		tcp::socket new_connection{ this->ioc };
		connections_acceptor.accept(new_connection);
		std::cout << "Connected:" << new_connection.remote_endpoint() << std::endl;

		/*std::thread thr([&] {
			manage_session(std::move(new_connection));
			});*/

		std::thread thr(&Server_interface::manage_session, this, std::move(new_connection));
		thr.detach();
	}
}
	