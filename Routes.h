#ifndef ROUTES_H
#define ROUTES_H

#include <crow.h>

#include "Limiter.h"
#include "DataBase.h"


class Routes
{
public:
	Routes(crow::SimpleApp&);
	void orders_routes();

private:
	std::unordered_map<std::string, Limiter> rateLimiter;
	crow::SimpleApp& m_App;

	std::unique_ptr<DBSQLite> db_;
};

#endif