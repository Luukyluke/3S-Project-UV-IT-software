#include "Wire.h" 
#include "Arduino.h"
#include "Adafruit_TSL2591.h"
#include "SparkFun_VEML6075_Arduino_Library.h"
#include "SparkFun_AS7265X.h"
#include "MLX90621.h"
#include "High_Temp.h"
#include "DHT.h"
#define TCAADDR 0x70

/* Assign a unique ID to the sensors */
DHT DHT1(7, DHT22);
Adafruit_TSL2591 TSL1; 
Adafruit_TSL2591 TSL2;
Adafruit_TSL2591 TSL3;
VEML6075 VEML1;
VEML6075 VEML2; 
VEML6075 VEML3;
AS7265X TRIAD1;
AS7265X TRIAD2;
AS7265X TRIAD3;
MLX90621 MLX1; 
MLX90621 MLX2; 
MLX90621 MLX3;
HighTemp BP1(A1,A0);
HighTemp BP2(A3,A2);
HighTemp BP3(A5,A4);

/* TCA ports from the port tester*/
//TCA Port #0
//Found I2C 0x10
//Found I2C 0x29
//Found I2C 0x49
//Found I2C 0x50
//Found I2C 0x51
//Found I2C 0x52
//Found I2C 0x53
//Found I2C 0x54
//Found I2C 0x55
//Found I2C 0x56
//Found I2C 0x57
//Found I2C 0x60
//TCA Port #1
//Found I2C 0x10
//Found I2C 0x29
//Found I2C 0x49
//Found I2C 0x50
//Found I2C 0x51
//Found I2C 0x52
//Found I2C 0x53
//Found I2C 0x54
//Found I2C 0x55
//Found I2C 0x56
//Found I2C 0x57
//Found I2C 0x60
//TCA Port #2
//Found I2C 0x10
//Found I2C 0x29
//Found I2C 0x49
//Found I2C 0x50
//Found I2C 0x51
//Found I2C 0x52
//Found I2C 0x53
//Found I2C 0x54
//Found I2C 0x55
//Found I2C 0x56
//Found I2C 0x57
//Found I2C 0x60
//TCA Port #3
//TCA Port #4
//TCA Port #5
//TCA Port #6
//TCA Port #7

/* Data output*/
//Time:         ms,
//DHT:          Hum,Temp,
//Blackpanel1:  BP1,
//MLX1:         Min1,Max1,
//TSL1:         IR1,Visible1,Full1,Lux1,
//VEML1:        UV1A,UV1B,
//TRIAD1:       T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,
//Blackpanel2:  BP2,
//MLX1:         Min2,Max2,
//TSL1:         IR2,Visible2,Full2,Lux2,
//VEML1:        UV2A,UV2B,
//TRIAD1:       T2xA,T2xB,T2xC,T2xD,T2xE,T2xF,T2xG,T2xH,T2xI,T2xJ,T2xK,T2xL,T2xR,T2xS,T2xT,T2xU,T2xV,T2xW,
//Blackpanel3:  BP3,
//MLX3:         Min3,Max3,
//TSL3:         IR3,Visible3,Full3,Lux3,
//VEML3:        UV3A,UV3B,
//TRIAD3:       T3xA,T3xB,T3xC,T3xD,T3xE,T3xF,T3xG,T3xH,T3xI,T3xJ,T3xK,T3xL,T3xR,T3xS,T3xT,T3xU,T3xV,T3xW,

//ms,Hum1,Temp1,BP1,Min1,Max1,IR1,Visible1,Full1,Lux1,UV1A,UV1B,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,BP2,Min2,Max2,IR2,Visible2,Full2,Lux2,UV2A,UV2B,T2xA,T2xB,T2xC,T2xD,T2xE,T2xF,T2xG,T2xH,T2xI,T2xJ,T2xK,T2xL,T2xR,T2xS,T2xT,Tx2U,T2xV,T2xW,BP3,in3,Max3,IR3,Visible3,Full3,Lux3,UV3A,UV3B,3xA,T3xB,T3xC,T3xD,T3xE,T3xF,T3xG,T3xH,T3xI,T3xJ,T3xK,T3xL,T3xR,T3xS,T3xT,T3xU,T3xV,T3xW,

