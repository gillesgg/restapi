#include <string>


/// <summary>
/// DBMeal class to store the meal information
/// </summary>
class DBMeal
{
private:
	std::string name;
	int quantity;
	std::string price;

public:
	DBMeal() : name(""), quantity(0), price("") {}
	DBMeal(std::string name, int quantity, std::string price) : name(name), quantity(quantity), price(price) {}

	std::string get_name() const { return name; }
	int get_quantity() const { return quantity; }
	std::string get_price() const { return price; }

	void set_name(std::string name) { this->name = name; }
	void set_quantity(int quantity) { this->quantity = quantity; }
	void set_price(std::string price) { this->price = price; }
};