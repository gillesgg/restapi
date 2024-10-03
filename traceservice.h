#ifndef TRACESERVICE_H
#define TRACESERVICE_H

#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"
#include "opentelemetry/metrics/provider.h"

#include "opentelemetry/exporters/ostream/log_record_exporter_factory.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/sdk/logs/processor.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"

namespace logs_api = opentelemetry::logs;

namespace UtilityService
{
    class SingletonTrace {

    public:
        static SingletonTrace* getInstance();
        void InitTracer();        
        void CleanupTracer();

        opentelemetry::nostd::shared_ptr<logs_api::Logger> get_logger();

    private:
        // private constructor and destructor
        SingletonTrace() {}
        ~SingletonTrace() {}

        // private copy constructor and assignment operator
        SingletonTrace(const SingletonTrace&);
        SingletonTrace& operator=(const SingletonTrace&);

        static SingletonTrace* m_instanceSingleton;
    };
}
#endif