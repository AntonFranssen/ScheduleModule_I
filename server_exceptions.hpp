#include <string>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

class Exception {
public:
	unsigned version;
	bool is_keep_alive;
	http::response<http::string_body> what();
	std::string error;

	http::status st;
};

class Internal_server_error : public Exception { };
class Resource_not_found_error : public Exception { };
class Bad_request_error : public Exception { };