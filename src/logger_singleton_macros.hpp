#pragma once
#include "logger_singleton.hpp"

#define LOG(level, msg) logger_singleton::get().log(level, msg)
#define TRACE(msg) LOG(TRACE, msg)
#define DEBUG(msg) LOG(DEBUG, msg)
#define INFO(msg) LOG(INFO, msg)
#define WARN(msg) LOG(WARN, msg)
#define ERROR(msg) LOG(ERROR, msg)
#define FATAL(msg) LOG(FATAL, msg)

#define WHO(w) logger_singleton::get().who(w)