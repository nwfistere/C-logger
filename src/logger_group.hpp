#pragma once

#include "logger.hpp"
#include "unmovable_logger.hpp"
#include <vector>

class logger_group {

private:
    std::vector<std::unique_ptr<base_logger>> m_loggers;

    logger_group() = default;

public:
    logger_group(const logger_group&) = delete;
    logger_group& operator=(const logger_group&) = delete;
    static logger_group& get() {
        static logger_group m_instance;
        return m_instance;
    }

    void clear() {
        m_loggers.clear();
    }

    void add_logger(std::unique_ptr<base_logger>&& plogger) {
        m_loggers.push_back(std::move(plogger));
    }

    void add_logger(std::unique_ptr<base_logger>& plogger) {
        m_loggers.push_back(std::move(plogger));
    }

    template <typename T>
    void add_logger(unmovable_logger<T>* plogger) {
        m_loggers.push_back(std::unique_ptr<unmovable_logger<T>>(plogger));
    }

    template <typename T>
    void add_logger(unmovable_logger<T>&& plogger) {
        m_loggers.push_back(std::make_unique<unmovable_logger<T>>(plogger));
    }

    void who(std::string&& rwho) {
        for (const std::unique_ptr<base_logger>& logr : m_loggers) {
            logr->who(std::move(rwho));
        }
    }

    void log(const Level& level, const std::string& msg) {
        for (const std::unique_ptr<base_logger>& logr : m_loggers) {
            logr->log(level, msg);
        }
    }
};
