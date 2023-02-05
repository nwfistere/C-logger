#pragma once

#include "logger.hpp"

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
        this->m_stream->flush();
    };

};
