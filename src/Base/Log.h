//
// Created by 王晨辉 on 2023/4/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_LOG_H
#define GAMEPHYSICSINONEWEEKEND_LOG_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;


class DefaultSink : public spdlog::sinks::sink {
public:
    DefaultSink();

    void log(const spdlog::details::log_msg &msg) override;

    void flush() override;

    void set_pattern(const std::string &pattern) override;

    void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override;

private:
    std::shared_ptr<spdlog::logger> m_defaultLogger;
};


class Logger {
public:
    Logger(const Logger &logger) = delete;

    Logger &operator=(const Logger &logger) = delete;

    static Logger &GetLogger() {
        static Logger logger;
        return logger;
    }


    void SetLevel(spdlog::level::level_enum log_level) {
        // have to double set. Don't know why
        spdlog::set_level(log_level);
        m_logger->set_level(log_level);
    }

    template<typename... Args>
    void Info(const char *message, const Args... args) {
        m_logger->info(message, args...);
    }

    template<typename... Args>
    void Warn(const char *message, const Args... args) {
        m_logger->warn(message, args...);
    }

    template<typename... Args>
    void Error(const char *message, const Args... args) {
        m_logger->error(message, args...);
    }

    template<typename... Args>
    void Debug(const char *message, const Args... args) {
        m_logger->debug(message, args...);
    }


private:
    Logger() {
        auto defaultSink = std::make_shared<DefaultSink>();
        m_logger = std::make_shared<spdlog::logger>("physic", defaultSink);
        m_logger->set_level(spdlog::level::info);
    }

    std::shared_ptr<spdlog::logger> m_logger;
};

template<typename... Args>
void Info(const char *message, const Args... args) {
    Logger::GetLogger().template Info(message, args...);
}

template<typename... Args>
void Warn(const char *message, const Args... args) {
    Logger::GetLogger().template Warn(message, args...);
}

template<typename... Args>
void Error(const char *message, const Args... args) {
    Logger::GetLogger().template Error(message, args...);
}

template<typename... Args>
void Debug(const char *message, const Args... args) {
    Logger::GetLogger().template Debug(message, args...);
}


#define PHY_INFO(message, ...) Info(message, ##__VA_ARGS__)
#define PHY_WARN(message, ...) Warn(message, ##__VA_ARGS__)
#define PHY_DEBUG(message, ...) Debug(message, ##__VA_ARGS__)
#define PHY_ERROR(message, ...) Error(message, ##__VA_ARGS__)

#define PHY_ASSERT(condition, statement) \
do{                                      \
if(!(condition)){                        \
    PHY_ERROR(statement);                \
    assert(condition);\
}\
}while (false);

#define PHY_LEVEL_INFO Logger::GetLogger().SetLevel(spdlog::level::info);
#define PHY_LEVEL_ERR Logger::GetLogger().SetLevel(spdlog::level::err);
#define PHY_LEVEL_DEBUG Logger::GetLogger().SetLevel(spdlog::level::debug);
#define PHY_LEVEL_WARN Logger::GetLogger().SetLevel(spdlog::level::warn);


template<>
struct fmt::formatter<glm::vec3> : fmt::formatter<std::string> {
    static auto format(glm::vec3 v, format_context &ctx) -> decltype((ctx.out())) {
        std::string temp = "vector3: (";
        for (int i = 0; i < 3; i++) {
            temp.append("{:.2f}");
            temp += (",)"[i == 2]);
            temp.append(" ");
        }
        return format_to(ctx.out(), temp, v[0], v[1], v[2]);
    }
};

template<>
struct fmt::formatter<glm::vec2> : fmt::formatter<std::string> {
    static auto format(glm::vec2 v, format_context &ctx) -> decltype((ctx.out())) {
        std::string temp = "vector2: (";
        for (int i = 0; i < 2; i++) {
            temp.append("{:.2f}");
            temp += (",)"[i == 1]);
            temp.append(" ");
        }
        return format_to(ctx.out(), temp, v[0], v[1]);
    }
};

template<>
struct fmt::formatter<glm::vec4> : fmt::formatter<std::string> {
    static auto format(glm::vec4 v, format_context &ctx) -> decltype((ctx.out())) {
        std::string temp = "vector4: (";
        for (int i = 0; i < 4; i++) {
            temp.append("{:.2f}");
            temp += (",)"[i == 3]);
            temp.append(" ");
        }
        return format_to(ctx.out(), temp, v[0], v[1], v[2], v[3]);
    }
};


template<>
struct fmt::formatter<glm::mat2> : fmt::formatter<std::string> {
    static auto format(glm::mat2 v, format_context &ctx) -> decltype((ctx.out())) {
        std::string temp = "mat2x2: \n ";
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                temp.append("{:.2f}");
                temp += (",\n"[j == 1]);
                temp.append(" ");
            }
        }
        return format_to(ctx.out(), temp,
                         v[0][0], v[1][0],
                         v[0][1], v[1][1]);
    }
};


template<>
struct fmt::formatter<glm::mat3> : fmt::formatter<std::string> {
    static auto format(glm::mat3 v, format_context &ctx) -> decltype((ctx.out())) {
        std::string temp = "mat3x3: \n ";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                temp.append("{:.2f}");
                temp += (",\n"[j == 2]);
                temp.append(" ");
            }
        }
        return format_to(ctx.out(), temp,
                         v[0][0], v[1][0], v[2][0],
                         v[0][1], v[1][1], v[2][1],
                         v[0][2], v[1][2], v[2][2]);
    }
};


template<>
struct fmt::formatter<glm::mat4> : fmt::formatter<std::string> {
    static auto format(glm::mat4 v, format_context &ctx) -> decltype((ctx.out())) {
        std::string temp = "mat4x4: \n ";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp.append("{:.2f}");
                temp += (",\n"[j == 3]);
                temp.append(" ");
            }
        }
        return format_to(ctx.out(), temp,
                         v[0][0], v[1][0], v[2][0], v[3][0],
                         v[0][1], v[1][1], v[2][1], v[3][1],
                         v[0][2], v[1][2], v[2][2], v[3][2],
                         v[0][3], v[1][3], v[2][3], v[3][3]);
    }
};




#endif //GAMEPHYSICSINONEWEEKEND_LOG_H
