#include "Controller.h"
#include <iomanip>

Controller::Controller(Table products) : products(products)
{
}

std::string Controller::formatAll()
{
	auto rows = products.select("*").execute();
	return formatTable(rows);
}

std::string Controller::findByName(std::string name)
{
	RowResult rows = products.select("*").where("productname like :name").bind("name", '%' + name + '%').execute();
	return formatTable(rows);
}

std::string Controller::filterPrice(double maxPrice)
{
	RowResult rows = products.select("*").where("price <= :maxPrice").bind("maxPrice", maxPrice).execute();
	return formatTable(rows);
}

void Controller::addProduct(std::string productname, std::string productdescription, std::string category, double price)
{
	Row row = Row(Value(productname), Value(productdescription), Value(category), Value(price));
	if (productdescription == "") {
		row[1] = Value(nullptr);
	}
	products.insert("productname", "productdescription", "category", "price").values(row).execute();
}

bool Controller::deleteProduct(int id)
{
	auto result = products.remove().where("pid = :id").bind("id", id).execute();
	return result.getAffectedItemsCount();
}

std::string Controller::formatTable(RowResult& table)
{
	auto& columns = table.getColumns();
	int height = table.count() + 1;
	auto tableValues = table.fetchAll();
	if (height <= 1) {
		return "Nothing found\n";
	}
	int width = table.getColumnCount();
	int maxwidth = 50;
	std::vector<int> columnwidths(width);
	std::vector<std::string> v;
	v.reserve(width * height);

	for (int i = 0; i < width; ++i) {
		columnwidths.at(i) = std::max(columnwidths.at(i), (int)columns[i].getColumnName().length());
		columnwidths.at(i) = std::min(columnwidths.at(i), maxwidth);
		v.push_back(columns[i].getColumnName());
	}
	for (const auto& row : tableValues) {
		for (int i = 0; i < row.colCount(); ++i) {
			std::string s = [](Value v) {
				switch (v.getType()) {
				case 0:
					return std::string("NULL");
				case 2:
					return std::to_string(int(v));
				case 3: {
					std::stringstream ffloat;
					ffloat << std::fixed << std::setprecision(2) << float(v);
					return ffloat.str();
				}
				case 4:
				{
					std::stringstream fdouble;
					fdouble << std::fixed << std::setprecision(2) << double(v);
					return fdouble.str();
				}
				case 6:
					return std::string(v);
				}
			}(row[i]);
			columnwidths.at(i) = std::max(columnwidths.at(i), (int)s.length());
			columnwidths.at(i) = std::min(columnwidths.at(i), maxwidth);
			v.push_back(s);
		}
	}
	std::stringstream formattedTable;
	for (int i = 0; i < height; ++i) {
		formattedTable << "\n";
		for (int j = 0; j < width; ++j) {
			formattedTable << '+' << std::string(columnwidths.at(j) + 2, '-');
		}
		formattedTable << "+\n| ";


		for (int j = 0; j < width; ++j) {
			formattedTable << std::setw(columnwidths.at(j)) << std::left;
			if (v.at(i * width + j).length() > maxwidth) {
				formattedTable << v.at(i * width + j).substr(0, maxwidth - 3) + "...";
			}
			else {
				formattedTable << v.at(i * width + j);
			}
			formattedTable << " | ";
		}

	}

	formattedTable << "\n";
	for (int j = 0; j < width; ++j) {
		formattedTable << '+' << std::string(columnwidths.at(j) + 2, '-');

	}

	formattedTable << "+\n";

	return formattedTable.str();
}
