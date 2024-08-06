/* MIT License
 *
 * Copyright (c) 2019 - 2024 Andreas Merkle <web@blue-andi.de>
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
 * @brief  OpenWeather view with icon and text for 32x8 LED matrix
 * @author Andreas Merkle <web@blue-andi.de>
 * @addtogroup plugin
 *
 * @{
 */

#ifndef OPEN_WEATHER_VIEW_32X8_H
#define OPEN_WEATHER_VIEW_32X8_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <YAGfx.h>
#include <Fonts.h>
#include <IOpenWeatherView.h>
#include <BitmapWidget.h>
#include <TextWidget.h>
#include <Util.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * OpenWeather view for 32x8 LED matrix with icon and text.
 * 
 * +-----------------------------------------------------------------+
 * |                |                                                |
 * |                |                                                |
 * |                |                                                |
 * |   Icon         |                   Text                         |
 * |   8x8          |                   24x8                         |
 * |                |                                                |
 * |                |                                                |
 * |                |                                                |
 * +-----------------------------------------------------------------+
 */
class OpenWeatherView32x8 : public IOpenWeatherView
{
public:

    /**
     * Construct the view.
     */
    OpenWeatherView32x8() :
        IOpenWeatherView(),
        m_fontType(Fonts::FONT_TYPE_DEFAULT),
        m_bitmapWidget(BITMAP_WIDTH, BITMAP_HEIGHT, BITMAP_X, BITMAP_Y),
        m_textWidget(TEXT_WIDTH, TEXT_HEIGHT, TEXT_X, TEXT_Y),
        m_viewDurationTimer(),
        m_viewDuration(0U),
        m_units("metric"),
        m_weatherInfo(),
        m_weatherInfoId(0U),
        m_weatherInfoCurrent(),
        m_isWeatherInfoUpdated(false),
        m_iconFullPath()
    {
        m_bitmapWidget.setVerticalAlignment(Alignment::Vertical::VERTICAL_CENTER);
        m_bitmapWidget.setHorizontalAlignment(Alignment::Horizontal::HORIZONTAL_CENTER);
        
        m_textWidget.setVerticalAlignment(Alignment::Vertical::VERTICAL_CENTER);
        m_textWidget.setHorizontalAlignment(Alignment::Horizontal::HORIZONTAL_CENTER);
    }

    /**
     * Destroy the view.
     */
    virtual ~OpenWeatherView32x8()
    {
    }

    /**
     * Initialize view, which will prepare the widgets and the default values.
     */
    void init(uint16_t width, uint16_t height) override
    {
        UTIL_NOT_USED(width);
        UTIL_NOT_USED(height);
    }

    /**
     * Get font type.
     * 
     * @return The font type the view uses.
     */
    Fonts::FontType getFontType() const override
    {
        return m_fontType;
    }

    /**
     * Set font type.
     * 
     * @param[in] fontType  The font type which the view shall use.
     */
    void setFontType(Fonts::FontType fontType) override
    {
        m_fontType = fontType;
        m_textWidget.setFont(Fonts::getFontByType(m_fontType));
    }

    /**
     * Update the underlying canvas.
     * 
     * @param[in] gfx   Graphic functionality to draw on the underlying canvas.
     */
    void update(YAGfx& gfx) override;

    /**
     * Set the duration in ms, how long the view will be shown on the display.
     * It will be used to derive how long every enabled weather info is shown.
     * All enabled weather infos together will be equal to the configured duration.
     * 
     * Note, the view itself decides whether to use it or not.
     * 
     * @param[in] duration  View duration in ms
     */
    void setViewDuration(uint32_t duration) override
    {
        if (m_viewDuration != duration)
        {
            m_viewDuration = duration;

            /* Force recalculation of the duration of every weather info. */
            m_viewDurationTimer.stop();
        }
    }

    /**
     * Set the units to use temperature and wind speed.
     * 
     * @param[in] units The units which to set.
     */
    void setUnits(const String& units) override
    {
        m_units = units;
    }

    /**
     * Get the enabled weather information.
     * See WeatherInfo for every bit.
     * 
     * @return Weather information which is enabled.
     */
    uint8_t getWeatherInfo() const override
    {
        return m_weatherInfo;
    }

    /**
     * Set weather information, which shall be shown.
     * Use the bitfield WeatherInfo to combine the information by OR.
     * 
     * @param[in] weatherInfo   The weather info which to show.
     */
    void setWeatherInfo(uint8_t weatherInfo) override
    {
        if (m_weatherInfo != weatherInfo)
        {
            m_weatherInfo = weatherInfo;
            
            restartWeatherInfo();

            /* Force recalculation of the duration of every weather info. */
            m_viewDurationTimer.stop();
        }
    }

    /**
     * Restart showing the first weather info again.
     * The order is according to the order of WeatherInfo.
     */
    void restartWeatherInfo() override
    {
        m_weatherInfoId = 0U;

        /* Force recalculation of the duration of every weather info. */
        m_viewDurationTimer.stop();
    }

    /**
     * Set current weather information.
     * 
     * @param[in] info  Weather information
     */
    void setWeatherInfoCurrent(const WeatherInfoCurrent& info) override
    {
        if ((m_weatherInfoCurrent.iconId != info.iconId) ||
            (m_weatherInfoCurrent.temperature != info.temperature) ||
            (m_weatherInfoCurrent.humidity != info.humidity) ||
            (m_weatherInfoCurrent.uvIndex != info.uvIndex) ||
            (m_weatherInfoCurrent.windSpeed != info.windSpeed))
        {
            m_weatherInfoCurrent    = info;
            m_isWeatherInfoUpdated  = true;
        }
    }

