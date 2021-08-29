//
// Created by bogdan on 8/21/2021.
//

#pragma once

#include <iostream>

enum LogLevel {
    DEBUG = 0,
    INFO = 1,
    ERROR = 2
};

inline LogLevel currentLevel = INFO;

template<LogLevel level, typename T>
void output_to_console( T t ) {
    if( level == LogLevel::INFO || ( level == LogLevel::DEBUG && currentLevel == LogLevel::DEBUG ) ) {
        std::cout << t;
        std::cout.flush();
    } else if( level == LogLevel::ERROR ) {
        std::cerr << t;
        std::cerr.flush();
    }
}

template<LogLevel level = LogLevel::INFO,typename T>
void print( T t ) {
    output_to_console<level>( t );
}

template<LogLevel level = LogLevel::INFO, typename T, typename... Args>
void print( T t, Args... args ) {
    output_to_console<level, false>( t );
    print<level>( args... );
}

template<LogLevel level>
class Logger {

public:
    template<typename T>
    constexpr const Logger& operator<<( T t ) const {
        print<level>( t );
        return *this;
    }

    template<typename... Args>
    constexpr const Logger& operator<<( Args... args ) const {
        print<level>( args... );
        return *this;
    }

    static constexpr char endl{'\n'};
};

inline constexpr static Logger<INFO> info;
inline constexpr static Logger<DEBUG> debug;
inline constexpr static Logger<ERROR> error;

inline void setLogLevel( LogLevel level ) {
    currentLevel = level;
    info << "Set current log level to " << currentLevel << info.endl;
}

