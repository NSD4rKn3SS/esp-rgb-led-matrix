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
 * @brief  Test simple timer.
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "TestSimpleTimer.h"

#include <unity.h>
#include <SimpleTimer.hpp>

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
 * Test simple timer.
 */
extern void testSimpleTimer()
{
    SimpleTimer testTimer;

    /* Timer must be stopped */
    TEST_ASSERT_FALSE(testTimer.isTimerRunning());
    TEST_ASSERT_FALSE(testTimer.isTimeout());

    /* Start and check */
    testTimer.start(0U);
    TEST_ASSERT_TRUE(testTimer.isTimerRunning());
    TEST_ASSERT_TRUE(testTimer.isTimeout());
    TEST_ASSERT_TRUE(testTimer.isTimerRunning());

    /* Stop timer and check again */
    testTimer.stop();
    TEST_ASSERT_FALSE(testTimer.isTimerRunning());
    TEST_ASSERT_FALSE(testTimer.isTimeout());

    /* Restart timer */
    testTimer.restart();
    TEST_ASSERT_TRUE(testTimer.isTimerRunning());
    TEST_ASSERT_TRUE(testTimer.isTimeout());

    /* Start timer and start it again after timeout. */
    testTimer.start(0U);
    TEST_ASSERT_TRUE(testTimer.isTimeout());
    testTimer.start(100U);
    TEST_ASSERT_FALSE(testTimer.isTimeout());
    testTimer.stop();

    return;
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
