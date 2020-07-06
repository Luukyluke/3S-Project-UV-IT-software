#include "Wire.h"                              //   ---
#include "Arduino.h"                           //  |
#include "Adafruit_TSL2591.h"                  //  |
#include "SparkFun_VEML6075_Arduino_Library.h" //  |
#include "SparkFun_AS7265X.h"                  //  |->Libraries for sensors
#include "MLX90621.h"                          //  |
#include "High_Temp.h"                         //  |
#include "DHT.h"                               //   ---
#define TCAADDR 0x70                           //Assigning the multiplexer with an adress
#define relay_0 48                             //Assign relay 0 to port 48

/* Assign a unique ID to the sensors */
Adafruit_TSL2591 TSL1; 
VEML6075 VEML1; 
AS7265X TRIAD1;
MLX90621 MLX1; 
HighTemp BP1(A1,A0);
DHT DHT1(22, DHT22);

/* TCA ports from the port tester*/
//TCA Port #0
//Found I2C 0x10                    -> VEML6075 
//Found I2C 0x29                    -> TSL2591 
//Found I2C 0x49                    -> Triad AS7265x
//Found I2C 0x50                    ---
//Found I2C 0x51                   |
//Found I2C 0x52                   |
//Found I2C 0x53                   |
//Found I2C 0x54                   |-> MLX90621 
//Found I2C 0x55                   |
//Found I2C 0x56                   |
//Found I2C 0x57                   |
//Found I2C 0x60                    ---
//TCA Port #1
//TCA Port #2
//TCA Port #3
//TCA Port #4
//TCA Port #5
//TCA Port #6
//TCA Port #7

/* Data output*/
//This is the format we use as data output

//Time:                         ms,
//DHT:                          Hum,Temp,
//Blackpanel1:                  BP1,
//MLX1:                         Max1,Avg1,
//TSL1:                         IR1,Visible1,Lux1,
//VEML1:                        UV1A,UV1B,UV1I,
//TRIAD1:                       T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,
//Light Exposure Equivalent:    LE1,
//Total Irradiance:             TI1,
 
//ms,Hum,Temp,BP1,Max1,Avg1,IR1,Visible1,Lux1,UV1A,UV1B,UV1I,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,LE1,TI1,

/* Initializing variables for sensors, if statements and calculation*/
  String buffer =  String("");
  uint16_t rawA, rawB, visibleComp, irComp, ir, full;
  float uviaCalc, uvibCalc, uvia, UV1Atot, uvib, UV1Btot, uvi, temperatures(64), avgTemp, hum, temp, IR1tot, Vis1tot, TotIrradiance1, LightExposure1, e_stop, A; 

/* Initializing the VEML sensor(s)*/
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

void configureTSL2591_1(void){     //Configuration for the TSL1 sensor
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    TSL1.setGain(TSL2591_GAIN_LOW);      // 1x gain (bright light)
  //TSL1.setGain(TSL2591_GAIN_MED);      // 25x gain
  //TSL1.setGain(TSL2591_GAIN_HIGH);     // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
     TSL1.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // TSL1.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
}

void configureTriadAS7265x_1(void){                                         //Configuration for the Triad1 sensor
  TRIAD1.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);     //Set all channels to measure contanious
  TRIAD1.setIntegrationCycles(10);                                          //Set intergration time (0-64) 10 is used for this module as standard
  TRIAD1.disableIndicator();                                                //Enable/Disable LED indicators

  //Turn on LEDs all the time, Comment/Uncomment which one to use or not
  //TRIAD1.enableBulb(AS7265x_LED_WHITE);                                   //Turn on white LED
  //TRIAD1.enableBulb(AS7265x_LED_IR);                                      //Turn on infrared LED
  //TRIAD1.enableBulb(AS7265x_LED_UV);                                      //Turn on ultra violet LED
}

void configureMLX90621_1(void){         //Configuration for the MLX1 sensor
  MLX1.setRefreshRate(RATE_512HZ);      //Set the refreshrate 0,1,2,4,8,16,32,64,128,256,512HZ is used as standard
  MLX1.setResolution(RES_18bit);        //Set the resolution in bits, 18  bits is used as standard
  MLX1.setEmissivity(0.95);             //Set emissivity, 0.95 is used as standard
  MLX1.initialize();                    //Initialize the MLX sensor
}

