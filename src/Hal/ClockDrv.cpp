/* MIT License
 *
 * Copyright (c) 2019 - 2020 Andreas Merkle <web@blue-andi.de>
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
 * @brief  Clock driver
 * @author Yann Le Glaz <yann_le@web.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "ClockDrv.h"
#include "Settings.h"
#include "time.h"

#include <sys/time.h>
#include <Logging.h>

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

/* Instantiate the button driver singleton. */
ClockDrv ClockDrv::m_instance;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void ClockDrv::init()
{
    if (false == m_isClockDrvInitialized)
    {
        int32_t     gmtOffset           = 0;
        int16_t     daylightSavingValue = 0;
        String      ntpServerAddress;
        bool        isDaylightSaving    = false;
        struct tm   timeInfo            = { 0 };

        /* Get the GMT offset, daylight saving time and NTP server address from persistent memory. */
        if (false == Settings::getInstance().open(true))
        {
            LOG_WARNING("Use default values for NTP request.");

            gmtOffset           = Settings::getInstance().getGmtOffset().getDefault();
            isDaylightSaving    = Settings::getInstance().getDaylightSavingAdjustment().getDefault();
            ntpServerAddress    = Settings::getInstance().getNTPServerAddress().getDefault();
        }
        else
        {
            gmtOffset           = Settings::getInstance().getGmtOffset().getValue();
            isDaylightSaving    = Settings::getInstance().getDaylightSavingAdjustment().getValue();
            ntpServerAddress    = Settings::getInstance().getNTPServerAddress().getValue();

            Settings::getInstance().close();
        }

        if (true == isDaylightSaving)
        {
            daylightSavingValue = NTP_DAYLIGHT_OFFSET_SEC;
        }

        /* Configure NTP:
         * This will periodically synchronize the time. The time synchronization
         * period is determined by CONFIG_LWIP_SNTP_UPDATE_DELAY (default value is one hour).
         * To modify the variable, set CONFIG_LWIP_SNTP_UPDATE_DELAY in project configuration.
         * https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/system_time.html
         * https://github.com/espressif/esp-idf/issues/4386
         */
        configTime(gmtOffset, daylightSavingValue, ntpServerAddress.c_str());

        /* Wait for synchronization (default 5s) */
        if (false == getLocalTime(&timeInfo))
        {
            LOG_ERROR("Failed to synchronize time.");
        }
        else
        {
            LOG_INFO("Time successfully synchronized: %d:%d", timeInfo.tm_hour, timeInfo.tm_min);
        }

        m_isClockDrvInitialized = true;
    }
}

bool ClockDrv::getTime(tm *currentTime)
{
    const uint32_t WAIT_TIME_MS = 0;

    return getLocalTime(currentTime, WAIT_TIME_MS);
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/

