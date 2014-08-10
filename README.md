anti-log-led
============
Anti-log Led correction, to correct for non-linear eyes :-)
Copyright (C) 2014  Daniel Sullivan (mumblepins)
 
Used http://electronics.stackexchange.com/questions/1983/correcting-for-non-linear-brightness-in-leds-when-using-pwm as a reference

Can either be constructed with normalization on or off.  If normalization is on (default), and you feed it `RgbLed.color(10,0,0)` and `RgbLed.brightness(255)` the resulting values will be normalized to 255,0,0

Note that this is different behavior than the built in library.  You can turn it off by constructing with  `AntiLogLed::RgbLed RGBControl(false);`
