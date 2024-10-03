#include <iostream>
#include "traceservice.h"

#include <opentelemetry/exporters/ostream/span_exporter_factory.h>
#include <opentelemetry/sdk/trace/exporter.h>
#include <opentelemetry/sdk/trace/processor.h>
#include <opentelemetry/sdk/trace/simple_processor_factory.h>
#include <opentelemetry/sdk/trace/tracer_provider_factory.h>
#include <opentelemetry/trace/provider.h>
         
#include <opentelemetry/sdk/metrics/meter_provider.h>
#include <opentelemetry/sdk/metrics/meter_provider_factory.h>
#include <opentelemetry/metrics/provider.h>
         
#include <opentelemetry/exporters/ostream/log_record_exporter_factory.h>
#include <opentelemetry/logs/provider.h>
#include <opentelemetry/sdk/logs/logger_provider_factory.h>
#include <opentelemetry/sdk/logs/processor.h>
#include <opentelemetry/sdk/logs/simple_log_record_processor_factory.h>

namespace trace_api = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace trace_exporter = opentelemetry::exporter::trace;

namespace metrics_sdk = opentelemetry::sdk::metrics;
namespace metrics_api = opentelemetry::metrics;

namespace logs_api = opentelemetry::logs;
namespace logs_sdk = opentelemetry::sdk::logs;
namespace logs_exporter = opentelemetry::exporter::logs;

namespace UtilityService
{
   /// <summary>
   /// Gets the singleton instance of the SingletonTrace class.
   /// </summary>
   /// <returns>Pointer to the SingletonTrace instance.</returns>
   SingletonTrace* SingletonTrace::getInstance()
   {
       return (!m_instanceSingleton) ?
           m_instanceSingleton = new SingletonTrace :
           m_instanceSingleton;
   }

   /// <summary>
   /// Initializes the tracer.
   /// </summary>
   void SingletonTrace::InitTracer()
   {
       auto exporter = trace_exporter::OStreamSpanExporterFactory::Create();
       auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
       std::shared_ptr<opentelemetry::trace::TracerProvider> provider = trace_sdk::TracerProviderFactory::Create(std::move(processor));
       trace_api::Provider::SetTracerProvider(provider);
   }

   /// <summary>
   /// Cleans up the tracer.
   /// </summary>
   void SingletonTrace::CleanupTracer()
   {
       std::shared_ptr<opentelemetry::trace::TracerProvider> none;
       trace_api::Provider::SetTracerProvider(none);
   }

   /// <summary>
   /// Gets the logger instance.
   /// </summary>
   /// <returns>Shared pointer to the logger instance.</returns>
   opentelemetry::nostd::shared_ptr<logs_api::Logger> SingletonTrace::get_logger()
   {
       auto provider = logs_api::Provider::GetLoggerProvider();
       return provider->GetLogger("restapi_logger", "restapi_rest_api");
   }

   /// <summary>
   /// Static member to hold the singleton instance.
   /// </summary>
   UtilityService::SingletonTrace* UtilityService::SingletonTrace::m_instanceSingleton = nullptr;
}