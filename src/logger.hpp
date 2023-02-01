#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <unistd.h>
#include <memory>
#include <tuple>
#include <type_traits>

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), std::string>::type
append(int i, std::string msg, std::tuple<Tp...>& t) { return msg; }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), std::string>::type
append(int i, std::string msg, std::tuple<Tp...>& t) {
    std::string findstr = "%" + std::to_string(i);
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
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

template <typename T, typename... fmtTypes>
class logger
{
protected:
    T m_stream;
    Level m_level;
    std::string m_format;
    std::tuple<fmtTypes...> m_format_vars;

    std::string m_message;
public:
    logger(const Level& level) : m_level(level) {};
    virtual ~logger() = default;
    inline virtual void trace(const std::string& msg) { log(TRACE, msg); };
    inline virtual void debug(const std::string& msg) { log(DEBUG, msg); };
    inline virtual void warn(const std::string& msg)  { log(WARN,  msg); };
    inline virtual void error(const std::string& msg) { log(ERROR, msg); };
    inline virtual void fatal(const std::string& msg) { log(FATAL, msg); };

    void set_format(const std::string& fmt, fmtTypes... args) {
        m_format = fmt;
        m_format_vars = std::make_tuple(args...);
    }

    virtual void log(const Level& level, const std::string& msg) {      
        m_message = append(0, m_format, m_format_vars);
        size_t pos = -1;
        while ((pos = m_message.find("%m")) != std::string::npos) {
            m_message.replace(pos, 2, msg);
        }
    };
    
    // Getters and Setters
    Level get_level() { return m_level; }
    void set_level(const Level& level) {  m_level = level; }

};

// std::ostream is not movable, causing this rift.
template <typename T, typename... fmtTypes>
class movable_logger : public logger<std::unique_ptr<T>, fmtTypes...>
{
public:
    virtual ~movable_logger() = default;

    movable_logger(std::unique_ptr<T> stream) : logger<std::unique_ptr<T>, fmtTypes...>(TRACE) {
        this->m_stream = std::move(stream);
    }

    void log(const Level& level, const std::string& msg) override {      
        logger<std::unique_ptr<T>, fmtTypes...>::log();

        this->m_stream.get() << this->m_message;
    };
};

template <typename T, typename... fmtTypes>
class unmovable_logger : public logger<T*, fmtTypes...>
{
public:
    virtual ~unmovable_logger() = default;

    unmovable_logger(T& stream) : logger<T*, fmtTypes...>(TRACE) {
        this->m_stream = &stream;
    }

    void log(const Level& level, const std::string& msg) override {      
        logger<T*, fmtTypes...>::log(level, msg);

        *(this->m_stream) << this->m_message;
    };
};
