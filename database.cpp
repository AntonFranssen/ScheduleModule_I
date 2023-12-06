#include "database.h"

database_interface::database_interface() {
	this->connection = mongocxx::client{ mongocxx::uri {"mongodb://localhost:27017"} };
	this->db = this->connection["schedule"];
}

void database_interface::insert_collection(std::string collection_name, std::string data) {
	mongocxx::collection collection = this->db[collection_name];
	/*std::ifstream file_to_load(data_abs_path);
	
	std::string strh, str_json;
	while (getline(file_to_load, strh)) {
		str_json += strh;
	}*/

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
	std::string subfield_k;
	subfield_k = subfield_k + class_num + "." + "comment";
	bsoncxx::builder::basic::document subfields;
	subfields.append(kvp(subfield_k, comment));

	bsoncxx::builder::basic::document update;
	update.append(
		kvp("$set", subfields)
	);

	auto result = collection.update_one(
		make_document(kvp("day", day)),
		update.view()
	);
}