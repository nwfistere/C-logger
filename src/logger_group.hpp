#pragma once

#include "logger.hpp"
#include "unmovable_logger.hpp"
#include <vector>

class logger_group {

private:
    std::vector<std::unique_ptr<base_logger>> m_loggers;
    std::vector<std::shared_ptr<base_logger>> m_unmovable_loggers;

    logger_group() = default;

public:
    logger_group(const logger_group&) = delete;
    logger_group& operator=(const logger_group&) = delete;
    static logger_group& get() {
        static logger_group m_instance;
        return m_instance;
    }

    void add_logger(std::unique_ptr<base_logger>& plogger) {
        m_loggers.push_back(std::move(plogger));
    }

    void add_logger(std::unique_ptr<base_logger>&& plogger) {
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

    // void add_logger(std::initializer_list<std::unique_ptr<base_logger>&> plogger...) {
    //     std::cout << "By init list reference" << std::endl;
    //     for (auto& logr : plogger) {
    //         logr->log(ERROR, "before\n");
    //         m_loggers.push_back(std::move(logr));
    //         logr->log(ERROR, "after\n");
    //     }
    //     m_loggers[0]->log(TRACE, "hi\n");
    // }

    void add_logger(std::vector<std::unique_ptr<base_logger>>&& loggers) {
        if (m_loggers.empty()) {
            for (auto& logr : loggers) {
                m_loggers.push_back(std::move(logr));
            }
        } else {
            m_loggers.reserve(m_loggers.size() + loggers.size());
            std::move(std::begin(loggers), std::end(loggers), std::back_inserter(m_loggers));
        }
    }

    void log(const Level& level, const std::string& msg) {
        for (const std::unique_ptr<base_logger>& logr : m_loggers) {
            logr->log(level, msg);
        }
    }
};
