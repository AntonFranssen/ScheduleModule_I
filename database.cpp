#include "database.h"

database_interface::database_interface() {
	this->connection = mongocxx::client{ mongocxx::uri {"mongodb+srv://AntonFrantsen:123@schedule.7262yqm.mongodb.net/?retryWrites=true&w=majority"} };
	this->db = this->connection["schedule"];
}

void database_interface::insert_collection(std::string collection_name, std::string data) {
	mongocxx::collection collection = this->db[collection_name];

	bsoncxx::document::value document = bsoncxx::from_json(data);
	std::vector<bsoncxx::document::value> docs;
	docs.push_back(document);
	collection.insert_many(docs);
}

std::string database_interface::query_document(std::string collection_name, std::string key_filter, std::string value_filter) {
	try {
		mongocxx::collection collection = this->db[collection_name];
		auto queried_doc = collection.find_one(make_document(kvp(key_filter, value_filter)));
		bsoncxx::document::value qd_val = queried_doc.value();
		if (queried_doc) {
			std::string queried_doc_str = bsoncxx::to_json(qd_val.view());
			return queried_doc_str;
		}
	}
	catch (std::exception& e) {
		return "-1";
	}
	
}

std::string database_interface::query_collection(std::string collection_name) {
	try {
		mongocxx::collection collection = this->db[collection_name];
		auto cursor_all = collection.find({});
		std::string collection_str = "[";

		for (auto doc : cursor_all) {
			collection_str += bsoncxx::to_json(doc);
			collection_str += ',';
		}

		collection_str.erase(collection_str.end() - 1);

		collection_str += ']';
		return collection_str;
	}
	catch (std::exception& e) {
		return "-1";
	}
}

void database_interface::add_comment(std::string collection_name, std::string day, std::string class_num, std::string comment) {
	mongocxx::collection collection = this->db[collection_name];
	collection.update_one(
		make_document(kvp("day", day), kvp("array.number", class_num)),
		make_document(
			kvp("$set", make_document(kvp("array.$.Comment", comment)))
		)
	);
}

void database_interface::drop_schedule(std::string collection_name_1, std::string collection_name_2) {
	mongocxx::collection collection_1 = this->db[collection_name_1];
	collection_1.delete_many({});

	mongocxx::collection collection_2 = this->db[collection_name_2];
	collection_2.delete_many({});
}

void database_interface::drop_schedule(std::string collection_name) {
	mongocxx::collection collection_1 = this->db[collection_name];
	collection_1.delete_many({});
}
