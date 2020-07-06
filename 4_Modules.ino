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
#define relay_1 49                             //Assign relay 1 to port 49
#define relay_2 46                             //Assign relay 2 to port 46
#define relay_3 47                             //Assign relay 3 to port 47

/* Assign a unique ID to the sensors */
DHT DHT1(7, DHT22);
Adafruit_TSL2591 TSL1; 
Adafruit_TSL2591 TSL2;
Adafruit_TSL2591 TSL3;
Adafruit_TSL2591 TSL4;
VEML6075 VEML1;
VEML6075 VEML2; 
VEML6075 VEML3;
VEML6075 VEML4;
AS7265X TRIAD1;
AS7265X TRIAD2;
AS7265X TRIAD3;
AS7265X TRIAD4;
MLX90621 MLX1; 
MLX90621 MLX2; 
MLX90621 MLX3;
MLX90621 MLX4;
HighTemp BP1(A1,A0);
HighTemp BP2(A3,A2);
HighTemp BP3(A5,A4);
HighTemp BP4(A7,A6);

/* TCA ports that you should get from the port tester*/
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
//TCA Port #2
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
//TCA Port #3
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
//TCA Port #4
//TCA Port #5
//TCA Port #6
//TCA Port #7

/* Data output*/
//This is the format used as data output

//Time:                         ms,
//DHT:                          Hum,Temp,
//Blackpanel1:                  BP1,
//MLX1:                         Max1,Avg1,
//TSL1:                         IR1,Visible1,Lux1,
//VEML1:                        UV1A,UV1B,UV1I,
//TRIAD1:                       T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,
//Light Exposure Equivalent:    LE1,
//Total Irradiance:             TI1,
//Blackpanel2:                  BP2,
//MLX2:                         Max2,Avg2,
//TSL2:                         IR2,Visible2,Lux2,
//VEML2:                        UV2A,UV2B,UV2I,
//TRIAD2:                       T2xA,T2xB,T2xC,T2xD,T2xE,T2xF,T2xG,T2xH,T2xI,T2xJ,T2xK,T2xL,T2xR,T2xS,T2xT,T2xU,T2xV,T2xW,
//Light Exposure Equivalent:    LE2,
//Total Irradiance:             TI2,
//Blackpanel3:                  BP3,
//MLX3:                         Max3,Avg3,
//TSL3:                         IR3,Visible3,Lux3,
//VEML3:                        UV3A,UV3B,UV3I,
//TRIAD3:                       T3xA,T3xB,T3xC,T3xD,T3xE,T3xF,T3xG,T3xH,T3xI,T3xJ,T3xK,T3xL,T3xR,T3xS,T3xT,T3xU,T3xV,T3xW,
//Light Exposure Equivalent:    LE3,
//Total Irradiance:             TI3,
//Blackpanel4:                  BP4,
//MLX4:                         Max4,Avg4,
//TSL4:                         IR4,Visible4,Lux4,
//VEML4:                        UV4A,UV4B,UV4I,
//TRIAD4:                       T4xA,T4xB,T4xC,T4xD,T4xE,T4xF,T4xG,T4xH,T4xI,T4xJ,T4xK,T4xL,T4xR,T4xS,T4xT,T4xU,T4xV,T4xW,
//Light Exposure Equivalent:    LE4,
//Total Irradiance:             TI4,

//ms,Hum,Temp,BP1,Max1,Avg1,IR1,Visible1,Lux1,UV1A,UV1B,UV1I,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,LE1,TI1,BP2,Max2,Avg2,IR2,Visible2,Lux2,UV2A,UV2B,UV2I,T2xA,T2xB,T2xC,T2xD,T2xE,T2xF,T2xG,T2xH,T2xI,T2xJ,T2xK,T2xL,T2xR,T2xS,T2xT,T2xU,T2xV,T2xW,LE2,TI2,BP3,Max3,Avg3,IR3,Visible3,Lux3,UV3A,UV3B,UV3I,T3xA,T3xB,T3xC,T3xD,T3xE,T3xF,T3xG,T3xH,T3xI,T3xJ,T3xK,T3xL,T3xR,T3xS,T3xT,T3xU,T3xV,T3xW,LE3,TI3,BP4,Max4,Avg4,IR4,Visible4,Lux4,UV4A,UV4B,UV4I,T4xA,T4xB,T4xC,T4xD,T4xE,T4xF,T4xG,T4xH,T4xI,T4xJ,T4xK,T4xL,T4xR,T4xS,T4xT,T4xU,T4xV,T4xW,LE4,TI4,

