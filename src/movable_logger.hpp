#pragma once

#include "logger.hpp"

// std::ostream is not movable, causing this rift.
template <typename T, typename... fmt_types>
class movable_logger : public logger<std::unique_ptr<T>, fmt_types...>
{
public:
    virtual ~movable_logger() = default;
    movable_logger(movable_logger & o) = delete;

    movable_logger(std::unique_ptr<T>&& stream) : logger<std::unique_ptr<T>, fmt_types...>(TRACE) {
        this->m_stream = std::move(stream);
    }

    movable_logger(std::unique_ptr<T>&& stream, const Level& level, const std::string& fmt, fmt_types... args)
        : logger<std::unique_ptr<T>, fmt_types...>(level, fmt, args...) {
        this->m_stream = std::move(stream);
    }

    movable_logger(std::unique_ptr<T>& stream, const Level& level, const std::string& fmt, fmt_types... args)
        : logger<std::unique_ptr<T>, fmt_types...>(level, fmt, args...) {
        this->m_stream = std::move(stream);
    }

    movable_logger(movable_logger && o) noexcept {
        this->m_stream = std::move(o.m_stream);
        this->m_level =  std::move(o.m_level);
        this->m_format = std::move(o.m_format);
        this->m_format_vars = std::move(o.m_format_vars);
        this->m_message = std::move(o.m_message);
    }

    void do_log(std::string& msg) override {
        *(this->m_stream) << msg;
        this->m_stream->flush();
    };

};