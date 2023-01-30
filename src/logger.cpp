#include "logger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <unistd.h>

static uint getUnixTimestamp();

//logger
template <typename T>
void logger<T>::set_format(const std::string& format) {
    m_format = format;
}

template <typename T>
std::string logger<T>::format_message(const int level, const std::string& msg) {
    if (m_format.empty()) {
        return msg;
    }

    std::stringstream string_stream("");
    for (char c : m_format) {
        switch (c)
        {
        case TIMESTAMP:
            string_stream << getUnixTimestamp();
            break;
        case PID:
            string_stream << ::getpid();
            break;
        case LEVEL:
            string_stream << level;
            break;
        case MESSAGE:
            string_stream << msg;
            break;
        case SEPARATOR:
            string_stream << m_separator;
            break;
        default:
            string_stream.put(c);
            break;
        }
    }

    return string_stream.str();
}

// movable_logger
// template <typename T>
// movable_logger<T>::movable_logger(std::unique_ptr<T> stream) {
//     this->m_stream = std::move(stream);
// }

// template <typename T>
// void movable_logger<T>::log(const std::string& msg) {
//     *(this->m_stream.get()) << msg;
// }

// template class movable_logger<std::ofstream>;

//unmovable_logger
template <typename T>
unmovable_logger<T>::unmovable_logger(T & stream, const std::string& format) {
    this->m_stream = &stream;
    this->m_format = format;
}

template <typename T>
void unmovable_logger<T>::log(const std::string& msg) {
    this->format_message(0, msg);
    *(this->m_stream) << msg;
}

// I can either add the following or move everything to the header file.
template class logger<std::ostream*>;

template class unmovable_logger<std::ostream>;


uint getUnixTimestamp() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}