/* Initializing variables for sensors*/
String buffer = String("");
uint16_t rawA, rawB, visibleComp, irComp, ir, full;
float uviaCalc, uvibCalc, uvia, uvib, uvi, temperatures(64), avgTemp, hum, temp; 

/* Initializing the VEML sensor*/
// Calibration constants:
// Four gain calibration constants -- alpha, beta, gamma, delta -- can be used to correct the output in
// reference to a GOLDEN sample. The golden sample should be calibrated under a solar simulator.
// Setting these to 1.0 essentialy eliminates the "golden"-sample calibration
const float CALIBRATION_ALPHA_VIS = 1.0; // UVA / UVAgolden
const float CALIBRATION_BETA_VIS = 1.0;  // UVB / UVBgolden
const float CALIBRATION_GAMMA_IR = 1.0;  // UVcomp1 / UVcomp1golden
const float CALIBRATION_DELTA_IR = 1.0;  // UVcomp2 / UVcomp2golden

// Responsivity:
// Responsivity converts a raw 16-bit UVA/UVB reading to a relative irradiance (W/m^2).
// These values will need to be adjusted as either integration time or dynamic settings are modififed.
// These values are recommended by the "Designing the VEML6075 into an application" app note for 100ms IT
const float UVA_RESPONSIVITY = 0.00110; // UVAresponsivity
const float UVB_RESPONSIVITY = 0.00125; // UVBresponsivity

// UV coefficients:
// These values are recommended by the "Designing the VEML6075 into an application" app note
const float UVA_VIS_COEF_A = 2.22; // a
const float UVA_IR_COEF_B = 1.33;  // b
const float UVB_VIS_COEF_C = 2.95; // c
const float UVB_IR_COEF_D = 1.75;  // d