void tcaselect(uint8_t i){              //Function to set-up connection with the multiplexer over I2C
  if (i > 7) return; 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup(){                   //Setup loop of the program, runs only 1 time
  delay(500);
  Serial.begin(115200);         //Serial baudrate speed
  A = 0;                        //Float for buzzer if statement
  Wire.begin();                 //Begin I2C connection
  Wire.setClock(400000);        //Set clockspeed of I2C connection, this is a standard clockspeed.
  DHT1.begin();                 //Begin DHT sensor measurement
  BP1.begin();                  //Begin Blackpane1 sensor measurement
  pinMode(2, OUTPUT);           //The buzzer must be declared as an output on port 2
  pinMode(33, INPUT);           //The emergency stop must be declared as an input on port 33
  pinMode(relay_0, OUTPUT);     //The relay must be declared as an output
  digitalWrite(relay_0, HIGH);  //Turn on the lamp
  
  
  tcaselect(0);              //Select port 0 of multiplexer
    if(!TSL1.begin()){                                                                    //Check if the TSL1 sensor 1 is connected, if not give error
     Serial.println("Unable to communicate with TSL2591.... Check your wiring!");
     while(1);
    }
    if(!VEML1.begin()){                                                                   //Check if the VEML1 sensor 1 is connected, if not give error
     Serial.println("Unable to communicate with VEML6075.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD1.begin()){                                                                  //Check if the TRIAD1 sensor 1 is connected, if not give error
     Serial.println("Unable to communicate with AS7265x.... Check your wiring!");
     while(1);
    }  
  configureTSL2591_1();        //Function to configure TLS1 sensor 
  configureMLX90621_1();       //Function to configure MLX1 sensor
  configureTriadAS7265x_1();   //Function to configure Triad1 sensor
      
  delay(1000);
  Serial.println("ms,Hum,Temp,BP1,Max1,Avg1,IR1,Visible1,Lux1,UV1A,UV1B,UV1I,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,LE1,TI1,"); //At the start of the test this will be printed 
 }

void CalcVEML(void){    //Function to calculate the VEML measured values
  // Calculate the simple UVIA and UVIB. These are used to calculate the UVI signal.
  uviaCalc = (float)rawA - ((UVA_VIS_COEF_A * CALIBRATION_ALPHA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVA_IR_COEF_B * CALIBRATION_ALPHA_VIS * irComp) / CALIBRATION_DELTA_IR);
  uvibCalc = (float)rawB - ((UVB_VIS_COEF_C * CALIBRATION_BETA_VIS * visibleComp) / CALIBRATION_GAMMA_IR) - ((UVB_IR_COEF_D * CALIBRATION_BETA_VIS * irComp) / CALIBRATION_DELTA_IR);

  // Convert raw UVIA and UVIB to values scaled by the sensor responsivity
  uvia = uviaCalc * (1.0 / CALIBRATION_ALPHA_VIS) * UVA_RESPONSIVITY;
  uvib = uvibCalc * (1.0 / CALIBRATION_BETA_VIS) * UVB_RESPONSIVITY;

  // Use UVIA and UVIB to calculate the average UVI:
  uvi = (uvia + uvib) / 2.0;
}

void EmptyCalc(void){       //Function to clear the variables of data used for other modules
  uviaCalc = 0;
  uvibCalc = 0;
  uvi = 0;
  ir = 0;
  full = 0; 
}

void CalcTotal1(void){                                                //Function for the calculations
  UV1Atot = (uviaCalc/93) + UV1Atot;                                  //Calculate total UVA total of the VEML 1 sensor
  UV1Btot = (uvibCalc/210) + UV1Btot;                                 //Calculate total UVB total of the VEML 1 sensor
  IR1tot = (ir/100) + IR1tot;                                         //Calculate total Infrared total of the TSL 1 sensor
  Vis1tot = ((full - ir)/3500) +  Vis1tot;                            //Calculate total Visible light total of the TSL 1 sensor
  TotIrradiance1 = UV1Atot + UV1Btot + IR1tot + Vis1tot;              //Calculate total Irradiance for module 1
  LightExposure1 = (TotIrradiance1 * (millis()/1000))/1000;           //Calculate the Light Exposure Equivalent for module 1
}

void buffValues1(void){ 
  int32_t lum = TSL1.getFullLuminosity();           //Set luminosity from TSL 1
  ir = lum >> 16;                                   //Set infrared from luminosity
  full = lum & 0xFFFF;                              //Set full from luminosity
  rawA = VEML1.rawUva();                            //Set rawA from VEML 1
  rawB = VEML1.rawUvb();                            //Set rawB from VEML 1
  visibleComp = VEML1.visibleCompensation();        //Set visible component from VEML 1
  irComp = VEML1.irCompensation();                  //Set infrared component from VEML 1
  MLX1.setCallibration(0);                          //Set calibration to 0 for MLX 1
  MLX1.measure(true);                               //Set measuro = true for MLX 1
  CalcVEML();                                       //Calculate function for VEML
  CalcTotal1();                                     //Calculate function for total values and light exposure equivalent

  // Filling the buffer with (calculated) data from sensors
  buffer += millis();                             buffer += ",";              // Time in milleseconds
  buffer += hum;                                  buffer += ",";              // Humidity from DHT sensor
  buffer += temp;                                 buffer += ",";              // Temperature from DHT sensor
  buffer += BP1.getThmc();                        buffer += ",";              // Temperature from blackpanel sensor
  buffer += MLX1.maxTemp;                         buffer += ",";              // Maximum temperature from MLX90621 sensors
  buffer += MLX1.avgTemp;                         buffer += ",";              // Average temperature from MLX90621 sensors
  buffer += ir;                                   buffer += ",";              // Infrared from TSL sensor
  buffer += (full-ir);                            buffer += ",";              // Visible from TSL sensor
  buffer += TSL1.calculateLux(full, ir), 6;       buffer += ",";              // Lux from TSL sensor
  buffer += (uviaCalc/93);                        buffer += ",";              // UVA from VEML sensor
  buffer += (uvibCalc/210);                       buffer += ",";              // UVB from VEML sensor
  buffer +=  uvi;                                 buffer += ",";              // UV Index from VEML sensor
  buffer += (TRIAD1.getCalibratedA()/3500);       buffer += ",";              // Channel A from TRIAD sensor (410 nm)  ---
  buffer += (TRIAD1.getCalibratedB()/3500);       buffer += ",";              // Channel B from TRIAD sensor (435 nm) |
  buffer += (TRIAD1.getCalibratedC()/3500);       buffer += ",";              // Channel C from TRIAD sensor (460 nm) |
  buffer += (TRIAD1.getCalibratedD()/3500);       buffer += ",";              // Channel D from TRIAD sensor (485 nm) |  AS7265 3 (410nm/535nm)
  buffer += (TRIAD1.getCalibratedE()/3500);       buffer += ",";              // Channel E from TRIAD sensor (510 nm) |
  buffer += (TRIAD1.getCalibratedF()/3500);       buffer += ",";              // Channel F from TRIAD sensor (535 nm)  ---
  buffer += (TRIAD1.getCalibratedG()/3500);       buffer += ",";              // Channel G from TRIAD sensor (560 nm)  ---
  buffer += (TRIAD1.getCalibratedH()/3500);       buffer += ",";              // Channel H from TRIAD sensor (585 nm) |
  buffer += (TRIAD1.getCalibratedI()/3500);       buffer += ",";              // Channel I from TRIAD sensor (645 nm) |
  buffer += (TRIAD1.getCalibratedJ()/3500);       buffer += ",";              // Channel J from TRIAD sensor (705 nm) |  AS7265 2 (560nm/940nm)
  buffer += (TRIAD1.getCalibratedK()/3500);       buffer += ",";              // Channel K from TRIAD sensor (900 nm) | 
  buffer += (TRIAD1.getCalibratedL()/3500);       buffer += ",";              // Channel L from TRIAD sensor (940 nm)  ---
  buffer += (TRIAD1.getCalibratedR()/3500);       buffer += ",";              // Channel R from TRIAD sensor (610 nm)  ---
  buffer += (TRIAD1.getCalibratedS()/3500);       buffer += ",";              // Channel S from TRIAD sensor (680 nm) |
  buffer += (TRIAD1.getCalibratedT()/3500);       buffer += ",";              // Channel T from TRIAD sensor (730 nm) |
  buffer += (TRIAD1.getCalibratedU()/3500);       buffer += ",";              // Channel U from TRIAD sensor (760 nm) |  AS7265 1 (610nm/860nm)
  buffer += (TRIAD1.getCalibratedV()/3500);       buffer += ",";              // Channel V from TRIAD sensor (810 nm) |
  buffer += (TRIAD1.getCalibratedW()/3500);       buffer += ",";              // Channel W from TRIAD sensor (860 nm)  ---
  buffer += LightExposure1;                       buffer += ",";              // Light Exposure Equivalent
  buffer += TotIrradiance1;                       buffer += ",";              // Light Exposure Equivalent  
  EmptyCalc();                                                                // Function to empty values used for calculations
}

void loop(void){                        //Main loop, runs continious until time cap is reached
  hum = DHT1.readHumidity();            //Read humidity from DHT sensor
  temp= DHT1.readTemperature();         //Read temperature from DHT sensor
  e_stop = digitalRead(33);             //Read the emergency stop input from pin 33

  if(e_stop == 0){                      //When the emergency stop is pressed stop script and turn off the lamp
    digitalWrite(relay_0, LOW);         //Turn off lamp
    if (A ==0){                         //If statement for the buzzer so that it only is on for 3 seconds
       analogWrite(2, 127);             //Turn on the buzzer
       delay(3000);                     //Wait 3 seconds with buzzer on
       analogWrite(2, 0);               //Turn off the buzzer
       A = 1;                           //Make A 1 so that the buzzer won't turn on again
    }
  }
  
  if(e_stop == 1){                      //When the emergency stop is released turn on the lamp
    digitalWrite(relay_0, HIGH);        //Turn on the lamp when emergency butten is lose
    A = 0;                              //Make A 0 so that the buzzer will turn on again once emergency stop is pressed again
  } 
  
  tcaselect(0);                         //Select port 0 on multiplexer
  buffValues1();                        //Calculate and send measured values

  if(buffer.length() > 2){              //If content of string buffer is greater than 2, print buffer on serial monitor and then empty the string
    Serial.println(buffer);             //Print the buffer
    buffer = String("");                //Empty the string 
  } 

  if (millis() > 28800000){             //Timer for test duration (8 hours = 28800000, 40 hours = 144000000, 60 hours = 216000000)
    digitalWrite(relay_0, LOW);         //Turn off lamp
    exit(0);                            //Exit program
  }
}
