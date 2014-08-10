#include "anti-log-led/anti-log-led.h"
AntiLogLed::RgbLed RGB;
bool doLoop=true;
int n;

int setBrightness(String args) {

    RGB.brightness(args.toInt());
    
    return 1;
}

int setColor(String args) {
    //"255,255,255"
    int i=args.indexOf(',');
    int red=args.substring(0,i).toInt();
    int j=args.lastIndexOf(',');
    int green=args.substring(i+1,j).toInt();
    int blue=args.substring(j+1).toInt();
    RGB.color(red,green,blue);
    return blue;
}

int doLoopChange(String args) {
    int i=args.toInt();
    if (i==1) doLoop=true;
    if (i==-1) doLoop=false;
    return i;
}

void setup() {
    //Serial.begin(9600);
    Spark.function("SetBright",setBrightness);
    Spark.function("SetColor",setColor);
    Spark.function("doLoop",doLoopChange);
    RGB.control(true);
    RGB.color(255,255,255);
}

void loop() {
    if (doLoop) {
       for (n=0; n<=255; n++) {
           RGB.brightness(n);
           delay(10);
       }
       for (n=255; n>=0; n--) {
           RGB.brightness(n);
           delay(10);
       }
    }
}