/* Initializing variables for sensors*/
 String buffer =  String("");
 uint16_t rawA, rawB, visibleComp, irComp, ir, full;
 float uviaCalc, uvibCalc, uvia, UV1Atot, uvib, UV1Btot, uvi, temperatures(64), avgTemp, hum, temp, IR1tot, Vis1tot, TotIrradiance1, LightExposure1, UV2Atot, UV2Btot,IR2tot, Vis2tot, TotIrradiance2, LightExposure2, UV3Atot, UV3Btot, IR3tot, Vis3tot, TotIrradiance3, LightExposure3, UV4Atot, UV4Btot,IR4tot, Vis4tot, TotIrradiance4, LightExposure4, e_stop, A; 


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

void configureTSL2591_4(void){
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    TSL4.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //TSL4.setGain(TSL2591_GAIN_MED);      // 25x gain
  //TSL4.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
     TSL4.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // TSL4.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // TSL4.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // TSL4.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // TSL4.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // TSL4.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
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

void configureTriadAS7265x_2(void){                                         //Configuration for the Triad1 sensor
  TRIAD2.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);     //Set all channels to measure contanious
  TRIAD2.setIntegrationCycles(10);                                          //Set intergration time (0-64) 10 is used for this module as standard
  TRIAD2.disableIndicator();                                                //Enable/Disable LED indicators

  //Turn on LEDs all the time, Comment/Uncomment which one to use or not
  //TRIAD2.enableBulb(AS7265x_LED_WHITE);                                   //Turn on white LED
  //TRIAD2.enableBulb(AS7265x_LED_IR);                                      //Turn on infrared LED
  //TRIAD2.enableBulb(AS7265x_LED_UV);                                      //Turn on ultra violet LED
}

void configureTriadAS7265x_3(void){                                         //Configuration for the Triad1 sensor
  TRIAD3.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);     //Set all channels to measure contanious
  TRIAD3.setIntegrationCycles(10);                                          //Set intergration time (0-64) 10 is used for this module as standard
  TRIAD3.disableIndicator();                                                //Enable/Disable LED indicators

  //Turn on LEDs all the time, Comment/Uncomment which one to use or not
  //TRIAD3.enableBulb(AS7265x_LED_WHITE);                                   //Turn on white LED
  //TRIAD3.enableBulb(AS7265x_LED_IR);                                      //Turn on infrared LED
  //TRIAD3.enableBulb(AS7265x_LED_UV);                                      //Turn on ultra violet LED
}

void configureTriadAS7265x_4(void){                                         //Configuration for the Triad1 sensor
  TRIAD4.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);     //Set all channels to measure contanious
  TRIAD4.setIntegrationCycles(10);                                          //Set intergration time (0-64) 10 is used for this module as standard
  TRIAD4.disableIndicator();                                                //Enable/Disable LED indicators

  //Turn on LEDs all the time, Comment/Uncomment which one to use or not
  //TRIAD4.enableBulb(AS7265x_LED_WHITE);                                   //Turn on white LED
  //TRIAD4.enableBulb(AS7265x_LED_IR);                                      //Turn on infrared LED
  //TRIAD4.enableBulb(AS7265x_LED_UV);                                      //Turn on ultra violet LED
}

void configureMLX90621_1(void){         //Configuration for the MLX1 sensor
  MLX1.setRefreshRate(RATE_512HZ);      //Set the refreshrate 0,1,2,4,8,16,32,64,128,256,512HZ is used as standard
  MLX1.setResolution(RES_18bit);        //Set the resolution in bits, 18  bits is used as standard
  MLX1.setEmissivity(0.95);             //Set emissivity, 0.95 is used as standard
  MLX1.initialize();                    //Initialize the MLX sensor
}

void configureMLX90621_2(void){         //Configuration for the MLX1 sensor
  MLX2.setRefreshRate(RATE_512HZ);      //Set the refreshrate 0,1,2,4,8,16,32,64,128,256,512HZ is used as standard
  MLX2.setResolution(RES_18bit);        //Set the resolution in bits, 18  bits is used as standard
  MLX2.setEmissivity(0.95);             //Set emissivity, 0.95 is used as standard
  MLX2.initialize();                    //Initialize the MLX sensor
}

