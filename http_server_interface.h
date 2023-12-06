#ifndef HTTP_SERVER_INTEERFACE
#define HTTP_SERVER_INTERFACE

#include <boost/beast/core.hpp>

#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <utility>
#include <tuple>

#include <memory>

#include "database.h"
#include "parse_req_str.hpp"
#include "server_exceptions.hpp"

namespace beast = boost::beast;         
namespace http = beast::http;         
namespace net = boost::asio;           
using tcp = boost::asio::ip::tcp;

class Server_interface {
private:
	net::io_context ioc {1};

	net::ip::address host;
	unsigned short port;

	database_interface mongo_schedule_db;

public:
	Server_interface(std::string host, unsigned short port);
	void accept_connections();

private:
	template <class Body, class Allocator> http::message_generator handle_request(http::request<Body, http::basic_fields<Allocator>>&& request);
	void manage_session(tcp::socket&& connection);

	void send_request();
};

#endif