#include "duckdb/parallel/thread_context.hpp"

#include "duckdb/main/client_context.hpp"

namespace duckdb {

ThreadContext::ThreadContext(ClientContext &context, int id) : profiler(context.profiler.IsEnabled()), id(id) {
}

ThreadContext1::ThreadContext1(ClientContext context, int id) : ThreadContext(context, id) {
}
} // namespace duckdb
