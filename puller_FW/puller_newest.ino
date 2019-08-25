/*
  earc filament puller - puller FW
  ---------------------------------
  v1.1
  earc group (c)2019
  Lukas Kovarik
  Jan Kovar
  ---------------------------------
  6.2019
*/

#include "Arduino.h"
#include "BasicStepperDriver.h"

#include <U8glib.h>
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

#define STEP_0 3
#define DIR_0 2
#define MOTOR_STEPS 200
#define MICROSTEPS 8

float dim_x[10];
float dim_y[10];

bool startSaving = 1;

float filament_width = 1.750; //i2c read
float filament_height = 1.750;
float ovality;

float driving_diameter[1];
float napicu_diameter[1];

int fil_log[4];
/* TRANSPARENT
float speed_const_base = 56.5;
float speed_const = 56.5;
*/

//BLUE
float speed_const_base = 45 ;
float speed_const = 45;

BasicStepperDriver stepper_puller(MOTOR_STEPS, DIR_0, STEP_0);



String x_data;
String y_data;
//-----------------------------------------------------------
// i2c_slave_test.ino
#include <Wire.h>
 
#define SLAVE_ADDRESS 0x08
 
// 10 byte data buffer
int receiveBuffer[9];
uint8_t keepCounted = 0;
 
 
// Read data in to buffer, offset in first element.
int data_1 = 0;
int data_2 = 0;
int data_3 = 0;
int data_4 = 0;
int cnt = 0;

float diam = 0;
 
// Read data in to buffer, offset in first element.
void receiveData(int byteCount){
  int counter = 0;

  while(Wire.available()) {
    receiveBuffer[counter] = Wire.read();
    //Serial.println("log");
    if(cnt == 3){
      data_4 = receiveBuffer[counter];
      fil_log[3] = data_4;
      cnt = 0;
/*
      Serial.println(data_1);
      Serial.println(data_2);
      Serial.println(data_3);
      Serial.println(data_4);
      Serial.println("--------");
*/
      diam = (data_3 * 100 + data_4);
      diam = diam / 1000;
  /*
      //if(data_2 == 1){diam1 = diam;}else{diam2 = diam;}
      Serial.print("diam: ");
      Serial.println(diam);
      Serial.println(data_2);
      Serial.println("--------");
*/
    }

    if(cnt == 2){
      data_3 = receiveBuffer[counter];
      fil_log[2] = data_3;
      cnt = 3;
    }

    if(cnt == 1){
      data_2 = receiveBuffer[counter];
      fil_log[1] = data_2;
      cnt = 2;
    }
    
    if(receiveBuffer[counter] == 0){
      data_1 = receiveBuffer[counter];
      fil_log[0] = data_1;
      cnt = 1;
    }

    //Serial.println(receiveBuffer[counter]);   // recived data !!
    counter ++;
  }

}
 
// Counter function
int keepCount() {
  keepCounted ++;
  if (keepCounted > 255) {
    keepCounted = 0;
    return 0;
  } else {
    return keepCounted;
  }
}
//----------------------------------------------


void setup() {
  Serial.begin(57600);
  Serial.println();
  Serial.println("earc filament winder - puller system");
  Serial.println("-----------------------------------");
  Serial.println("v1.1");
  Serial.println("earc group (c)2019");
  Serial.println("Lukas Kovarik");
  Serial.println("Jan Kovar");
  Serial.println("-----------------------------------");
  
  Wire.begin();
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  stepper_puller.enable();

}

void loop() {
  //Serial.println(diam);

  /*
  for(int i = 0; i < 4; i++){
    Serial.println(fil_log[i]);
  }
  */
/*
  if(digitalRead(startPin == 1)){
    startSaving = 1;
  }
  if(digitalRead(stopPin == 1)){
    startSaving = 0;
  }
*/  
  for(int fuck = 0; fuck < 10; fuck ++){
    //-------------WORKS---------------
    
    if(fil_log[1] == 1){
      driving_diameter[0] = diam;
    }/*
    if(fil_log[1] == 2){
      napicu_diameter[0] = diam;
    } 
    */
    speed_const = speed_const_base;
    
    if(driving_diameter[0] == 0 || driving_diameter[0] > 5 /*|| driving_diameter[0] < 1.2*/){
      driving_diameter[0] = 1.75;
    }

    if(driving_diameter[0] > 1.85){
      speed_const = speed_const + 0.8;
    }
    
    if(driving_diameter[0] > 3){
      speed_const = speed_const + 1.6;
    }
    
    if(driving_diameter[0] < 1.65){
      speed_const = speed_const - 1.2;
    }
    if(driving_diameter[0] < 1.55){
      speed_const = speed_const - 2;
    }
    Serial.println(speed_const);
    stepper_puller.begin(/*driving_diameter[0]**/speed_const, MICROSTEPS);
    stepper_puller.move(1000); //rotate or move
    //Serial.print("driving_diameter[0]: ");
    //Serial.println(driving_diameter[0]);
    //Serial.print("napicu_diameter[0]: ");
    //Serial.println(napicu_diameter[0]);
    //-------------WORKS---------------


    //x_data = x_data + "_" + String(driving_diameter[0]);
    //y_data = y_data + "_" + String(napicu_diameter[0]);

    Serial.print("x: ");
    Serial.println(driving_diameter[0]);
    Serial.print("y: ");
    Serial.println(napicu_diameter[0]);

    char startBit[1] = "0";
    char data_x_char[5];
    char data_y_char[5];

    //Serial.write(startBit, 1);

    //itoa(driving_diameter[0], data_x_char, 10); //Turn value into a character array
    //Serial.write(data_x_char, 5);

    //itoa(napicu_diameter[0], data_y_char, 10); //Turn value into a character array
    //Serial.write(data_y_char, 5);  
  }
  Serial.println("konec cyklu");
  /*
  Serial.println("Sending to server: ");
  Serial.println(x_data);
  Serial.println(y_data);
  */
  
  //send to server
  //
  //
  //                 <========================
  //
  //
  //sent to server
  
  //x_data = "";
  //y_data = "";
  



/*
  //-----------NOT WORKS :D-----------------
  u8g.setFont(u8g_font_fub14r);
  u8g.setPrintPos(10,10);        // set position
  u8g.println("No Battery!");
  //-----------NOT WORKS :D-----------------
*/

  //ovality = (2*(filament_width - filament_height))/(filament_width + filament_height);*/
 
}
