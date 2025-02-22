/* MIT License
 *
 * Copyright (c) 2019 - 2022 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  Logging
 * @author Yann Le Glaz <yann_le@web.de>
 *
 * @addtogroup utilities
 *
 * @{
 */

#ifndef __LOGGING_H__
#define __LOGGING_H__

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include <stdarg.h>
#include <stdint.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/** Severity: InfoLevel. */
#define LL_INFO                         (Logging::LOGLEVEL_INFO)

/** Severity: WarningLevel. */
#define LL_WARNING                      (Logging::LOGLEVEL_WARNING)

/** Severity: ErrorLevel. */
#define LL_ERROR                        (Logging::LOGLEVEL_ERROR)

/** Severity: FatalLevel. */
#define LL_FATAL                        (Logging::LOGLEVEL_FATAL)

/** Macro for Logging with LOGLEVEL_INFO. */
#define LOG_INFO(...)                   (Logging::getInstance().processLogMessage(__FILE__, __LINE__, LL_INFO, __VA_ARGS__))

/** Macro for Logging with LOGLEVEL_WARNING. */
#define LOG_WARNING(...)                (Logging::getInstance().processLogMessage(__FILE__, __LINE__, LL_WARNING, __VA_ARGS__))

/** Macro for Logging with LOGLEVEL_ERROR. */
#define LOG_ERROR(...)                  (Logging::getInstance().processLogMessage(__FILE__, __LINE__, LL_ERROR, __VA_ARGS__))

/** Macro for Logging with LOGLEVEL_FATAL. */
#define LOG_FATAL(...)                  (Logging::getInstance().processLogMessage(__FILE__, __LINE__, LL_FATAL, __VA_ARGS__))

/** Macro for switching the LogLevel. */
#define SWITCH_LOG_LEVEL_TO(logLevel)   (Logging::getInstance().setLogLevel(logLevel))

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/* Forward declaration */
class LogSink;

/**
 * Logging class for log messages depending on the previously set log level.
 */
class Logging
{
public:

    /**
     * Enumeration to distinguish between different levels of severity.
     */
    enum LogLevel
    {
        LOGLEVEL_INFO = 0,  /**< Log information interested for the user. */
        LOGLEVEL_WARNING,   /**< Log warning messages to show the user to pay attention. */
        LOGLEVEL_ERROR,     /**< Log error messages in case of a fault with an alternative solution. */
        LOGLEVEL_FATAL      /**< Log fatal messages in case there is no way out. */
    };

    /**
     * A single log message.
     */
    struct Msg
    {
        uint32_t            timestamp;  /**< Timestamp in ms */
        Logging::LogLevel   level;      /**< Log level */
        const char*         filename;   /**< Name of the file where this message is thrown. */
        int                 line;       /**< Line number in the file, where this message is thrown. */
        const char*         str;        /**< Message text */

        /**
         * Initializes a empty message.
         */
        Msg() :
            timestamp(0U),
            level(LOGLEVEL_INFO),
            filename(nullptr),
            line(0),
            str(nullptr)
        {
        }
    };

    /** The maximum size of the logMessage buffer to get the variable arguments. */
    static const uint16_t MESSAGE_BUFFER_SIZE   = 80U;

    /**
     * Get the Logging instance.
     *
     * @return Logging instance.
     */
    static Logging& getInstance()
    {
        static Logging instance; /* singleton idiom to force initialization in the first usage. */

        return instance;
    }

    /**
     * Register a log sink.
     *
     * @param[in] sink  Log sink
     *
     * @return If successful registered it will return true otherwise false.
     */
    bool registerSink(LogSink* sink);

    /**
     * Unregister a log sink.
     *
     * @param[in] sink  Log sink
     */
    void unregisterSink(LogSink* sink);

    /**
     * Select log sink.
     *
     * @param[in] name Log sink name
     *
     * @return If sink is selected, it will return true otherwise false.
     */
    bool selectSink(const String& name);

