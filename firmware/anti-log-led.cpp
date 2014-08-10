/* Anti-log Led correction, to correct for non-linear eyes :-)
 * Copyright (C) 2014  Daniel Sullivan (mumblepins)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 * 
 * Used http://electronics.stackexchange.com/questions/1983/correcting-for-non-linear-brightness-in-leds-when-using-pwm
 * as a reference
 * 
 * Can either be constructed with normalization on or off.  If normalization
 * is on (default), and you feed it RgbLed.color(10,0,0) and
 * RgbLed.brightness(255) the color values will be normalized to 255,0,0
 * Note that this is different behavior than the built in library.  You can turn
 * it off by constructing with 
 * AntiLogLed::RgbLed RGBControl(false);
 *  
 */

#include "anti-log-led.h"


AntiLogLed::RgbLed::RgbLed(bool normalize) {
    _state = RGB.controlled();
    _normalize = normalize;
}

void AntiLogLed::RgbLed::control(bool state) {
    _state = state;
    RGB.control(_state);
}

bool AntiLogLed::RgbLed::controlled(void) {
    return _state;
}

void AntiLogLed::RgbLed::brightness(uint8_t bright) {
    _desBright = bright;
    updateLed();
}

void AntiLogLed::RgbLed::color(uint8_t red, uint8_t green, uint8_t blue) {
    // scales colors to make largest 255
    float scaleMax = 1.0;

    if ((red == 255) || (green == 255) || (blue = 255) || (!_normalize)) {
        // nothing ??
    } else {
        int maximum = max(red, max(green, blue));
        scaleMax = 255.0 / (float) maximum;
    }
    //
    _desRed = _colorScaling[(int) round(scaleMax * red)];
    _desGreen = _colorScaling[(int) round(scaleMax * green)];
    _desBlue = _colorScaling[(int) round(scaleMax * blue)];
    updateLed();
}

void AntiLogLed::RgbLed::updateLed(void) {
    _brightness = round(_scalingFactor[_desBright]*255.0);
    _red = round(_scalingFactor[_desBright] * _desRed);
    _green = round(_scalingFactor[_desBright] * _desGreen);
    _blue = round(_scalingFactor[_desBright] * _desBlue);
    RGB.brightness(_brightness);
    RGB.color(_red, _green, _blue);
}