void configureMLX90621_3(void){         //Configuration for the MLX1 sensor
  MLX3.setRefreshRate(RATE_512HZ);      //Set the refreshrate 0,1,2,4,8,16,32,64,128,256,512HZ is used as standard
  MLX3.setResolution(RES_18bit);        //Set the resolution in bits, 18  bits is used as standard
  MLX3.setEmissivity(0.95);             //Set emissivity, 0.95 is used as standard
  MLX3.initialize();                    //Initialize the MLX sensor
}

void configureMLX90621_4(void){         //Configuration for the MLX1 sensor
  MLX4.setRefreshRate(RATE_512HZ);      //Set the refreshrate 0,1,2,4,8,16,32,64,128,256,512HZ is used as standard
  MLX4.setResolution(RES_18bit);        //Set the resolution in bits, 18  bits is used as standard
  MLX4.setEmissivity(0.95);             //Set emissivity, 0.95 is used as standard
  MLX4.initialize();                    //Initialize the MLX sensor
}

void tcaselect(uint8_t i){
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
  BP1.begin();                  //Begin Blackpanel sensor measurement
  BP2.begin();                  //Begin Blackpane1 sensor measurement
  BP3.begin();                  //Begin Blackpane1 sensor measurement
  BP4.begin();                  //Begin Blackpane1 sensor measurement
  pinMode(2, OUTPUT);           //The buzzer must be declared as an output on port 2
  pinMode(33, INPUT);           //The emergency stop must be declared as an input on port 33
  pinMode(relay_0, OUTPUT);     //The relay must be declared as an output
  digitalWrite(relay_0, HIGH);  //Turn on the lamp
  pinMode(relay_1, OUTPUT);     //The relay must be declared as an output
  digitalWrite(relay_1, HIGH);  //Turn on the lamp
  pinMode(relay_2, OUTPUT);     //The relay must be declared as an output
  digitalWrite(relay_2, HIGH);  //Turn on the lamp 
  pinMode(relay_3, OUTPUT);     //The relay must be declared as an output
  digitalWrite(relay_3, HIGH);  //Turn on the lamp 
  
  tcaselect(0);                 //Select port 0 of multiplexer
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

    tcaselect(1);                //Select port 1 of multiplexer
    if(!TSL2.begin()){                                                                    //Check if the TSL1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with TSL2591.... Check your wiring!");
     while(1);
    }
    if(!VEML2.begin()){                                                                   //Check if the VEML1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with VEML6075.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD2.begin()){                                                                  //Check if the TRIAD1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with AS7265x.... Check your wiring!");
     while(1);
    }  
    configureTSL2591_2();        //Function to configure TLS1 sensor 
    configureMLX90621_2();       //Function to configure MLX1 sensor
    configureTriadAS7265x_2();   //Function to configure Triad1 sensor

    tcaselect(2);                //Select port 2 of multiplexer
    if(!TSL3.begin()){                                                                    //Check if the TSL1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with TSL2591.... Check your wiring!");
     while(1);
    }
    if(!VEML3.begin()){                                                                   //Check if the VEML1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with VEML6075.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD3.begin()){                                                                  //Check if the TRIAD1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with AS7265x.... Check your wiring!");
     while(1);
    }  
    configureTSL2591_3();        //Function to configure TLS1 sensor 
    configureMLX90621_3();       //Function to configure MLX1 sensor
    configureTriadAS7265x_3();   //Function to configure Triad1 sensor
    
    tcaselect(3);                //Select port 2 of multiplexer
    if(!TSL4.begin()){                                                                    //Check if the TSL1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with TSL2591.... Check your wiring!");
     while(1);
    }
    if(!VEML4.begin()){                                                                   //Check if the VEML1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with VEML6075.... Check your wiring!");
     while(1);
    }  
    if(!TRIAD4.begin()){                                                                  //Check if the TRIAD1 sensor 2 is connected, if not give error
     Serial.println("Unable to communicate with AS7265x.... Check your wiring!");
     while(1);
    }  
    configureTSL2591_4();        //Function to configure TLS1 sensor 
    configureMLX90621_4();       //Function to configure MLX1 sensor
    configureTriadAS7265x_4();   //Function to configure Triad1 sensor
      
  delay(1000);
  Serial.println("ms,Hum,Temp,BP1,Max1,Avg1,IR1,Visible1,Lux1,UV1A,UV1B,UV1I,T1xA,T1xB,T1xC,T1xD,T1xE,T1xF,T1xG,T1xH,T1xI,T1xJ,T1xK,T1xL,T1xR,T1xS,T1xT,T1xU,T1xV,T1xW,LE1,TI1,BP2,Max2,Avg2,IR2,Visible2,Lux2,UV2A,UV2B,UV2I,T2xA,T2xB,T2xC,T2xD,T2xE,T2xF,T2xG,T2xH,T2xI,T2xJ,T2xK,T2xL,T2xR,T2xS,T2xT,T2xU,T2xV,T2xW,LE2,TI2,BP3,Max3,Avg3,IR3,Visible3,Lux3,UV3A,UV3B,UV3I,T3xA,T3xB,T3xC,T3xD,T3xE,T3xF,T3xG,T3xH,T3xI,T3xJ,T3xK,T3xL,T3xR,T3xS,T3xT,T3xU,T3xV,T3xW,LE3,TI3,BP4,Max4,Avg4,IR4,Visible4,Lux4,UV4A,UV4B,UV4I,T4xA,T4xB,T4xC,T4xD,T4xE,T4xF,T4xG,T4xH,T4xI,T4xJ,T4xK,T4xL,T4xR,T4xS,T4xT,T4xU,T4xV,T4xW,LE4,TI4,"); //At the start of the test this will be printed 
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

void CalcTotal_1(void){                                               //Function for the calculations
  UV1Atot = (uviaCalc/93) + UV1Atot;                                  //Calculate total UVA total of the VEML 1 sensor
  UV1Btot = (uvibCalc/210) + UV1Btot;                                 //Calculate total UVB total of the VEML 1 sensor
  IR1tot = (ir/100) + IR1tot;                                         //Calculate total Infrared total of the TSL 1 sensor
  Vis1tot = ((full - ir)/3500) +  Vis1tot;                            //Calculate total Visible light total of the TSL 1 sensor
  TotIrradiance1 = UV1Atot + UV1Btot + IR1tot + Vis1tot;              //Calculate total Irradiance for module 1
  LightExposure1 = (TotIrradiance1 * (millis()/1000))/1000;           //Calculate the Light Exposure Equivalent for module 1
}

void CalcTotal_2(void){                                               //Function for the calculations
  UV2Atot = (uviaCalc/93) + UV2Atot;                                  //Calculate total UVA total of the VEML 2 sensor
  UV2Btot = (uvibCalc/210) + UV2Btot;                                 //Calculate total UVB total of the VEML 2 sensor
  IR2tot = (ir/100) + IR2tot;                                         //Calculate total Infrared total of the TSL 2 sensor
  Vis2tot = ((full - ir)/3500) +  Vis2tot;                            //Calculate total Visible light total of the TSL 2 sensor
  TotIrradiance2 = UV2Atot + UV2Btot + IR2tot + Vis2tot;              //Calculate total Irradiance for module 2
  LightExposure2 = (TotIrradiance2 * (millis()/1000))/1000;           //Calculate the Light Exposure Equivalent for module 2
}

void CalcTotal_3(void){                                               //Function for the calculations
  UV3Atot = (uviaCalc/93) + UV3Atot;                                  //Calculate total UVA total of the VEML 3 sensor
  UV3Btot = (uvibCalc/210) + UV3Btot;                                 //Calculate total UVB total of the VEML 3 sensor
  IR3tot = (ir/100) + IR3tot;                                         //Calculate total Infrared total of the TSL 3 sensor
  Vis3tot = ((full - ir)/3500) +  Vis3tot;                            //Calculate total Visible light total of the TSL 3 sensor
  TotIrradiance3 = UV3Atot + UV3Btot + IR3tot + Vis3tot;              //Calculate total Irradiance for module 3
  LightExposure3 = (TotIrradiance3 * (millis()/1000))/1000;           //Calculate the Light Exposure Equivalent for module 3
}

void CalcTotal_4(void){                                               //Function for the calculations
  UV4Atot = (uviaCalc/93) + UV4Atot;                                  //Calculate total UVA total of the VEML 4 sensor
  UV4Btot = (uvibCalc/210) + UV4Btot;                                 //Calculate total UVB total of the VEML 4 sensor
  IR4tot = (ir/100) + IR4tot;                                         //Calculate total Infrared total of the TSL 4 sensor
  Vis4tot = ((full - ir)/3500) +  Vis4tot;                            //Calculate total Visible light total of the TSL 4 sensor
  TotIrradiance4 = UV4Atot + UV4Btot + IR4tot + Vis4tot;              //Calculate total Irradiance for module 4
  LightExposure4 = (TotIrradiance4 * (millis()/1000))/1000;           //Calculate the Light Exposure Equivalent for module 4
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
  CalcTotal_1();                                     //Calculate function for total values and light exposure equivalent

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

void buffValues2(void){ 
  int32_t lum = TSL2.getFullLuminosity();           //Set luminosity from TSL 2
  ir = lum >> 16;                                   //Set infrared from luminosity
  full = lum & 0xFFFF;                              //Set full from luminosity
  rawA = VEML2.rawUva();                            //Set rawA from VEML 2
  rawB = VEML2.rawUvb();                            //Set rawB from VEML 2
  visibleComp = VEML2.visibleCompensation();        //Set visible component from VEML 2
  irComp = VEML2.irCompensation();                  //Set infrared component from VEML 2
  MLX2.setCallibration(0);                          //Set calibration to 0 for MLX 2
  MLX2.measure(true);                               //Set measuro = true for MLX 2
  CalcVEML();                                       //Calculate function for VEML
  CalcTotal_2();                                    //Calculate function for total values and light exposure equivalent

  // Filling the buffer with (calculated) data from sensors
  buffer += BP2.getThmc();                        buffer += ",";              // Temperature from blackpanel sensor
  buffer += MLX2.maxTemp;                         buffer += ",";              // Maximum temperature from MLX90621 sensors
  buffer += MLX2.avgTemp;                         buffer += ",";              // Average temperature from MLX90621 sensors
  buffer += ir;                                   buffer += ",";              // Infrared from TSL sensor
  buffer += (full-ir);                            buffer += ",";              // Visible from TSL sensor
  buffer += TSL2.calculateLux(full, ir), 6;       buffer += ",";              // Lux from TSL sensor
  buffer += (uviaCalc/93);                        buffer += ",";              // UVA from VEML sensor
  buffer += (uvibCalc/210);                       buffer += ",";              // UVB from VEML sensor
  buffer +=  uvi;                                 buffer += ",";              // UV Index from VEML sensor
  buffer += (TRIAD2.getCalibratedA()/3500);       buffer += ",";              // Channel A from TRIAD sensor (410 nm)  ---
  buffer += (TRIAD2.getCalibratedB()/3500);       buffer += ",";              // Channel B from TRIAD sensor (435 nm) |
  buffer += (TRIAD2.getCalibratedC()/3500);       buffer += ",";              // Channel C from TRIAD sensor (460 nm) |
  buffer += (TRIAD2.getCalibratedD()/3500);       buffer += ",";              // Channel D from TRIAD sensor (485 nm) |  AS7265 3 (410nm/535nm)
  buffer += (TRIAD2.getCalibratedE()/3500);       buffer += ",";              // Channel E from TRIAD sensor (510 nm) |
  buffer += (TRIAD2.getCalibratedF()/3500);       buffer += ",";              // Channel F from TRIAD sensor (535 nm)  ---
  buffer += (TRIAD2.getCalibratedG()/3500);       buffer += ",";              // Channel G from TRIAD sensor (560 nm)  ---
  buffer += (TRIAD2.getCalibratedH()/3500);       buffer += ",";              // Channel H from TRIAD sensor (585 nm) |
  buffer += (TRIAD2.getCalibratedI()/3500);       buffer += ",";              // Channel I from TRIAD sensor (645 nm) |
  buffer += (TRIAD2.getCalibratedJ()/3500);       buffer += ",";              // Channel J from TRIAD sensor (705 nm) |  AS7265 2 (560nm/940nm)
  buffer += (TRIAD2.getCalibratedK()/3500);       buffer += ",";              // Channel K from TRIAD sensor (900 nm) | 
  buffer += (TRIAD2.getCalibratedL()/3500);       buffer += ",";              // Channel L from TRIAD sensor (940 nm)  ---
  buffer += (TRIAD2.getCalibratedR()/3500);       buffer += ",";              // Channel R from TRIAD sensor (610 nm)  ---
  buffer += (TRIAD2.getCalibratedS()/3500);       buffer += ",";              // Channel S from TRIAD sensor (680 nm) |
  buffer += (TRIAD2.getCalibratedT()/3500);       buffer += ",";              // Channel T from TRIAD sensor (730 nm) |
  buffer += (TRIAD2.getCalibratedU()/3500);       buffer += ",";              // Channel U from TRIAD sensor (760 nm) |  AS7265 1 (610nm/860nm)
  buffer += (TRIAD2.getCalibratedV()/3500);       buffer += ",";              // Channel V from TRIAD sensor (810 nm) |
  buffer += (TRIAD2.getCalibratedW()/3500);       buffer += ",";              // Channel W from TRIAD sensor (860 nm)  ---
  buffer += LightExposure2;                       buffer += ",";              // Light Exposure Equivalent
  buffer += TotIrradiance2;                       buffer += ",";              // Light Exposure Equivalent  
  EmptyCalc();                                                                // Function to empty values used for calculations
}

void buffValues3(void){ 
  int32_t lum = TSL3.getFullLuminosity();           //Set luminosity from TSL 3
  ir = lum >> 16;                                   //Set infrared from luminosity
  full = lum & 0xFFFF;                              //Set full from luminosity
  rawA = VEML3.rawUva();                            //Set rawA from VEML 3
  rawB = VEML3.rawUvb();                            //Set rawB from VEML 3
  visibleComp = VEML3.visibleCompensation();        //Set visible component from VEML 3
  irComp = VEML3.irCompensation();                  //Set infrared component from VEML 3
  MLX3.setCallibration(0);                          //Set calibration to 0 for MLX 3
  MLX3.measure(true);                               //Set measuro = true for MLX 3
  CalcVEML();                                       //Calculate function for VEML
  CalcTotal_3();                                    //Calculate function for total values and light exposure equivalent

  // Filling the buffer with (calculated) data from sensors
  buffer += BP3.getThmc();                        buffer += ",";              // Temperature from blackpanel sensor
  buffer += MLX3.maxTemp;                         buffer += ",";              // Maximum temperature from MLX90621 sensors
  buffer += MLX3.avgTemp;                         buffer += ",";              // Average temperature from MLX90621 sensors
  buffer += ir;                                   buffer += ",";              // Infrared from TSL sensor
  buffer += (full-ir);                            buffer += ",";              // Visible from TSL sensor
  buffer += TSL3.calculateLux(full, ir), 6;       buffer += ",";              // Lux from TSL sensor
  buffer += (uviaCalc/93);                        buffer += ",";              // UVA from VEML sensor
  buffer += (uvibCalc/210);                       buffer += ",";              // UVB from VEML sensor
  buffer +=  uvi;                                 buffer += ",";              // UV Index from VEML sensor
  buffer += (TRIAD3.getCalibratedA()/3500);       buffer += ",";              // Channel A from TRIAD sensor (410 nm)  ---
  buffer += (TRIAD3.getCalibratedB()/3500);       buffer += ",";              // Channel B from TRIAD sensor (435 nm) |
  buffer += (TRIAD3.getCalibratedC()/3500);       buffer += ",";              // Channel C from TRIAD sensor (460 nm) |
  buffer += (TRIAD3.getCalibratedD()/3500);       buffer += ",";              // Channel D from TRIAD sensor (485 nm) |  AS7265 3 (410nm/535nm)
  buffer += (TRIAD3.getCalibratedE()/3500);       buffer += ",";              // Channel E from TRIAD sensor (510 nm) |
  buffer += (TRIAD3.getCalibratedF()/3500);       buffer += ",";              // Channel F from TRIAD sensor (535 nm)  ---
  buffer += (TRIAD3.getCalibratedG()/3500);       buffer += ",";              // Channel G from TRIAD sensor (560 nm)  ---
  buffer += (TRIAD3.getCalibratedH()/3500);       buffer += ",";              // Channel H from TRIAD sensor (585 nm) |
  buffer += (TRIAD3.getCalibratedI()/3500);       buffer += ",";              // Channel I from TRIAD sensor (645 nm) |
  buffer += (TRIAD3.getCalibratedJ()/3500);       buffer += ",";              // Channel J from TRIAD sensor (705 nm) |  AS7265 2 (560nm/940nm)
  buffer += (TRIAD3.getCalibratedK()/3500);       buffer += ",";              // Channel K from TRIAD sensor (900 nm) | 
  buffer += (TRIAD3.getCalibratedL()/3500);       buffer += ",";              // Channel L from TRIAD sensor (940 nm)  ---
  buffer += (TRIAD3.getCalibratedR()/3500);       buffer += ",";              // Channel R from TRIAD sensor (610 nm)  ---
  buffer += (TRIAD3.getCalibratedS()/3500);       buffer += ",";              // Channel S from TRIAD sensor (680 nm) |
  buffer += (TRIAD3.getCalibratedT()/3500);       buffer += ",";              // Channel T from TRIAD sensor (730 nm) |
  buffer += (TRIAD3.getCalibratedU()/3500);       buffer += ",";              // Channel U from TRIAD sensor (760 nm) |  AS7265 1 (610nm/860nm)
  buffer += (TRIAD3.getCalibratedV()/3500);       buffer += ",";              // Channel V from TRIAD sensor (810 nm) |
  buffer += (TRIAD3.getCalibratedW()/3500);       buffer += ",";              // Channel W from TRIAD sensor (860 nm)  ---
  buffer += LightExposure3;                       buffer += ",";              // Light Exposure Equivalent
  buffer += TotIrradiance3;                       buffer += ",";              // Light Exposure Equivalent  
  EmptyCalc();                                                                // Function to empty values used for calculations
}

void buffValues4(void){ 
  int32_t lum = TSL4.getFullLuminosity();           //Set luminosity from TSL 4
  ir = lum >> 16;                                   //Set infrared from luminosity
  full = lum & 0xFFFF;                              //Set full from luminosity
  rawA = VEML4.rawUva();                            //Set rawA from VEML 4
  rawB = VEML4.rawUvb();                            //Set rawB from VEML 4
  visibleComp = VEML4.visibleCompensation();        //Set visible component from VEML 4
  irComp = VEML4.irCompensation();                  //Set infrared component from VEML 4
  MLX4.setCallibration(0);                          //Set calibration to 0 for MLX 4
  MLX4.measure(true);                               //Set measuro = true for MLX 4
  CalcVEML();                                       //Calculate function for VEML
  CalcTotal_4();                                    //Calculate function for total values and light exposure equivalent

  // Filling the buffer with (calculated) data from sensors
  buffer += BP4.getThmc();                        buffer += ",";              // Temperature from blackpanel sensor
  buffer += MLX4.maxTemp;                         buffer += ",";              // Maximum temperature from MLX90621 sensors
  buffer += MLX4.avgTemp;                         buffer += ",";              // Average temperature from MLX90621 sensors
  buffer += ir;                                   buffer += ",";              // Infrared from TSL sensor
  buffer += (full-ir);                            buffer += ",";              // Visible from TSL sensor
  buffer += TSL4.calculateLux(full, ir), 6;       buffer += ",";              // Lux from TSL sensor
  buffer += (uviaCalc/93);                        buffer += ",";              // UVA from VEML sensor
  buffer += (uvibCalc/210);                       buffer += ",";              // UVB from VEML sensor
  buffer +=  uvi;                                 buffer += ",";              // UV Index from VEML sensor
  buffer += (TRIAD4.getCalibratedA()/3500);       buffer += ",";              // Channel A from TRIAD sensor (410 nm)  ---
  buffer += (TRIAD4.getCalibratedB()/3500);       buffer += ",";              // Channel B from TRIAD sensor (435 nm) |
  buffer += (TRIAD4.getCalibratedC()/3500);       buffer += ",";              // Channel C from TRIAD sensor (460 nm) |
  buffer += (TRIAD4.getCalibratedD()/3500);       buffer += ",";              // Channel D from TRIAD sensor (485 nm) |  AS7265 3 (410nm/535nm)
  buffer += (TRIAD4.getCalibratedE()/3500);       buffer += ",";              // Channel E from TRIAD sensor (510 nm) |
  buffer += (TRIAD4.getCalibratedF()/3500);       buffer += ",";              // Channel F from TRIAD sensor (535 nm)  ---
  buffer += (TRIAD4.getCalibratedG()/3500);       buffer += ",";              // Channel G from TRIAD sensor (560 nm)  ---
  buffer += (TRIAD4.getCalibratedH()/3500);       buffer += ",";              // Channel H from TRIAD sensor (585 nm) |
  buffer += (TRIAD4.getCalibratedI()/3500);       buffer += ",";              // Channel I from TRIAD sensor (645 nm) |
  buffer += (TRIAD4.getCalibratedJ()/3500);       buffer += ",";              // Channel J from TRIAD sensor (705 nm) |  AS7265 2 (560nm/940nm)
  buffer += (TRIAD4.getCalibratedK()/3500);       buffer += ",";              // Channel K from TRIAD sensor (900 nm) | 
  buffer += (TRIAD4.getCalibratedL()/3500);       buffer += ",";              // Channel L from TRIAD sensor (940 nm)  ---
  buffer += (TRIAD4.getCalibratedR()/3500);       buffer += ",";              // Channel R from TRIAD sensor (610 nm)  ---
  buffer += (TRIAD4.getCalibratedS()/3500);       buffer += ",";              // Channel S from TRIAD sensor (680 nm) |
  buffer += (TRIAD4.getCalibratedT()/3500);       buffer += ",";              // Channel T from TRIAD sensor (730 nm) |
  buffer += (TRIAD4.getCalibratedU()/3500);       buffer += ",";              // Channel U from TRIAD sensor (760 nm) |  AS7265 1 (610nm/860nm)
  buffer += (TRIAD4.getCalibratedV()/3500);       buffer += ",";              // Channel V from TRIAD sensor (810 nm) |
  buffer += (TRIAD4.getCalibratedW()/3500);       buffer += ",";              // Channel W from TRIAD sensor (860 nm)  ---
  buffer += LightExposure4;                       buffer += ",";              // Light Exposure Equivalent
  buffer += TotIrradiance4;                       buffer += ",";              // Light Exposure Equivalent  
  EmptyCalc();                                                                // Function to empty values used for calculations
}


void loop(void){                        //Main loop, runs continious until time cap is reached
  hum = DHT1.readHumidity();            //Read humidity from DHT sensor
  temp= DHT1.readTemperature();         //Read temperature from DHT sensor
  e_stop = digitalRead(33);             //Read the emergency stop input from pin 33

  if(e_stop == 0){                      //When the emergency stop is pressed stop script and turn off the lamp
    digitalWrite(relay_0, LOW);         //Turn off lamp
    digitalWrite(relay_1, LOW);         //Turn off lamp
    digitalWrite(relay_2, LOW);         //Turn off lamp
    digitalWrite(relay_3, LOW);         //Turn off lamp
    if (A ==0){                         //If statement for the buzzer so that it only is on for 3 seconds
       analogWrite(2, 127);             //Turn on the buzzer
       delay(3000);                     //Wait 3 seconds with buzzer on
       analogWrite(2, 0);               //Turn off the buzzer
       A = 1;                           //Make A 1 so that the buzzer won't turn on again
    }
  }
  
  if(e_stop == 1){                      //When the emergency stop is released turn on the lamp
    digitalWrite(relay_0, HIGH);        //Turn on the lamp when emergency butten is lose
    digitalWrite(relay_1, HIGH);        //Turn on the lamp when emergency butten is lose
    digitalWrite(relay_2, HIGH);        //Turn on the lamp when emergency butten is lose
    digitalWrite(relay_3, HIGH);        //Turn on the lamp when emergency butten is lose
    A = 0;                              //Make A 0 so that the buzzer will turn on again once emergency stop is pressed again
  } 
  
  tcaselect(0);                         //Select adress 0 on multiplexer
  buffValues1();                        //Calculate and send measured values
  if(buffer.length() > 2){              //If content of string buffer is greater than 2, print buffer on serial monitor and then empty the string
    Serial.println(buffer);             //Print the buffer
    buffer = String("");                //Empty the string 
  } 
  
  tcaselect(1);                         //Select adress 1 on multiplexer
  buffValues2();                        //Calculate and send measured values
  if(buffer.length() > 2){              //If content of string buffer is greater than 2, print buffer on serial monitor and then empty the string
    Serial.println(buffer);             //Print the buffer
    buffer = String("");                //Empty the string 
  }

  tcaselect(2);                         //Select adress 2 on multiplexer
  buffValues3();                        //Calculate and send measured values
  if(buffer.length() > 2){              //If content of string buffer is greater than 2, print buffer on serial monitor and then empty the string
    Serial.println(buffer);             //Print the buffer
    buffer = String("");                //Empty the string 
  }

  tcaselect(3);                         //Select adress 3 on multiplexer
  buffValues4();                        //Calculate and send measured values
  if(buffer.length() > 2){              //If content of string buffer is greater than 2, print buffer on serial monitor and then empty the string
    Serial.println(buffer);             //Print the buffer
    buffer = String("");                //Empty the string 
  }
  
  if (millis() > 28800000){             //Timer for test duration (8 hours = 28800000, 40 hours = 144000000, 60 hours = 216000000)
    digitalWrite(relay_0, LOW);         //Turn off lamp
    exit(0);                            //Exit program
  }
}
