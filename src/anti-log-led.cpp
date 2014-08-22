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
    _inError = _inBlink = false;
    updateBrightness(bright);
    updateLed();
}

void AntiLogLed::RgbLed::updateBrightness(uint8_t bright) {
    _desBright = bright;
}

void AntiLogLed::RgbLed::color(uint8_t red, uint8_t green, uint8_t blue) {
    _inError = _inBlink = false;
    updateColor(red, green, blue);
    updateLed();
}

void AntiLogLed::RgbLed::updateColor(uint8_t red, uint8_t green, uint8_t blue) {
    if ((!_normalize) || (red == 255) || (green == 255) || (blue == 255)) {
        _desRed = _colorScaling[red];
        _desGreen = _colorScaling[green];
        _desBlue = _colorScaling[blue];
    } else {
        float maximum = max(red, green);
        maximum = max(maximum, blue);
        float scaleMax = 255.0 / (float) maximum;
        _desRed = _colorScaling[round(scaleMax * red)];
        _desGreen = _colorScaling[round(scaleMax * green)];
        _desBlue = _colorScaling[round(scaleMax * blue)];
    }
}

void AntiLogLed::RgbLed::colorAndBrightness(uint8_t red, uint8_t green, uint8_t blue, uint8_t bright) {
    _inError = _inBlink = false;
    updateColor(red, green, blue);
    updateBrightness(bright);
    updateLed();
}

void AntiLogLed::RgbLed::blinkLED(uint8_t red, uint8_t green, uint8_t blue, unsigned long blinkTime) {
    if (!_inBlink) {
        _inBlink = _currentOn = true;
        _inError = false;
        updateColor(red, green, blue);
        updateBrightness(255);
        updateLed();
        _lastSwitch = millis();
        return;
    }
    if ((millis() - _lastSwitch) < blinkTime) return;
    if (_currentOn) {
        _currentOn = false;
        updateBrightness(0);
        updateLed();
        _lastSwitch = millis();
    } else {
        _currentOn=true;
        updateBrightness(255);
        updateLed();
        _lastSwitch = millis();
    }
}

void AntiLogLed::RgbLed::blinkError(uint8_t red, uint8_t green, uint8_t blue, uint8_t pulses) {
    if (!_inError) {
        _inError = _currentOn = true;
        _inBlink = false;
        _sosTrack = 0;
        updateColor(red, green, blue);
        updateBrightness(255);
        updateLed();
        _lastSwitch = millis();
        _curDelay = DOT_DURATION;
        return;
    }
    switch (_sosTrack) {
        case 0: case 1: case 2: case 6: case 7: // S, three dots
            if ((millis() - _lastSwitch) < _curDelay) return;
            if (!_currentOn) {
                updateBrightness(255);
                updateLed();
                _lastSwitch = millis();
                _currentOn = true;

                _curDelay = DOT_DURATION;
                return;
            }
            updateBrightness(0);
            updateLed();
            _lastSwitch = millis();
            _currentOn = false;
            _sosTrack++;

            _curDelay = DOT_DURATION;
            return;
            break;
        case 8:
            if ((millis() - _lastSwitch) < _curDelay) return;
            if (!_currentOn) {
                updateBrightness(255);
                updateLed();
                _lastSwitch = millis();
                _currentOn = true;
                _curDelay = DOT_DURATION;
                return;
            }
            updateBrightness(0);
            updateLed();
            _lastSwitch = millis();
            _curDelay = DASH_DURATION * 2;
            _currentOn = false;
            _sosTrack++;
            return;
            break;
        case 3: case 4: case 5: // O, 3 dashes
            if ((millis() - _lastSwitch) < _curDelay) return;
            if (!_currentOn) {
                updateBrightness(255);
                updateLed();
                _lastSwitch = millis();
                _currentOn = true;

                _curDelay = DASH_DURATION;
                return;
            }
            updateBrightness(0);
            updateLed();
            _lastSwitch = millis();
            _currentOn = false;
            _sosTrack++;

            _curDelay = DOT_DURATION;
            return;
            break;
        default: // number of error Dots
            if ((millis() - _lastSwitch) < _curDelay) return;
            if (!_currentOn) {
                updateBrightness(255);
                updateLed();
                _lastSwitch = millis();
                _currentOn = true;
                _curDelay = DOT_DURATION * 2;
                return;
            }
            updateBrightness(0);
            updateLed();
            _lastSwitch = millis();
            _currentOn = false;
            _sosTrack++;
            if ((_sosTrack - 8) > pulses) {
                _sosTrack = 0;
                _curDelay = DASH_DURATION * 3;
            } else {
                _curDelay = DOT_DURATION;
            }
            return;
            break;
    }

}

void AntiLogLed::RgbLed::updateLed(void) {
    _brightness = round(_scalingFactor[_desBright]*255.0);
    _red = round(_scalingFactor[_desBright] * _desRed);
    _green = round(_scalingFactor[_desBright] * _desGreen);
    _blue = round(_scalingFactor[_desBright] * _desBlue);
    RGB.brightness(_brightness);
    RGB.color(_red, _green, _blue);
}