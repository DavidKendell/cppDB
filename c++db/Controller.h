#pragma once
#include <mysqlx/xdevapi.h>
using namespace mysqlx;
class Controller
{
public:
	Controller(Table products);
	Controller(Controller&) = delete;
	std::string formatAll();
	std::string findByName(std::string name);
	std::string filterPrice(double maxPrice);
	void addProduct(std::string productname,
		std::string productdescription, std::string category, double price);
	bool deleteProduct(int id);
private:
	std::string formatTable(RowResult& table);
	Table products;
};

