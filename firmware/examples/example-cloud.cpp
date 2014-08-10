#include "anti-log-led/anti-log-led.h"
AntiLogLed::RgbLed RGBControl;
// AntiLogLed::RgbLed RGBControl(false);  // don't do normalization on color values
bool doLoop = true;
int n;

int setBrightness(String args) {

    RGBControl.brightness(args.toInt());

    return 1;
}

int setColor(String args) {
    //"255,255,255"
    int i = args.indexOf(',');
    int red = args.substring(0, i).toInt();
    int j = args.lastIndexOf(',');
    int green = args.substring(i + 1, j).toInt();
    int blue = args.substring(j + 1).toInt();
    RGBControl.color(red, green, blue);
    return blue;
}

int doLoopChange(String args) {
    int i = args.toInt();
    if (i == 1) doLoop = true;
    if (i == -1) doLoop = false;
    return i;
}

void setup() {
    //Serial.begin(9600);
    Spark.function("SetBright", setBrightness);
    Spark.function("SetColor", setColor);
    Spark.function("doLoop", doLoopChange);
    RGBControl.control(true);
    RGBControl.color(255, 255, 255);
}

void loop() {
    if (doLoop) {
        for (n = 0; n <= 255; n++) {
            RGBControl.brightness(n);
            delay(10);
        }
        for (n = 255; n >= 0; n--) {
            RGBControl.brightness(n);
            delay(10);
        }
    }
}