    /**
     * Get selected sink.
     *
     * @return Selected sink
     */
    LogSink* getSelectedSink();

    /**
     * Set the logLevel.
     *
     * @param[in] logLevel The new logLevel.
    */
    void setLogLevel(const LogLevel logLevel);

    /**
     * Get the current logLevel.
     *
     * @return The current logLevel.
    */
    LogLevel getLogLevel() const;

    /**
     * Write a formatable logMessage to the current output,
     * if the severity is >= the current logLevel, otherwise the logMessage is discarded.
     *
     * @param[in] file              Name of the file
     * @param[in] line              Line number in the file
     * @param[in] messageLogLevel   The logLevel.
     * @param[in] format            The format of the variable arguments.
     * @param[in] ...               The variable arguments.
     *
     * @note The max. size of a logMessage is restricted by MESSAGE_BUFFER_SIZE.
     */
    void processLogMessage(const char* file, int line, const LogLevel messageLogLevel, const char* format, ...);

    /**
     * Write a formatable logMessage to the current output,
     * if the severity is >= the current logLevel, otherwise the logMessage is discarded.
     *
     * @param[in] file              Name of the file
     * @param[in] line              Line number in the file
     * @param[in] messageLogLevel   The logLevel.
     * @param[in] message           The message as string.
     *
     * @note The max. size of a logMessage is restricted by MESSAGE_BUFFER_SIZE.
     */
    void processLogMessage(const char* file, int line, const LogLevel messageLogLevel, const String& message);

    /**
     * Write a formatable logMessage to the current output,
     * if the severity is >= the current logLevel, otherwise the logMessage is discarded.
     *
     * @param[in] timestamp         Timestamp in ms.
     * @param[in] logger            Logger name.
     * @param[in] messageLogLevel   The logLevel.
     * @param[in] message           The message as string.
     *
     * @note The max. size of a logMessage is restricted by MESSAGE_BUFFER_SIZE.
     */
    void processLogMessage(uint32_t timestamp, const String& logger, const LogLevel messageLogLevel, const String& message);

    /** Number of supported log sinks. */
    static const uint8_t MAX_SINKS = 2U;

private:

    /** The current log level. */
    LogLevel    m_currentLogLevel;

    /** List of log sinks */
    LogSink*    m_sinks[MAX_SINKS];

    /** Active sink */
    LogSink*    m_selectedSink;

    /**
     * Checks wether the given severity of a logMessage is valid to be printed.
     *
     * @param[in] logLevel the logLevel that has to be checked.
     *
     * @return True if the severity is >= the current logLevel, otherwise false.
    */
    bool isSeverityValid(const LogLevel logLevel);

    /**
     * Extracts the basename of a file from a given path.
     *
     * @param[in] path The path as retrieved from __FILE__.
     *
     * @return The basename.
    */
    const char* getBaseNameFromPath(const char* path) const;

    /**
     * Construct Logging.
     */
    Logging() :
        m_currentLogLevel(LOGLEVEL_ERROR),
        m_sinks(),
        m_selectedSink(nullptr)
    {
        uint8_t index = 0U;

        for(index = 0U; index < MAX_SINKS; ++index)
        {
            m_sinks[index] = nullptr;
        }
    }

    /**
     * Destroys Logging.
     */
    ~Logging()
    {

    }

    /* Prevent copying */
    Logging(const Logging&);
    Logging&operator=(const Logging&);
};

/**
 * Logging sink interface.
 */
class LogSink
{
public:

    /**
     * Constructs the log sink.
     */
    LogSink()
    {
    }

    /**
     * Destroys the log sink.
     */
    virtual ~LogSink()
    {
    }

    /**
     * Get sink name.
     *
     * @return Name of the sink.
     */
    virtual const String& getName() const = 0;

    /**
     * Send a log message to this sink.
     *
     * @param[in] msg   Log message
     */
    virtual void send(const Logging::Msg& msg) = 0;

private:
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* __LOGGING_H__ */

/** @} */