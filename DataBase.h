#ifndef DATABASE_H
#define DATABASE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <filesystem>
#include <crow.h>
#include <list>
#include "DBMeal.h"

/// DBSQLite definition
class DBSQLite
{
// Constructor
public:
	DBSQLite(const std::string& file_name);
	virtual ~DBSQLite() {};

// public methods
public:
    void                create_new_meal(const DBMeal& meal);
    std::list<DBMeal>   get_all_meals();
    DBMeal              get_meal_by_id(int id);
    DBMeal              get_meal_by_name(const std::string& name);
    int                 delete_mail_by_id(int id);
    void                drop_table_meals();
    void                create_table_if_not_exist();

private:
    SQLite::Database    db_;    // Database connection   
};

#endif 