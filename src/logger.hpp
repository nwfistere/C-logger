#pragma once

#include "logger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <unistd.h>
#include <memory>

void log(std::string str);

enum Level {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

template <typename T>
class logger
{
protected:
    T m_stream;
    Level m_level;
    std::string m_format;
    std::string m_separator = "\n============================================================================================\n";
public:
    logger(const Level& level) : m_level(level) {};
    virtual ~logger() = default;
    virtual void log(const Level& level, const std::string& msg) = 0;
    inline virtual void trace(const std::string& msg) { log(TRACE, msg); };
    inline virtual void debug(const std::string& msg) { log(DEBUG, msg); };
    inline virtual void warn(const std::string& msg)  { log(WARN,  msg); };
    inline virtual void error(const std::string& msg) { log(ERROR, msg); };
    inline virtual void fatal(const std::string& msg) { log(FATAL, msg); };

    template<typename ...Ts>
    void set_format(std::string fmt, Ts... args) {
        static const std::size_t value = sizeof...(Ts);
        std::string msg = "size: " + std::to_string(value);
        trace(msg);
    }
};

// std::ostream is not movable, causing this rift.
template <typename T>
class movable_logger : public logger<std::unique_ptr<T>>
{
public:
    virtual ~movable_logger() = default;

    movable_logger(std::unique_ptr<T> stream) : logger<std::unique_ptr<T>>(TRACE) {
        this->m_stream = std::move(stream);
    }

    void log(const Level& level, const std::string& msg) {
        if (level >= this->m_level)
            *(this->m_stream.get()) << msg;
    }
};

template <typename T>
class unmovable_logger : public logger<T*>
{
public:
    virtual ~unmovable_logger() = default;

    unmovable_logger(T& stream) : logger<T*>(TRACE) {
        this->m_stream = &stream;
    }

    void log(const Level& level, const std::string& msg) {
        if (level >= this->m_level)
            *(this->m_stream) << msg;
    }
};