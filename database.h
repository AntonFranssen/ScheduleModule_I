#ifndef DATABASE_H
#define DATABASE_H

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include <sstream>
#include <fstream>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace bsoncxx::builder::basic;

class database_interface {
private:
	mongocxx::instance instance{};
	mongocxx::client connection;
	mongocxx::database db;
public:
	database_interface();
	void insert_collection(std::string collection_name, std::string data_abs_path);

	std::string query_document(std::string collection_name, std::string key_filter, std::string value_filter);
	std::string query_collection(std::string collection_name);

	void add_comment(std::string collection_name, std::string day, std::string class_num, std::string comment);
};

#endif
