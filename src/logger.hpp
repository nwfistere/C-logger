#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <unistd.h>
#include <memory>
#include <tuple>
#include <type_traits>
#include <map>

const std::string FORMAT_PREFIX = "%^";
const std::string MESSAGE_STR = FORMAT_PREFIX + "m";
const std::string LEVEL_STR_STR = FORMAT_PREFIX + "ls";
const std::string LEVEL_INT_STR = FORMAT_PREFIX + "ld";


template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), std::string>::type
append(int i, std::string msg, std::tuple<Tp...>& t) { return msg; }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), std::string>::type
append(int i, std::string msg, std::tuple<Tp...>& t) {
    std::string findstr = FORMAT_PREFIX + std::to_string(i);
    int pos = msg.find(findstr);

    while (pos != std::string::npos) {
        std::stringstream stream("");
    
        // Currently only support parameterless functions.
        stream << std::get<I>(t)();

        msg.replace(pos, findstr.length(), stream.str());

        // Might want to do find(findstr + pos + stream.str().length())
        // to prevent variable values from being replaced
        pos = msg.find(findstr);
    }
    return append<I + 1, Tp...>(i + 1, msg, t);
}

enum Level {
    NONE,
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

static const std::map<Level, std::string> LevelString = {
    { NONE, "NONE" },
    { TRACE, "TRACE" },
    { DEBUG, "DEBUG" },
    { INFO, "INFO" },
    { WARN, "WARN" },
    { ERROR, "ERROR" },
    { FATAL, "FATAL" }
};

class base_logger {
public:
    virtual void log(const Level& level, const std::string& msg) {};
};

template <typename T, typename... fmt_types>
class logger : public base_logger
{
protected:
    T m_stream;
    Level m_level;
    std::string m_format;
    std::tuple<fmt_types...> m_format_vars;
    std::string m_message;

public:
    logger(const Level& level) : m_level(level), m_format(MESSAGE_STR) {};
    logger(const Level& level, const std::string& fmt, fmt_types... args) :
    m_level(level), m_format(fmt), m_format_vars(std::make_tuple(args...)) {};
    virtual ~logger() = default;
    inline virtual void trace(const std::string& msg) { log(TRACE, msg); };
    inline virtual void debug(const std::string& msg) { log(DEBUG, msg); };
    inline virtual void  warn(const std::string& msg) { log(WARN,  msg); };
    inline virtual void error(const std::string& msg) { log(ERROR, msg); };
    inline virtual void fatal(const std::string& msg) { log(FATAL, msg); };

    void set_format(const std::string& fmt, fmt_types... args) {
        m_format = fmt;
        m_format_vars = std::make_tuple(args...);
    }

    virtual void log(const Level& level, const std::string& msg) {
        if (level < m_level) {
            m_message.clear();
            return;
        }
        std::size_t pos;

        m_message = m_format;

        while ((pos = m_message.find(MESSAGE_STR)) != std::string::npos) {
            m_message.replace(pos, MESSAGE_STR.length(), msg);
        }

        while ((pos = m_message.find(LEVEL_INT_STR)) != std::string::npos) {
            m_message.replace(pos, LEVEL_INT_STR.length(), std::to_string(level));
        }

        while ((pos = m_message.find(LEVEL_STR_STR)) != std::string::npos) {
            m_message.replace(pos, LEVEL_STR_STR.length(), LevelString.at(level));
        }

        m_message = append(0, m_message, m_format_vars);
    };
    
    // Getters and Setters
    Level get_level() { return m_level; }

    // %^m is message
    // %[0-9]+ is the return values of the functions you pass in of type fmt_types(void)
    void set_level(const Level& level) {  m_level = level; }

};

// std::ostream is not movable, causing this rift.
template <typename T, typename... fmt_types>
class movable_logger : public logger<std::unique_ptr<T>, fmt_types...>
{
public:
    virtual ~movable_logger() = default;

    movable_logger(std::unique_ptr<T> stream) : logger<std::unique_ptr<T>, fmt_types...>(TRACE) {
        this->m_stream = std::move(stream);
    }

    movable_logger(std::unique_ptr<T> stream, const Level& level, const std::string& fmt, fmt_types... args)
        : logger<std::unique_ptr<T>, fmt_types...>(level, fmt, args...) {
        this->m_stream = std::move(stream);
    }

    movable_logger(movable_logger && o) noexcept {
        std::cout << "moving" << std::endl;
        this->m_stream = std::move(o.m_stream);
        this->m_level =  std::move(o.m_level);
        this->m_format = std::move(o.m_format);
        this->m_format_vars = std::move(o.m_format_vars);
        this->m_message = std::move(o.m_message);
    }

    void log(const Level& level, const std::string& msg) override {      
        logger<std::unique_ptr<T>, fmt_types...>::log(level, msg);

        *(this->m_stream.get()) << this->m_message;
    };

    // Since the stream is moved to the logger,
    // tests don't have access to it.
    // This provides access. I prefer this over
    // adding #define private public in the tests.
    std::unique_ptr<T> get_stream() { return std::move(this->m_stream); }
};

template <typename T, typename... fmt_types>
class unmovable_logger : public logger<T*, fmt_types...>
{
public:
    virtual ~unmovable_logger() = default;

    unmovable_logger(T* stream) : logger<T*, fmt_types...>(TRACE) {
        this->m_stream = stream;
    }

    unmovable_logger(T* stream, const Level& level, const std::string& fmt, fmt_types... args)
        : logger<T*, fmt_types...>(level, fmt, args...) {
        this->m_stream = stream;
    }

    void log(const Level& level, const std::string& msg) override {      
        logger<T*, fmt_types...>::log(level, msg);

        *(this->m_stream) << this->m_message;
    };

};
