// restapi.cpp : Defines the entry point for the application.
//

#include <crow.h>
#include <memory>
#include <utility>
#include "restapi.h"
#include "Routes.h"
#include "utility.h"
#include "traceservice.h"

using namespace std;




// Main function
int main()
{
    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Warning);

    UtilityService::SingletonTrace* singleton;
    singleton = singleton->getInstance();

    singleton->getInstance()->InitTracer();

    Routes routes(app);
    routes.orders_routes();

    app.validate();
    auto run = app.port(8080).multithreaded().run_async();
    std::cout << "Server is running on port 8080. Press ENTER to quit..." << std::endl;
    std::cin.get();
    
    singleton->getInstance()->CleanupTracer();

    app.stop();
}