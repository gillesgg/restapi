#include <filesystem>
#include <SQLiteCpp/SQLiteCpp.h>
#include "DataBase.h"

namespace fs = std::filesystem;

static const std::string data_db = "meals.txt";


DBSQLite::DBSQLite(const std::string& file_name) : db_(file_name.c_str(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
	create_table_if_not_exist();
}
/// <summary>
/// Get a meal by name
/// </summary>
/// <param name="name"></param>
/// <returns>the meal</returns>
DBMeal DBSQLite::get_meal_by_name(const std::string& name)
{
	try
	{
		// Create new SQLite::Statement query to get the meal by name
		// Get the meal by name
		// throw an exception if the meal is not found
		// return the meal using a DBMeal class
		SQLite::Statement query(db_, "SELECT * FROM meals WHERE name = ?");
		query.bind(1, name);
		if (query.executeStep())
		{
			std::string name = query.getColumn(1).getText();
			int quantity = query.getColumn(2).getInt();
			std::string price = query.getColumn(3).getText();
			DBMeal meal(name, quantity, price);
			return meal;
		}
		else
		{
			throw std::runtime_error("Meal not found");
		}
	}
	catch (std::exception& e)
	{
		throw(e);
	}
}
/// <summary>
/// Get all meals from the table "meals".    
/// </summary>
/// <returns>Return a list of meals.</returns>
std::list<DBMeal> DBSQLite::get_all_meals()
{
	try
	{
		// Get all meals from the table meals
		// use a list of DBMeal to store the meals
		// return the list of meals

		std::list<DBMeal> meals;
		SQLite::Statement query(db_, "SELECT * FROM meals");
		while (query.executeStep())
		{
			DBMeal meal;
			meal.set_name(query.getColumn("name").getString());
			meal.set_quantity(query.getColumn("quantity").getInt());
			meal.set_price(query.getColumn("price").getString());
			meals.push_back(meal);
		}
		return meals;
	}
	catch (std::exception& e)
	{
		throw(e);
	}
}


/// <summary>
/// Get a meal by id
/// use exception to handle the case where the meal is not found
/// </summary>
/// <param name="id"></param>
/// <returns>a DBMeal class</returns>
DBMeal DBSQLite::get_meal_by_id(int id)
{
	try
	{
		// Get the meal by id
		// throw an exception if the meal is not found
		// return the meal using a DBMeal class
		SQLite::Statement query(db_, "SELECT * FROM meals WHERE id = ?");
		query.bind(1, id);
		if (query.executeStep())
		{
			DBMeal meal;
			meal.set_name(query.getColumn("name").getString());
			meal.set_quantity(query.getColumn("quantity").getInt());
			meal.set_price(query.getColumn("price").getString());
			return meal;
		}
		else
		{
			throw std::runtime_error("Meal not found");
		}
	}
	catch (std::exception& e)
	{
		throw(e);
	}
}
/// <summary>
/// Add a new meal to the table "meals".
/// use exception to handle the case where the meal already exists
/// </summary>
/// <param name="meal"></param>
void DBSQLite::create_new_meal(const DBMeal& meal)
{
	try
	{
		// test if the meal already exists
		// throw an exception if the meal already exists
		SQLite::Statement query(db_, "SELECT * FROM meals WHERE name = ?");
		query.bind(1, meal.get_name());
		if (query.executeStep())
		{
			throw std::runtime_error("Meal already exists");
		}

		// Insert the meal into the table meals
		SQLite::Statement querynew(db_, "INSERT INTO meals (name, quantity, price) VALUES (?, ?, ?)");
		querynew.bind(1, meal.get_name());
		querynew.bind(2, meal.get_quantity());
		querynew.bind(3, meal.get_price());
		querynew.exec();
	}
	catch (std::exception& e)
	{
		throw(e);
	}
}
/// <summary>
/// Delete a meal by id
/// </summary>
/// <param name="id">the id</param>
/// <returns>200 if the meal is deleted</returns>
int DBSQLite::delete_mail_by_id(int id)
{
	try
	{
		// Create new SQLite::Statement query to delete the meal by id
		// throw an exception if the meal is not found
		// return 200 if the meal is deleted
		// return exception if the meal is not found
		SQLite::Statement query(db_, "DELETE FROM meals WHERE id = ?");
		query.bind(1, id);
		query.exec();
		return 200;
	}
	catch (std::exception& e)
	{
		throw(e);
	}
}
#pragma region database creation

/// <summary>
/// Drop the table "meals" if it exists.
/// use exception to handle the case where the table does not exist
/// </summary>
void DBSQLite::drop_table_meals()
{
	try
	{
		// test if the table meals exists
		// throw an exception if the table does not exist
		// Drop the table meals if the table exists
		SQLite::Statement query(db_, "SELECT name FROM sqlite_master WHERE type='table' AND name='meals'");
		if (query.executeStep())
		{
			// Drop the table meals if the table exists
			db_.exec("DROP TABLE meals");
		}
		else
		{
			throw std::runtime_error("Table does not exist");
		}
	}
	catch (std::exception& e)
	{
		throw(e);
	}
}
/// <summary>
/// Creates the table "meals" if it does not exist and inserts data from data_db file into the table.
/// </summary>
void DBSQLite::create_table_if_not_exist()
{
	// If the file meals.csv does not exist, return
	if (fs::exists(data_db))
	{
		//test if the table meals exists
		SQLite::Statement query(db_, "SELECT name FROM sqlite_master WHERE type='table' AND name='meals'");
		if (!query.executeStep())
		{
			// Create the table meals if the table does not exist
			db_.exec("CREATE TABLE meals (id INTEGER PRIMARY KEY, name TEXT, quantity INTEGER, price TEXT)");

			// Vector of vector of string to store the data
			// ifstream to read the data from the file meals.txt
			// vector of vector of string to store the data
			std::string line;
			std::ifstream file(data_db);
			std::vector<std::vector<std::string>> data;

			// Read the data from the file meals.txt
			// Insert a line into the vector of string
			// inset the vector of string into the vector of vector of string

			while (std::getline(file, line)) { // Read each line
				std::stringstream ss(line);
				std::string item;
				std::vector<std::string> row;

				while (std::getline(ss, item, ',')) { // Split by comma
					row.push_back(item);
				}
				data.push_back(row);
			}

			// Insert the data into the table meals
			for (int i = 0; i < data.size(); i++) {
				SQLite::Statement query(db_, "INSERT INTO meals (name, quantity, price) VALUES (?, ?, ?)");
				query.bind(1, data[i][0]);
				query.bind(2, std::stoi(data[i][1]));
				query.bind(3, data[i][2]);
				query.exec();
			}
		}
	}
#pragma endregion
}