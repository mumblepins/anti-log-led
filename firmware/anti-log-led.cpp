#include "anti-log-led.h"

AntiLogLed::RgbLed::RgbLed(bool normalize) {
    _state=RGB.controlled();
    _normalize=normalize;
}

void AntiLogLed::RgbLed::control(bool state) {
    _state=state;
    RGB.control(_state);
}

bool AntiLogLed::RgbLed::controlled(void) {
    return _state;
}

void AntiLogLed::RgbLed::brightness(uint8_t bright) {
    _desBright=bright;
    updateLed();
}

void AntiLogLed::RgbLed::color(uint8_t red, uint8_t green, uint8_t blue) {
    // scales colors to make largest 255
    float scaleMax=1.0;
    
    if ((red==255)||(green==255)||(blue=255) || (!_normalize)) {
        // nothing ??
    } else {
        int maximum=max(red,max(green,blue));
        scaleMax=255.0/(float)maximum;
    }
    //
    _desRed=scaleMax*_colorScaling[red];
    _desGreen=scaleMax*_colorScaling[green];
    _desBlue=scaleMax*_colorScaling[blue];
    updateLed();    
}
    
void AntiLogLed::RgbLed::updateLed(void) {
    _brightness=round(_scalingFactor[_desBright]*255.0);
    _red=round(_scalingFactor[_desBright]*_desRed);
    _green=round(_scalingFactor[_desBright]*_desGreen);
    _blue=round(_scalingFactor[_desBright]*_desBlue);
    RGB.brightness(_brightness);
    RGB.color(_red,_green,_blue);
}