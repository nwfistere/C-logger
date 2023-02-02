#include "logger.hpp"
#include <vector>

class logger_group {

private:
    std::vector<base_logger*> m_loggers;

public:
    logger_group(base_logger* logger) {
        m_loggers.push_back(std::move(logger));
    }

    logger_group(std::vector<base_logger*>& loggers) {
        m_loggers = std::move(loggers);
    };

    logger_group(base_logger* args...) {     
        m_loggers.push_back({ args });
    };


};
