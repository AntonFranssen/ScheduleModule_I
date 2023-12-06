/*#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <cassert>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace bsoncxx::builder::basic;

namespace http = boost::beast::http;

mongocxx::instance instance{};// don't put inside main 

int main() {
	const std::string host = "mongodb://localhost";
	const std::string target = "use Zalupa";

	boost::asio::io_context ioc;

	boost::asio::ip::tcp::resolver resolver(ioc);
	boost::asio::ip::tcp::socket socket(ioc);

	boost::asio::connect(socket, resolver.resolve(host, "27017"));

	http::request<http::string_body> req(http::verb::get, target, 11);

	req.set(http::field::host, host);
	req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	http::write(socket, req);

	boost::beast::flat_buffer buffer;
	http::response<http::dynamic_body> res;
	http::read(socket, buffer, res);

	std::cout << res << std::endl;

	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	//std::cout << "Hello, world!" << std::endl;

	return 0;

	//mongocxx::client conn{ mongocxx::uri{ "mongodb://localhost:27017" } };
	//auto coll = conn["test"]["coll"];

	//coll.insert_one(doc_value2.view());
	/*auto insert_one_result = coll.insert_one(make_document(kvp("i", 0)));*/

	//auto cursor_all = coll.find({});
	//std::cout << "collection " << coll.name()
		//<< " contains these documents:" << std::endl;



	//for (auto doc : cursor_all) {
	//	std::cout << bsoncxx::to_json(doc) << std::endl;
	//}
	//std::cout << std::endl;
	//return 0;
//}

#include <iostream>
#include "http_server_interface.h"

Server_interface server("127.0.0.1", 8080);

int main() {
	setlocale(LC_ALL, "Russian");
	server.accept_connections();
}
