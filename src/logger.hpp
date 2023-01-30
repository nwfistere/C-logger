#pragma once

#include <memory>

void log(std::string str);

enum Format {
    TIMESTAMP   = 't',
    PID         = 'p',
    LEVEL       = 'l',
    MESSAGE     = 'm',
    SEPARATOR   = 's'
};

template <typename T>
class logger
{
protected:
    T m_stream;
    std::string m_format;
    std::string m_separator = "\n============================================================================================\n";
public:
    logger() = default;
    virtual ~logger() = default;
    virtual void log(const std::string& msg) = 0;
    void set_format(const std::string& format);
    std::string format_message(const int level, const std::string& msg);
};

// std::ostream is not movable, causing this rift.
// template <typename T>
// class movable_logger : public virtual logger<std::unique_ptr<T>>
// {
// public:
//     movable_logger(std::unique_ptr<T> stream);
//     virtual ~movable_logger() = default;
//     void log(const std::string& msg);
// };

template <typename T>
class unmovable_logger : public logger<T*>
{
public:
    unmovable_logger(T& stream, const std::string& format = "");
    virtual ~unmovable_logger() = default;
    void log(const std::string& msg);
};