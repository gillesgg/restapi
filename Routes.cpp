#include <chrono>
#include "Routes.h"
#include "utility.h"
#include "traceservice.h"

// Database filename
static const std::string filename_db = "database.db3";

// Constructor
Routes::Routes(crow::SimpleApp& app) : m_App(app)

{
	// use the function deletedatabase to delete the database file if you want to start with a clean database
	db_ = std::make_unique<DBSQLite>(Utility::get_temporary_folder(filename_db));
};

void Routes::orders_routes()
{
	/**
	 * Handles the POST request for creating a new meal.
	 *
	 * @param req The crow::request object.
	 * @return The crow::response object.
	 */
	CROW_ROUTE(m_App, "/meals")
		.methods(crow::HTTPMethod::POST)
		([this](const crow::request& req)
			{
				// Get the a tracer instance from OpenTelemetry::trace::Provider
				// Name the tracer "rest-tracer"               
				auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("rest-tracer");

				// create a new span for the POST request
				auto span = tracer->StartSpan("POST /meal");

				// start a timer to measure the duration of the request using std::chrono:steady_clock
				auto start = std::chrono::steady_clock::now();

				crow::json::wvalue output;
				if (!rateLimiter[req.url].allow_request()) return crow::response(429);
				try
				{
					// load req.body to a crwo::json::rvalue object
					// check if the JSON object is valid raise an exception if it is not
					auto json_body = crow::json::load(req.body);
					if (!json_body) {
						return crow::response(400, "Invalid JSON");
					}
					// create a new DBMeal object
					// set the name, quantity, and price of the meal from the JSON object
					DBMeal meal;
					meal.set_name(json_body["name"].s());
					meal.set_quantity(json_body["quantity"].i());
					meal.set_price(json_body["price"].s());
					// add the meal to the database                  
					db_->create_new_meal(meal);
					// end the timer, calculate the elapse time between start and end 
					// convert the duration to milliseconds
					auto end = std::chrono::steady_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					// using the span, create a new attribute named elapse, set the duration of the request in milliseconds, convert the duration a std::string				
					span->SetAttribute("elapse", std::to_string(duration.count()));
					// end the span
					span->End();

					return crow::response(200);
				}
				catch (const std::exception& error)
				{
					// return a JSON object with the error message explaining the error
					// Extract the error message from the exception and send it back in the response body
					// return a 500 status code
					crow::json::wvalue error_json;
					error_json["message"] = error.what();
					return crow::response(500, error_json);
				}
			}
			);
	/**
	 * Handles the GET request for retrieving all meals.
	 *
	 * @param req The crow::request object.
	 * @return The crow::response object.
	 */
	CROW_ROUTE(m_App, "/meals")
		.methods(crow::HTTPMethod::GET)
		([this](const crow::request& req)
			{
				// Get the a tracer instance from OpenTelemetry::trace::Provider
				// Name the tracer "rest-tracer"    
				auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("rest-tracer");
				// create a new span for the POST request
				auto span = tracer->StartSpan("GET /meal");
				// start a timer to measure the duration of the request using std::chrono:steady_clock
				auto start = std::chrono::steady_clock::now();
				crow::json::wvalue output;
				if (!rateLimiter[req.url].allow_request()) return crow::response(429);

				try
				{
					// get all meals from the database
					// send back the list of meals in the response body using array of JSON objects
					std::list<DBMeal> meals = db_->get_all_meals();
					std::vector<crow::json::wvalue> crew_members;

					// loop through the list of meals and add each meal to a std::vector<crow::json::wvalue>
					for (auto& meal : meals)
					{
						crow::json::wvalue meal_json;
						meal_json["name"] = meal.get_name();
						meal_json["quantity"] = meal.get_quantity();
						meal_json["price"] = meal.get_price();
						crew_members.push_back(meal_json);
					}

					// return the vector of meals in the response body
					// return a 200 status code

					crow::json::wvalue wv;
					wv = std::move(crew_members);

					// end the timer, calculate the elapse time between start and end 
					 // convert the duration to milliseconds
					auto end = std::chrono::steady_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					// using the span, create a new attribute named elapse, set the duration of the request in milliseconds, convert the duration a std::string				
					span->SetAttribute("elapse", std::to_string(duration.count()));
					// end the span
					span->End();
					return crow::response(200, std::move(wv));
				}
				catch (const std::exception& error)
				{
					// return a JSON object with the error message explaining the error
					// Extract the error message from the exception and send it back in the response body
					// return a 500 status code
					crow::json::wvalue error_json;
					error_json["message"] = error.what();
					return crow::response(500, error_json);
				}
			}
			);

	/**
	 * Handles the GET request for retrieving a meal by ID.
	 *
	 * @param req The crow::request object.
	 * @param meal_id The ID of the meal to retrieve.
	 * @return The crow::response with the error from the error.what().
	 */
	CROW_ROUTE(m_App, "/meals/<int>")
		.methods(crow::HTTPMethod::GET)
		([this](const crow::request& req, int meal_id)
			{
				// Get the a tracer instance from OpenTelemetry::trace::Provider
				// Name the tracer "rest-tracer"
				auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("rest-tracer");
				// create a new span for the POST request
				auto span = tracer->StartSpan("GET /meal/<int>");
				// start a timer to measure the duration of the request using std::chrono:steady_clock                
				auto start = std::chrono::steady_clock::now();

				crow::json::wvalue output;
				if (!rateLimiter[req.url].allow_request()) return crow::response(429);
				try
				{
					// get the meal by id from the database
					// send back the meal in the response body using JSON object
					DBMeal meal = db_->get_meal_by_id(meal_id);
					crow::json::wvalue meal_json;
					meal_json["name"] = meal.get_name();
					meal_json["quantity"] = meal.get_quantity();
					meal_json["price"] = meal.get_price();

					// end the timer, calculate the elapse time between start and end 
					// convert the duration to milliseconds
					auto end = std::chrono::steady_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					// using the span, create a new attribute named elapse, set the duration of the request in milliseconds, convert the duration a std::string				
					span->SetAttribute("elapse", std::to_string(duration.count()));
					// end the span
					span->End();
					return crow::response(200, meal_json);
				}
				catch (const std::exception& error)
				{
					// return a JSON object with the error message explaining the error
					// Extract the error message from the exception and send it back in the response body
					// return a 500 status code
					crow::json::wvalue error_json;
					error_json["message"] = error.what();
					return crow::response(500, error_json);
				}
			}
			);
	/**
	 * Handles the GET request for retrieving a meal by name.
	 *
	 * @param req The crow::request object.
	 * @param meal_name The name of the meal to retrieve.
	 * @return The crow::response with the error from the error.what().
	 */
	CROW_ROUTE(m_App, "/meals/<string>")
		.methods(crow::HTTPMethod::GET)
		([this](const crow::request& req, std::string meal_name)
			{
				// Get the a tracer instance from OpenTelemetry::trace::Provider
				// Name the tracer "rest-tracer"
				auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("rest-tracer");
				// create a new span for the POST request
				auto span = tracer->StartSpan("GET /meal/<string>");
				// start a timer to measure the duration of the request using std::chrono:steady_clock                
				auto start = std::chrono::steady_clock::now();

				crow::json::wvalue output;
				if (!rateLimiter[req.url].allow_request()) return crow::response(429);
				try
				{
					// Unescape the meal name
					Utility::UnescapePostData(meal_name);

					// get the meal by name from the database
					// send back the meal in the response body using JSON object
					crow::json::wvalue output;
					if (!rateLimiter[req.url].allow_request()) return crow::response(429);
					// get the meal by name from the database
					// send back the meal in the response body using JSON object
					DBMeal meal = db_->get_meal_by_name(meal_name);
					output["name"] = meal.get_name();
					output["quantity"] = meal.get_quantity();
					output["price"] = meal.get_price();
					
					// end the timer, calculate the elapse time between start and end 
					// convert the duration to milliseconds
					auto end = std::chrono::steady_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					// using the span, create a new attribute named elapse, set the duration of the request in milliseconds, convert the duration a std::string				
					span->SetAttribute("elapse", std::to_string(duration.count()));
					// end the span
					span->End();
					return crow::response(200, output);
				}
				catch (const std::exception& error)
				{
					// return a JSON object with the error message explaining the error
					// Extract the error message from the exception and send it back in the response body
					// return a 500 status code
					crow::json::wvalue error_json;
					error_json["error"] = error.what();
					return crow::response(500, error_json);
				}
			}
			);

	/**
	 * Handles the DELTE request for deleting a meal by id.
	 *
	 * @param req The crow::request object.
	 * @param meal_name The id of the meal to delete.
	 * @return The crow::response with the error from the error.what().
	 */
	CROW_ROUTE(m_App, "/meals/<int>")
		.methods(crow::HTTPMethod::Delete)
		([this](const crow::request& req, int meal_id)
			{
				// Get the a tracer instance from OpenTelemetry::trace::Provider
				// Name the tracer "rest-tracer"
				auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("rest-tracer");
				// create a new span for the POST request
				auto span = tracer->StartSpan("Delete /meal/<int>");
				// start a timer to measure the duration of the request using std::chrono:steady_clock                
				auto start = std::chrono::steady_clock::now();

				crow::json::wvalue output;
				if (!rateLimiter[req.url].allow_request()) return crow::response(429);
				try
				{
					// delete the meal by id from the database
					// get a db_
					// return a 200 status code
					int status = db_->delete_mail_by_id(meal_id);
					crow::json::wvalue error_status;
					error_status["status"] = status;
					// end the timer, calculate the elapse time between start and end 
					// convert the duration to milliseconds
					auto end = std::chrono::steady_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					// using the span, create a new attribute named elapse, set the duration of the request in milliseconds, convert the duration a std::string				
					span->SetAttribute("elapse", std::to_string(duration.count()));
					// end the span
					span->End();
					return crow::response(status, error_status);

				}
				catch (const std::exception& error)
				{
					// return a JSON object with the error message explaining the error
					// Extract the error message from the exception and send it back in the response body
					// return a 500 status code
					crow::json::wvalue error_json;
					error_json["error"] = error.what();
					return crow::response(500, error_json);
				}
			}
			);
}