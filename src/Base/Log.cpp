//
// Created by 王晨辉 on 2023/4/28.
//

#include "Log.h"

DefaultSink::DefaultSink() {
    m_defaultLogger = spdlog::stdout_color_mt("default");
}

void DefaultSink::log(const spdlog::details::log_msg &msg) {
    m_defaultLogger->log(msg.level, msg.payload);
}

void DefaultSink::flush() {
    m_defaultLogger->flush();
}

void DefaultSink::set_pattern(const std::string &pattern) {

}

void DefaultSink::set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) {

}