void configureTSL2591_1(void){
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  TSL1.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //TSL1.setGain(TSL2591_GAIN_MED);      // 25x gain
  //TSL1.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
   TSL1.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

}
void configureTSL2591_2(void){
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  TSL2.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //TSL2.setGain(TSL2591_GAIN_MED);      // 25x gain
  //TSL2.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
   TSL2.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // TSL2.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // TSL2.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // TSL2.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // TSL2.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // TSL2.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

}
void configureTSL2591_3(void){
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  TSL3.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //TSL3.setGain(TSL2591_GAIN_MED);      // 25x gain
  //TSL3.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
   TSL3.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // TSL3.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // TSL3.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // TSL3.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // TSL3.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // TSL3.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

}
void tcaselect(uint8_t i){
  if (i > 7) return; 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup(){  
  delay(500);
  Serial.begin(115200);
  Wire.begin();
  configureTSL2591_1();
  configureTSL2591_2();
  configureTSL2591_3();   
  DHT1.begin();
  BP1.begin();
  BP2.begin();
  BP3.begin();

  Wire.setClock(400000);
  
  tcaselect(0);
    if(!TSL1.begin()){
     Serial.println("Unable to communicate with TSL2591  1.... Check your wiring!");
     while(1);
    }
    if(!VEML1.begin()){
     Serial.println("Unable to communicate with VEML6075  1.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD1.begin()){
     Serial.println("Unable to communicate with AS7265x  1.... Check your wiring!");
     while(1);
    }
  
  MLX1.setRefreshRate(RATE_512HZ);
  MLX1.setResolution(RES_18bit);
  MLX1.setEmissivity(0.95);
  MLX1.initialize();  
  
  TRIAD1.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);
  TRIAD1.setIntegrationCycles(10); 
  TRIAD1.disableIndicator();

  //Rather than toggle the LEDs with each measurement, turn on LEDs all the time
  //TRIAD1.enableBulb(AS7265x_LED_WHITE);
  //TRIAD1.enableBulb(AS7265x_LED_IR);
  //TRIAD1.enableBulb(AS7265x_LED_UV);
  
  tcaselect(1);
    if(!TSL2.begin()){
     Serial.println("Unable to communicate with TSL2591  2.... Check your wiring!");
     while(1);
    }
    if(!VEML2.begin()){
     Serial.println("Unable to communicate with VEML6075  2.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD2.begin()){
     Serial.println("Unable to communicate with AS7265x  2.... Check your wiring!");
     while(1);
    }
  
  MLX2.setRefreshRate(RATE_512HZ);
  MLX2.setResolution(RES_18bit);
  MLX2.setEmissivity(0.95);
  MLX2.initialize(); 
 
  TRIAD2.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);
  TRIAD2.setIntegrationCycles(10); 
  TRIAD2.disableIndicator();

  //Rather than toggle the LEDs with each measurement, turn on LEDs all the time
  //TRIAD2.enableBulb(AS7265x_LED_WHITE);
  //TRIAD2.enableBulb(AS7265x_LED_IR);
  //TRIAD2.enableBulb(AS7265x_LED_UV);

    tcaselect(2);
    if(!TSL3.begin()){
     Serial.println("Unable to communicate with TSL2591  3.... Check your wiring!");
     while(1);
    }
    if(!VEML3.begin()){
     Serial.println("Unable to communicate with VEML6075  3.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD3.begin()){
     Serial.println("Unable to communicate with AS7265x  3.... Check your wiring!");
     while(1);
    }
  
  MLX3.setRefreshRate(RATE_512HZ);
  MLX3.setResolution(RES_18bit);
  MLX3.setEmissivity(0.95);
  MLX3.initialize(); 
 
  TRIAD3.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);
  TRIAD3.setIntegrationCycles(10); 
  TRIAD3.disableIndicator();

  //Rather than toggle the LEDs with each measurement, turn on LEDs all the time
  //TRIAD3.enableBulb(AS7265x_LED_WHITE);
  //TRIAD3.enableBulb(AS7265x_LED_IR);
  //TRIAD3.enableBulb(AS7265x_LED_UV);
  
  delay(1000);
  Serial.println("ms,Hum1,Temp1,BP1,Min1,Max1,IR1,Visible1,Full1,Lux1,UV1A,UV1B,T1x1,T1x2,T1x3,T1x4,T1x5,T1x6,T1x7,T1x8,T1x9,T1x10,T1x11,T1x12,T1x13,T1x14,T1x15,T1x16,T1x17,T1x18,BP2,Min2,Max2,IR2,Visible2,Full2,Lux2,UV2A,UV2B,T2x1,T2x2,T2x3,T2x4,T2x5,T2x6,T2x7,T2x8,T2x9,T2x10,T2x11,T2x12,T2x13,T2x14,T2x15,T2x16,T2x17,T2x18,BP3,in3,Max3,IR3,Visible3,Full3,Lux3,UV3A,UV3B,3xA,T3xB,T3xC,T3xD,T3xE,T3xF,T3xG,T3xH,T3xI,T3xJ,T3xK,T3xL,T3xR,T3xS,T3xT,T3xU,T3xV,T3xW,");
 }

void buffValues1(){ 
  int32_t lum = TSL1.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  rawA = VEML1.rawUva();
  rawB = VEML1.rawUvb();
  visibleComp = VEML1.visibleCompensation();
  irComp = VEML1.irCompensation();
  MLX1.setCallibration(0);
  MLX1.measure(true);

  // Calculate the simple UVIA and UVIB. These are used to calculate the UVI signal.
  uviaCalc = (float)rawA - ((UVA_VIS_COEF_A * CALIBRATION_ALPHA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVA_IR_COEF_B * CALIBRATION_ALPHA_VIS * irComp) / CALIBRATION_DELTA_IR);
  uvibCalc = (float)rawB - ((UVB_VIS_COEF_C * CALIBRATION_BETA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVB_IR_COEF_D * CALIBRATION_BETA_VIS * irComp) / CALIBRATION_DELTA_IR);

  // Convert raw UVIA and UVIB to values scaled by the sensor responsivity
  uvia = uviaCalc * (1.0 / CALIBRATION_ALPHA_VIS) * UVA_RESPONSIVITY;
  uvib = uvibCalc * (1.0 / CALIBRATION_BETA_VIS) * UVB_RESPONSIVITY;

  // Filling the buffer with data from sensors
  buffer += millis();                             buffer += ",";  
  buffer += hum;                                  buffer += ",";
  buffer += temp;                                 buffer += ",";
  buffer += BP1.getThmc();                        buffer += ","; 
  buffer += MLX1.minTemp;                         buffer += ",";
  buffer += MLX1.maxTemp;                         buffer += ",";
  buffer += ir;                                   buffer += ",";
  buffer += (full - ir);                          buffer += ",";
  buffer += full;                                 buffer += ",";
  buffer += TSL1.calculateLux(full, ir), 6;       buffer += ",";
  buffer += uviaCalc;                             buffer += ",";
  buffer += uvibCalc;                             buffer += ",";
  buffer += TRIAD1.getCalibratedA();              buffer += ",";
  buffer += TRIAD1.getCalibratedB();              buffer += ",";
  buffer += TRIAD1.getCalibratedC();              buffer += ",";
  buffer += TRIAD1.getCalibratedD();              buffer += ",";
  buffer += TRIAD1.getCalibratedE();              buffer += ",";
  buffer += TRIAD1.getCalibratedF();              buffer += ",";
  buffer += TRIAD1.getCalibratedG();              buffer += ",";
  buffer += TRIAD1.getCalibratedH();              buffer += ",";
  buffer += TRIAD1.getCalibratedI();              buffer += ",";
  buffer += TRIAD1.getCalibratedJ();              buffer += ",";
  buffer += TRIAD1.getCalibratedK();              buffer += ",";
  buffer += TRIAD1.getCalibratedL();              buffer += ",";
  buffer += TRIAD1.getCalibratedR();              buffer += ",";
  buffer += TRIAD1.getCalibratedS();              buffer += ",";
  buffer += TRIAD1.getCalibratedT();              buffer += ",";
  buffer += TRIAD1.getCalibratedU();              buffer += ",";
  buffer += TRIAD1.getCalibratedV();              buffer += ",";
  buffer += TRIAD1.getCalibratedW();              buffer += ",";
 }

void buffValues2(){ 
  int32_t lum = TSL2.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  rawA = VEML2.rawUva();
  rawB = VEML2.rawUvb();
  visibleComp = VEML2.visibleCompensation();
  irComp = VEML2.irCompensation();
  MLX2.setCallibration(0);
  MLX2.measure(true);

  // Calculate the simple UVIA and UVIB. These are used to calculate the UVI signal.
  uviaCalc = (float)rawA - ((UVA_VIS_COEF_A * CALIBRATION_ALPHA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVA_IR_COEF_B * CALIBRATION_ALPHA_VIS * irComp) / CALIBRATION_DELTA_IR);
  uvibCalc = (float)rawB - ((UVB_VIS_COEF_C * CALIBRATION_BETA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVB_IR_COEF_D * CALIBRATION_BETA_VIS * irComp) / CALIBRATION_DELTA_IR);

  // Convert raw UVIA and UVIB to values scaled by the sensor responsivity
  uvia = uviaCalc * (1.0 / CALIBRATION_ALPHA_VIS) * UVA_RESPONSIVITY;
  uvib = uvibCalc * (1.0 / CALIBRATION_BETA_VIS) * UVB_RESPONSIVITY;

  // Filling the buffer with data from sensors
  buffer += millis();                             buffer += ",";  
  buffer += hum;                                  buffer += ",";
  buffer += temp;                                 buffer += ",";
  buffer += BP2.getThmc();                        buffer += ","; 
  buffer += MLX2.minTemp;                         buffer += ",";
  buffer += MLX2.maxTemp;                         buffer += ",";
  buffer += ir;                                   buffer += ",";
  buffer += (full - ir);                          buffer += ",";
  buffer += full;                                 buffer += ",";
  buffer += TSL2.calculateLux(full, ir), 6;       buffer += ",";
  buffer += uviaCalc;                             buffer += ",";
  buffer += uvibCalc;                             buffer += ",";
  buffer += TRIAD2.getCalibratedA();              buffer += ",";
  buffer += TRIAD2.getCalibratedB();              buffer += ",";
  buffer += TRIAD2.getCalibratedC();              buffer += ",";
  buffer += TRIAD2.getCalibratedD();              buffer += ",";
  buffer += TRIAD2.getCalibratedE();              buffer += ",";
  buffer += TRIAD2.getCalibratedF();              buffer += ",";
  buffer += TRIAD2.getCalibratedG();              buffer += ",";
  buffer += TRIAD2.getCalibratedH();              buffer += ",";
  buffer += TRIAD2.getCalibratedI();              buffer += ",";
  buffer += TRIAD2.getCalibratedJ();              buffer += ",";
  buffer += TRIAD2.getCalibratedK();              buffer += ",";
  buffer += TRIAD2.getCalibratedL();              buffer += ",";
  buffer += TRIAD2.getCalibratedR();              buffer += ",";
  buffer += TRIAD2.getCalibratedS();              buffer += ",";
  buffer += TRIAD2.getCalibratedT();              buffer += ",";
  buffer += TRIAD2.getCalibratedU();              buffer += ",";
  buffer += TRIAD2.getCalibratedV();              buffer += ",";
  buffer += TRIAD2.getCalibratedW();              buffer += ",";
}

void buffValues3(){ 
  int32_t lum = TSL3.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  rawA = VEML3.rawUva();
  rawB = VEML3.rawUvb();
  visibleComp = VEML3.visibleCompensation();
  irComp = VEML3.irCompensation();
  MLX3.setCallibration(0);
  MLX3.measure(true);

  // Calculate the simple UVIA and UVIB. These are used to calculate the UVI signal.
  uviaCalc = (float)rawA - ((UVA_VIS_COEF_A * CALIBRATION_ALPHA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVA_IR_COEF_B * CALIBRATION_ALPHA_VIS * irComp) / CALIBRATION_DELTA_IR);
  uvibCalc = (float)rawB - ((UVB_VIS_COEF_C * CALIBRATION_BETA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVB_IR_COEF_D * CALIBRATION_BETA_VIS * irComp) / CALIBRATION_DELTA_IR);

  // Convert raw UVIA and UVIB to values scaled by the sensor responsivity
  uvia = uviaCalc * (1.0 / CALIBRATION_ALPHA_VIS) * UVA_RESPONSIVITY;
  uvib = uvibCalc * (1.0 / CALIBRATION_BETA_VIS) * UVB_RESPONSIVITY;

  // Filling the buffer with data from sensors
  buffer += millis();                             buffer += ",";  
  buffer += hum;                                  buffer += ",";
  buffer += temp;                                 buffer += ",";
  buffer += BP3.getThmc();                        buffer += ","; 
  buffer += MLX3.minTemp;                         buffer += ",";
  buffer += MLX3.maxTemp;                         buffer += ",";
  buffer += ir;                                   buffer += ",";
  buffer += (full - ir);                          buffer += ",";
  buffer += full;                                 buffer += ",";
  buffer += TSL3.calculateLux(full, ir), 6;       buffer += ",";
  buffer += uviaCalc;                             buffer += ",";
  buffer += uvibCalc;                             buffer += ",";
  buffer += TRIAD3.getCalibratedA();              buffer += ",";
  buffer += TRIAD3.getCalibratedB();              buffer += ",";
  buffer += TRIAD3.getCalibratedC();              buffer += ",";
  buffer += TRIAD3.getCalibratedD();              buffer += ",";
  buffer += TRIAD3.getCalibratedE();              buffer += ",";
  buffer += TRIAD3.getCalibratedF();              buffer += ",";
  buffer += TRIAD3.getCalibratedG();              buffer += ",";
  buffer += TRIAD3.getCalibratedH();              buffer += ",";
  buffer += TRIAD3.getCalibratedI();              buffer += ",";
  buffer += TRIAD3.getCalibratedJ();              buffer += ",";
  buffer += TRIAD3.getCalibratedK();              buffer += ",";
  buffer += TRIAD3.getCalibratedL();              buffer += ",";
  buffer += TRIAD3.getCalibratedR();              buffer += ",";
  buffer += TRIAD3.getCalibratedS();              buffer += ",";
  buffer += TRIAD3.getCalibratedT();              buffer += ",";
  buffer += TRIAD3.getCalibratedU();              buffer += ",";
  buffer += TRIAD3.getCalibratedV();              buffer += ",";
  buffer += TRIAD3.getCalibratedW();              buffer += ",";
  }
  
void loop(void){  
  hum = DHT1.readHumidity();
  temp= DHT1.readTemperature();
  
  tcaselect(0);
  buffValues1();
  if(buffer.length() > 2){
    Serial.println(buffer);
    buffer = String(""); 
  }

  tcaselect(1);
  buffValues2();
  if(buffer.length() > 2){
    Serial.println(buffer);
    buffer = String(""); 
  }
  
  tcaselect(2);
  buffValues3();
  if(buffer.length() > 2){
    Serial.println(buffer);
    buffer = String(""); 
  }
}
