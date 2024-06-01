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
 * @brief  Plugin view for 32x8 LED matrix
 * @author Andreas Merkle <web@blue-andi.de>
 * @addtogroup plugin
 *
 * @{
 */

#ifndef TEMPHUMID_PLUGIN_VIEW_32X8_H
#define TEMPHUMID_PLUGIN_VIEW_32X8_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <YAGfx.h>
#include <Fonts.h>
#include <BitmapWidget.h>
#include <TextWidget.h>
#include <FileSystem.h>
#include <Util.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Internal plugin functionality. */
namespace _TempHumidPlugin
{

/**
 * View for 32x8 LED matrix.
 */
class View
{
public:

    /**
     * Construct the layout.
     */
    View() :
        m_fontType(Fonts::FONT_TYPE_DEFAULT),
        m_bitmapWidget(BITMAP_WIDTH, BITMAP_HEIGHT, 0, 0),
        m_textWidget(TEXT_WIDTH, TEXT_HEIGHT, TEXT_X, 0)
    {
    }

    /**
     * Destroy the layout.
     */
    ~View()
    {
    }

    /**
     * Initialize view, which will prepare the widgets and the default values.
     */
    void init(uint16_t width, uint16_t height)
    {
        UTIL_NOT_USED(width);

        m_textWidget.setFormatStr("\\calign?");
        (void)m_bitmapWidget.load(FILESYSTEM, IMAGE_PATH_TEMP_ICON);

        /* Move the text into the middle of the height. */
        if (height > m_textWidget.getFont().getHeight())
        {
            uint16_t diffY = height - m_textWidget.getFont().getHeight();
            uint16_t offsY = diffY / 2U;

            m_textWidget.move(BITMAP_WIDTH, offsY);
        }
    }

    /**
     * Get font type.
     * 
     * @return The font type the view uses.
     */
    Fonts::FontType getFontType() const
    {
        return m_fontType;
    }

    /**
     * Set font type.
     * 
     * @param[in] fontType  The font type which the view shall use.
     */
    void setFontType(Fonts::FontType fontType)
    {
        m_fontType = fontType;
        m_textWidget.setFont(Fonts::getFontByType(m_fontType));
    }

    /**
     * Update the underlying canvas.
     * 
     * @param[in] gfx   Graphic functionality to draw on the underlying canvas.
     */
    void update(YAGfx& gfx)
    {
        gfx.fillScreen(ColorDef::BLACK);
        m_bitmapWidget.update(gfx);
        m_textWidget.update(gfx);
    }

    /**
     * Supported icons.
     */
    enum Icon
    {
        ICON_TEMPERATURE = 0,   /**< Temperature icon. */
        ICON_HUMIDITY,          /**< Humidity icon. */
        ICON_MAX                /**< Max. number of icons. */
    };

    /**
     * Load dedicated icon.
     * 
     * @param[in] type  The icon type which to set.
     */
    void loadIcon(Icon type);

    /**
     * Get text (non-formatted).
     * 
     * @return Text
     */
    String getText() const
    {
        return m_textWidget.getStr();
    }

    /**
     * Get text (formatted).
     * 
     * @return Text
     */
    String getFormatText() const
    {
        return m_textWidget.getFormatStr();
    }

    /**
     * Set text (formatted).
     * 
     * @param[in] formatText    Formatted text to show.
     */
    void setFormatText(const String& formatText)
    {
        m_textWidget.setFormatStr(formatText);
    }

private:

    /**
     * Path to the temperature icon
     */    
    static const char*      IMAGE_PATH_TEMP_ICON;

    /**
     * Path to the humidity icon
     */
    static const char*      IMAGE_PATH_HUMID_ICON;

    /**
     * Table of icons mapped according to the icon enumeration.
     */
    static const char*      ICON_TABLE[ICON_MAX];

    /**
     * Bitmap width in pixels.
     */
    static const uint16_t   BITMAP_WIDTH    = CONFIG_LED_MATRIX_HEIGHT;

    /**
     * Bitmap height in pixels.
     */
    static const uint16_t   BITMAP_HEIGHT   = CONFIG_LED_MATRIX_HEIGHT;

    /**
     * Text width in pixels.
     */
    static const uint16_t   TEXT_WIDTH      = CONFIG_LED_MATRIX_WIDTH - BITMAP_WIDTH;

    /**
     * Text height in pixels.
     */
    static const uint16_t   TEXT_HEIGHT     = CONFIG_LED_MATRIX_HEIGHT;

    /**
     * Text widget x-coordinate in pixels.
     */
    static const int16_t    TEXT_X          = BITMAP_WIDTH;

    Fonts::FontType m_fontType;     /**< Font type which shall be used if there is no conflict with the layout. */
    BitmapWidget    m_bitmapWidget; /**< Bitmap widget used to show a icon. */
    TextWidget      m_textWidget;   /**< Text widget used to show some text. */

    View(const View& other);
    View& operator=(const View& other);
};

} /* _TempHumidPlugin */

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* TEMPHUMID_PLUGIN_VIEW_32X8_H */

/** @} */
