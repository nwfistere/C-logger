#pragma once

#include "logger.hpp"
#include "unmovable_logger.hpp"
#include <vector>

class logger_singleton {

private:
    std::unique_ptr<base_logger> m_logger;

    logger_singleton() = default;

public:
    logger_singleton(const logger_singleton&) = delete;
    logger_singleton& operator=(const logger_singleton&) = delete;
    static logger_singleton& get() {
        static logger_singleton m_instance;
        return m_instance;
    }

    void add_logger(std::unique_ptr<base_logger>&& plogger) {
        m_logger = std::move(plogger);
    }

    void add_logger(std::unique_ptr<base_logger>& plogger) {
        m_logger = std::move(plogger);
    }

    template <typename T>
    void add_logger(unmovable_logger<T>* plogger) {
        m_logger = std::unique_ptr<unmovable_logger<T>>(plogger);
    }

    template <typename T>
    void add_logger(unmovable_logger<T>&& plogger) {
        m_logger = std::make_unique<unmovable_logger<T>>(plogger);
    }

    void who(std::string&& rwho) {
        m_logger->who(std::move(rwho));
    }

    void log(const Level& level, const std::string& msg) {
        m_logger->log(level, msg);
    }

    void set_level(const Level& level) {
        m_logger->set_level(level);
    }
};
