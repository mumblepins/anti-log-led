#ifndef _ANTI_LOG_LED
#define _ANTI_LOG_LED

// Make library cross-compatiable
// with Arduino, GNU C++ for tests, and Spark.
//#if defined(ARDUINO) && ARDUINO >= 100
//#include "Arduino.h"
//#elif defined(SPARK)
//#include "application.h"
//#endif

// TEMPORARY UNTIL the stuff that supports the code above is deployed to the build IDE
#include "application.h"
#include "math.h"

namespace AntiLogLed {

    class RgbLed {
    private:
        bool _state;
        float _scalingFactor[256]={0.0000,0.0522,0.0738,0.0905,0.1046,0.1170,0.1282,0.1386,0.1483,0.1574,0.1660,0.1742,0.1821,0.1897,0.1969,0.2040,0.2108,0.2175,0.2239,0.2302,0.2364,0.2424,0.2482,0.2540,0.2596,0.2652,0.2706,0.2760,0.2812,0.2864,0.2915,0.2965,0.3015,0.3063,0.3112,0.3159,0.3206,0.3253,0.3299,0.3344,0.3389,0.3434,0.3478,0.3521,0.3564,0.3607,0.3650,0.3692,0.3733,0.3775,0.3815,0.3856,0.3896,0.3936,0.3976,0.4016,0.4055,0.4094,0.4132,0.4171,0.4209,0.4247,0.4284,0.4322,0.4359,0.4396,0.4433,0.4469,0.4506,0.4542,0.4578,0.4614,0.4649,0.4685,0.4720,0.4755,0.4790,0.4825,0.4860,0.4894,0.4929,0.4963,0.4997,0.5031,0.5065,0.5098,0.5132,0.5165,0.5198,0.5232,0.5265,0.5298,0.5330,0.5363,0.5396,0.5428,0.5460,0.5493,0.5525,0.5557,0.5589,0.5621,0.5653,0.5684,0.5716,0.5747,0.5779,0.5810,0.5841,0.5872,0.5904,0.5935,0.5965,0.5996,0.6027,0.6058,0.6088,0.6119,0.6149,0.6180,0.6210,0.6241,0.6271,0.6301,0.6331,0.6361,0.6391,0.6421,0.6451,0.6481,0.6510,0.6540,0.6570,0.6599,0.6629,0.6658,0.6688,0.6717,0.6747,0.6776,0.6805,0.6834,0.6863,0.6893,0.6922,0.6951,0.6980,0.7009,0.7037,0.7066,0.7095,0.7124,0.7153,0.7181,0.7210,0.7239,0.7267,0.7296,0.7324,0.7353,0.7381,0.7410,0.7438,0.7466,0.7495,0.7523,0.7551,0.7580,0.7608,0.7636,0.7664,0.7692,0.7720,0.7749,0.7777,0.7805,0.7833,0.7861,0.7889,0.7917,0.7944,0.7972,0.8000,0.8028,0.8056,0.8084,0.8111,0.8139,0.8167,0.8195,0.8222,0.8250,0.8278,0.8305,0.8333,0.8361,0.8388,0.8416,0.8444,0.8471,0.8499,0.8526,0.8554,0.8581,0.8609,0.8636,0.8664,0.8691,0.8719,0.8746,0.8773,0.8801,0.8828,0.8856,0.8883,0.8910,0.8938,0.8965,0.8992,0.9020,0.9047,0.9074,0.9102,0.9129,0.9156,0.9184,0.9211,0.9238,0.9265,0.9293,0.9320,0.9347,0.9374,0.9402,0.9429,0.9456,0.9483,0.9511,0.9538,0.9565,0.9592,0.9619,0.9647,0.9674,0.9701,0.9728,0.9755,0.9783,0.9810,0.9837,0.9864,0.9891,0.9918,0.9946,0.9973,1.0000};
        float _colorScaling[256]={0.0000,0.0220,0.0445,0.0674,0.0909,0.1149,0.1394,0.1644,0.1900,0.2162,0.2429,0.2702,0.2982,0.3267,0.3559,0.3857,0.4161,0.4473,0.4791,0.5116,0.5448,0.5788,0.6135,0.6490,0.6852,0.7223,0.7601,0.7988,0.8384,0.8788,0.9201,0.9623,1.0054,1.0495,1.0946,1.1406,1.1877,1.2358,1.2849,1.3352,1.3865,1.4390,1.4926,1.5474,1.6034,1.6606,1.7191,1.7789,1.8400,1.9024,1.9662,2.0314,2.0981,2.1662,2.2358,2.3069,2.3796,2.4539,2.5299,2.6075,2.6868,2.7678,2.8506,2.9353,3.0218,3.1102,3.2006,3.2929,3.3873,3.4838,3.5823,3.6831,3.7860,3.8912,3.9988,4.1086,4.2210,4.3357,4.4530,4.5729,4.6954,4.8206,4.9486,5.0794,5.2130,5.3496,5.4892,5.6319,5.7776,5.9266,6.0789,6.2345,6.3936,6.5561,6.7222,6.8920,7.0655,7.2428,7.4240,7.6092,7.7985,7.9919,8.1896,8.3916,8.5981,8.8091,9.0247,9.2451,9.4703,9.7005,9.9358,10.1762,10.4219,10.6730,10.9296,11.1918,11.4599,11.7338,12.0137,12.2998,12.5922,12.8910,13.1964,13.5085,13.8274,14.1534,14.4865,14.8270,15.1749,15.5305,15.8939,16.2653,16.6449,17.0328,17.4292,17.8343,18.2484,18.6716,19.1040,19.5460,19.9977,20.4593,20.9310,21.4132,21.9059,22.4095,22.9241,23.4500,23.9875,24.5369,25.0983,25.6720,26.2584,26.8576,27.4700,28.0959,28.7356,29.3893,30.0573,30.7401,31.4379,32.1510,32.8798,33.6246,34.3858,35.1637,35.9587,36.7712,37.6016,38.4502,39.3175,40.2038,41.1096,42.0354,42.9815,43.9483,44.9365,45.9464,46.9784,48.0332,49.1111,50.2128,51.3386,52.4892,53.6651,54.8669,56.0951,57.3502,58.6330,59.9440,61.2838,62.6530,64.0524,65.4825,66.9440,68.4377,69.9642,71.5243,73.1187,74.7481,76.4133,78.1152,79.8544,81.6319,83.4485,85.3050,87.2023,89.1414,91.1230,93.1483,95.2180,97.3333,99.4950,101.7043,103.9622,106.2696,108.6279,111.0379,113.5009,116.0181,118.5906,121.2197,123.9066,126.6525,129.4588,132.3268,135.2579,138.2534,141.3147,144.4434,147.6408,150.9085,154.2481,157.6610,161.1490,164.7137,168.3567,172.0799,175.8848,179.7735,183.7476,187.8091,191.9598,196.2019,200.5371,204.9677,209.4957,214.1232,218.8525,223.6857,228.6252,233.6733,238.8323,244.1048,249.4932,255.0000};
        int _brightness,_red,_green,_blue;
        float _desRed, _desGreen, _desBlue;
        int _desBright;
        bool _normalize
        void updateLed(void);
    public:
        RgbLed(bool normalize=true);
        void control(bool state);
        bool controlled(void);
        void color(uint8_t red, uint8_t green, uint8_t blue);
        
        //note, with a little work, the brightness resolution could be increased substantially, by scaling everything
        void brightness(uint8_t bright);
    };
}

#endif