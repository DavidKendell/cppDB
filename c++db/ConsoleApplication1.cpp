#include <iostream>
#include "Controller.h"
#include <mysqlx/xdevapi.h>
#include <fstream>
#include <sstream>

using namespace mysqlx;
Schema setupSchema(Session& session, std::string script)
try
{
	std::ifstream sqlScript(script);
	std::string line;
	while (!sqlScript.eof()) {
		getline(sqlScript, line);
		string query = line;
		session.sql(query).execute();
	}
	return session.getSchema("product");
}
catch (std::exception e) {
	std::cout << e.what();
}

template <typename Number>
Number getValidNumber(std::string errormessage) {
	Number number;
	std::string line;
	getline(std::cin, line);
	std::istringstream iss(line);
	while (!(iss >> number)) {
		std::cout << errormessage << '\n';
		iss.clear();
		getline(std::cin, line);
		iss.str(line);
	}
	
	return number;
}
void mandatoryString(std::string& str, std::string errormessage) {
	
	getline(std::cin, str);
	while (str == "") {
		std::cout << errormessage << '\n';
		getline(std::cin, str);
	}
}

int main()
{
	Session session(SessionOption::HOST, "localhost",
		SessionOption::PORT, 33060,
		SessionOption::USER, "root",
		SessionOption::PWD, "root");
	Schema productdb = session.getSchema("productdbtest");
	Table products = productdb.getTable("product");
	Controller controller(products);
	int choice, id;
	double price;
	std::string search, name;
	std::cout << "1: List all products" << std::endl;
	std::cout << "2: Search for product by name" << std::endl;
	std::cout << "3: Show products by maximum price" << std::endl;
	std::cout << "4: Add new product" << std::endl;
	std::cout << "5: Delete existing product" << std::endl;
	std::cout << "0: Exit" << std::endl;
	std::cout << "Enter number of choice: ";
	choice = getValidNumber<int>("Enter a valid integer");
	
	while (choice != 0) {
		
		switch (choice) {
		case 1:
			// TODO - Enter function to get all entries from table
			std::cout << "Print all items from database" << std::endl;
			std::cout << controller.formatAll();
			break;
		case 2:
			// TODO - Enter function to return entries based on search term
			std::cout << "Enter search term for name of product:\n";
			mandatoryString(name, "name must not be blank");
			std::cout << controller.findByName(name);
			break;
		case 3:
			// TODO - Enter function to show products by max price based on entered number
			std::cout << "Enter max price of items to show:\n";
			price = getValidNumber<double>("Enter a valid floating point number");
			std::cout << controller.filterPrice(price);
			break;
		case 4:
			// TODO - Enter function for adding new product
		{
			std::cout << "Enter new product name:\n";
			
			mandatoryString(name, "name must not be blank");
			std::string description;
			std::cout << "Enter a description (optional, input <blank> to skip)\n";
			getline(std::cin, description);
			std::cout << "Enter product category\n";
			std::string category;
			mandatoryString(category, "category must not be blank");
			std::cout << "Enter new product price:\n";
			price = getValidNumber<double>("Enter a valid floating point number");
			controller.addProduct(name, description, category, price);
			break;
		}
		case 5:
			// TODO - Enter function for deleteing existing product
			std::cout << "Enter product id to be deleted:\n";
			id = getValidNumber<int>("Enter a valid integer");
			if (controller.deleteProduct(id)) {
				std::cout << "Deleted the product with id " << id << '\n';
			}
			else {
				std::cout << "No product found with that id\n";
			}
			break;
		case 0:
			std::cout << "Exiting" << std::endl;
			return 0;
		}
		std::cout << "Enter number of choice: ";
		choice = getValidNumber<int>("Enter a valid integer");
	}
}