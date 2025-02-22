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
 * @brief  Test Logging.
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "TestLogging.h"
#include "TestLogger.h"

#include <unity.h>
#include <Logging.h>
#include <LogSinkPrinter.h>

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/**
 * Test Logging.
 *
 * Note: In order to let these tests pass the formating of the expected logMessage has to stay directly after the LogMessage (__LINE__-1),
 * otherwise the tests will fail.
 */
extern void testLogging()
{
    TestLogger      myTestLogger;
    LogSinkPrinter  myLogSink("test", &myTestLogger);
    const char*     printBuffer     = nullptr;
    const char*     LOG_MODULE      = strrchr(__FILE__, '\\'); /* Windows backslash */
    const char*     TEST_STRING_1   = "TestMessage";
    const String    TEST_STRING_2   = "TestMessageAsString";
    char            expectedLogMessage[128];
    int             lineNo          = 0;

    /* If no windows backslash is found, maybe its a linux slash? */
    if (nullptr == LOG_MODULE)
    {
        LOG_MODULE = strrchr(__FILE__, '/');
    }

    /* No directory found in filename. */
    if (nullptr == LOG_MODULE)
    {
        LOG_MODULE = __FILE__;
    }
    /* Overstep backslash/slash. */
    else
    {
        ++LOG_MODULE;
    }

    /* Check intial LogLevel. */
    TEST_ASSERT_TRUE(Logging::getInstance().registerSink(&myLogSink));
    TEST_ASSERT_TRUE(Logging::getInstance().selectSink("test"));
    TEST_ASSERT_EQUAL(Logging::getInstance().getLogLevel() , Logging::LOGLEVEL_ERROR);

    /* Set LogLevel to LOGLEVEL_INFO. */
    Logging::getInstance().setLogLevel(Logging::LOGLEVEL_INFO);
    TEST_ASSERT_EQUAL(Logging::getInstance().getLogLevel() , Logging::LOGLEVEL_INFO);

    /* Set LogLevel to LOGLEVEL_ERROR and trigger a LOG_INFO message. */
    Logging::getInstance().setLogLevel(Logging::LOGLEVEL_ERROR);
    LOG_INFO(TEST_STRING_1);
    (void)snprintf(expectedLogMessage, sizeof(expectedLogMessage), "%s", "");
    printBuffer = myTestLogger.getBuffer();
    TEST_ASSERT_EQUAL_STRING(expectedLogMessage, printBuffer);

    /* Check expected error log output, with type const char* string. */
    LOG_ERROR(TEST_STRING_1); lineNo = __LINE__;
    (void)snprintf(expectedLogMessage, sizeof(expectedLogMessage), "%*s %*s:%*d %s\n",
        LogSinkPrinter::LOG_LEVEL_LEN,
        "ERROR  ",
        LogSinkPrinter::FILENAME_LEN,
        LOG_MODULE,
        LogSinkPrinter::LINE_LEN,
        lineNo,
        TEST_STRING_1);
    printBuffer = myTestLogger.getBuffer();

    /* Skip timestamp */
    printBuffer = &printBuffer[LogSinkPrinter::TIMESTAMP_LEN + 1];

    TEST_ASSERT_EQUAL_STRING(expectedLogMessage, printBuffer);

    /* Check expected error log output, with type const String string. */
    LOG_ERROR(TEST_STRING_2);  lineNo = __LINE__;
    (void)snprintf(expectedLogMessage, sizeof(expectedLogMessage), "%*s %*s:%*d %s\n",
        LogSinkPrinter::LOG_LEVEL_LEN,
        "ERROR  ",
        LogSinkPrinter::FILENAME_LEN,
        LOG_MODULE,
        LogSinkPrinter::LINE_LEN,
        lineNo,
        TEST_STRING_2.c_str());
    printBuffer = myTestLogger.getBuffer();

    /* Skip timestamp */
    printBuffer = &printBuffer[LogSinkPrinter::TIMESTAMP_LEN + 1];

    TEST_ASSERT_EQUAL_STRING(expectedLogMessage, printBuffer);

    /* Unregister log sink and nothing shall be printed anymore. */
    Logging::getInstance().unregisterSink(&myLogSink);
    myTestLogger.clear();
    LOG_ERROR("Should not be shown.");
    TEST_ASSERT_EQUAL_UINT32(0, strlen(myTestLogger.getBuffer()));

    return;
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