    /**
     * Set forecast weather information.
     * 
     * Meaning of day:
     * - 0: Forecast for the next day
     * - 1: Forecast in two days
     * - N: Forecast in N days
     * 
     * @param[in] day   Weather information for the upcoming day [0; 4].
     * @param[in] info  Weather information
     */
    void setWeatherInfoForecast(uint8_t day, const WeatherInfoForecast& info) override
    {
        /* Not supported. */
    }

    /**
     * Is the weather forecast feature supported by the view?
     * 
     * @return If feature is supported, it will return true otherwise false.
     */
    static constexpr bool isWeatherForecastSupported()
    {
        return false;
    }

protected:

    /**
     * Bitmap size in pixels.
     */
    static const uint16_t   BITMAP_SIZE     = 8U;

    /**
     * Bitmap width in pixels.
     */
    static const uint16_t   BITMAP_WIDTH    = BITMAP_SIZE;

    /**
     * Bitmap height in pixels.
     */
    static const uint16_t   BITMAP_HEIGHT   = BITMAP_SIZE;

    /**
     * Bitmap widget x-coordinate in pixels.
     * Left aligned.
     */
    static const int16_t    BITMAP_X        = 0;

    /**
     * Bitmap widget y-coordinate in pixels.
     * Top aligned.
     */
    static const int16_t    BITMAP_Y        = 0;

    /**
     * Text width in pixels.
     */
    static const uint16_t   TEXT_WIDTH      = CONFIG_LED_MATRIX_WIDTH - BITMAP_WIDTH;

    /**
     * Text height in pixels.
     */
    static const uint16_t   TEXT_HEIGHT     = 8U;

    /**
     * Text widget x-coordinate in pixels.
     * Left aligned, after icon.
     */
    static const int16_t    TEXT_X          = BITMAP_WIDTH;

    /**
     * Text widget y-coordinate in pixels.
     * Top aligned.
     */
    static const int16_t    TEXT_Y          = 0;

    /**
     * Image path within the filesystem to weather condition icons.
     */
    static const char*      IMAGE_PATH;

    /**
     * Image path within the filesystem to standard icon.
     */
    static const char*      IMAGE_PATH_STD_ICON;

    /**
     * Image path within the filesystem to UV index icon.
     */
    static const char*      IMAGE_PATH_UVI_ICON;

    /**
     * Image path within the filesystem to humidity icon.
     */
    static const char*      IMAGE_PATH_HUMIDITY_ICON;

    /**
     * Image path within the filesystem to windspeed icon.
     */
    static const char*      IMAGE_PATH_WIND_ICON;

    /**
     * Default duration in ms used for the view.
     * If the view duration is INFINITE (value is 0), the default one is used.
     */
    static const uint32_t   VIEW_DURATION_DEFAULT   = SIMPLE_TIMER_SECONDS(30U);

    /**
     * Minimum duration of one single weather information in ms used by the view.
     */
    static const uint32_t   VIEW_DURATION_MIN       = SIMPLE_TIMER_SECONDS(4U);

    Fonts::FontType     m_fontType;             /**< Font type which shall be used if there is no conflict with the layout. */
    BitmapWidget        m_bitmapWidget;         /**< Bitmap widget used to show a icon. */
    TextWidget          m_textWidget;           /**< Text widget used to show some text. */
    uint32_t            m_viewDuration;         /**< The duration in ms, this view will be shown on the display. */
    SimpleTimer         m_viewDurationTimer;    /**< The timer used to determine which weather info to show on the display. */
    String              m_units;                /**< Units (default, metric, imperial) */
    uint8_t             m_weatherInfo;          /**< Use the bits to determine which weather info to show. */
    uint8_t             m_weatherInfoId;        /**< The weather info id is used to mask the weather info flag. Its the number of bit shifts. */
    WeatherInfoCurrent  m_weatherInfoCurrent;   /**< Current weather information. */
    bool                m_isWeatherInfoUpdated; /**< Is weather info updated? */
    String              m_iconFullPath;         /**< The full path to the icon. Used to detect whether an icon must really be changed. */

    OpenWeatherView32x8(const OpenWeatherView32x8& other);
    OpenWeatherView32x8& operator=(const OpenWeatherView32x8& other);

    /**
     * Get number of enabled weather infos.
     * 
     * @return Count of weather infos
     */
    uint8_t getWeatherInfoCount() const;

    /**
     * Select next weather info.
     */
    void nextWeatherInfo();

    /**
     * Get current active weather info.
     * 
     * @return Weather info 
     */
    WeatherInfo getActiveWeatherInfo() const;

    /**
     * Update the weather info on the view by considering the current active
     * weather info.
     */
    void updateWeatherInfoOnView();

    /**
     * Handle main weather info, which to show.
     */
    void handleWeatherInfo();

    /**
     * Get the full path to the icon in the filesystem by the weather icon id.
     * 
     * @param[out]  fullPath        Full path to icon in the filesystem.
     * @param[in]   weatherIconId   The weather icon id.
     */
    void getIconPathByWeatherIconId(String& fullPath, const String& weatherIconId) const;

    /**
     * Map the UV index value to a color corresponding the the icon.
    */
    const char* uvIndexToColor(uint8_t uvIndex);
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* OPEN_WEATHER_VIEW_32X8_H */

/** @} */
