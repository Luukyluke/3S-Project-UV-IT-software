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
Adafruit_TSL2591 TSL1; 
VEML6075 VEML1; 
AS7265X TRIAD1;
MLX90621 MLX1; 
HighTemp BP1(A1,A0);
DHT DHT1(7, DHT22);

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
//TCA Port #2
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

//ms,Hum1,Temp1,BP1,Min1,Max1,IR1,Visible1,Full1,Lux1,UV1A,UV1B,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,

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

void configureTSL2591(void){
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

void tcaselect(uint8_t i){
  if (i > 7) return; 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup(){  
  delay(500);
  Serial.begin(115200);
  configureTSL2591();
  Wire.begin();
  DHT1.begin();
  BP1.begin();
  
  tcaselect(0);
    if(!TSL1.begin()){
     Serial.println("Unable to communicate with TSL2591.... Check your wiring!");
     while(1);
    }
    if(!VEML1.begin()){
     Serial.println("Unable to communicate with VEML6075.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD1.begin()){
     Serial.println("Unable to communicate with AS7265x.... Check your wiring!");
     while(1);
    }
  
  MLX1.setRefreshRate(RATE_512HZ);
  MLX1.setResolution(RES_18bit);
  MLX1.setEmissivity(0.95);
  MLX1.initialize(); 
  
  Wire.setClock(400000);
  TRIAD1.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);
  TRIAD1.setIntegrationCycles(10); 
  TRIAD1.disableIndicator();

  //Rather than toggle the LEDs with each measurement, turn on LEDs all the time
  //TRIAD1.enableBulb(AS7265x_LED_WHITE);
  //TRIAD1.enableBulb(AS7265x_LED_IR);
  //TRIAD1.enableBulb(AS7265x_LED_UV);
  delay(1000);
  Serial.println("ms,Hum1,Temp1,BP1,Min1,Max1,IR1,Visible1,Full1,Lux1,UV1A,UV1B,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,");
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

void loop(void){  
  hum = DHT1.readHumidity();
  temp= DHT1.readTemperature();
  
  tcaselect(0);
  buffValues1();

  if(buffer.length() > 2){
    Serial.println(buffer);
    buffer = String(""); 
  }  
}
