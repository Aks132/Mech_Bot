#include "FastLED.h"
#define NUM_LEDS 60   
#define DATA_PIN 35
               
CRGB leds[NUM_LEDS];
int temp_error=0;
long int error_starttime = 0;
long side_light_start = 0;
bool side_light_state = true;
//---------------Sterring---------------------------------------
int DIR_pin = 13;
int PWM_pin = 12;
int ENC_A = 20;
int ENC_B = 21;
int A =0;
int B=0;
int position_error = 0;
int sterring_speed = 0;
char Drive_mode = 'N';
//------------------------------------
int Neutral_pin = 26;
int Reverse_pin = 27;
int Brake_pin = 24;

int channel_data[10] = {0,0,0,0,0,0,0,0,0,0};

int Current_position_A =0;
int Current_position_B =0;
int Current_position = 0;

bool M_D_connected = true;
bool flysky_connected = true; 
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;
long int motor_driver_start_time = 0;
long int Sterring_input = 0;

int throttle_pin = 4;
int throttle= 0;


// Variables for brake
int B_DIR_pin[2] = {8,9};
int B_PWM_pin[2] = {11,10};//Right,left
int B_ENC_A[2] = {19,2};//3 18
int B_ENC_B[2] = {18,3};//2 19
int B_position_error[2] = {0,0};
int Brake_Speed = 0;
int B_Current_position_B[2] = {0,0};
int B_Current_position_A[2] = {0,0};
int Current_brake_position[2] = {0,0};
int enable_encoder_0 = false;
int enable_encoder_1 = false;
bool enable_encoder = true;


//-----------------Lights----------------
int Head_light_pin[2] = {32,33};//HB,LB(All on)
int back_light_pin[2]= {34,34};
int side_light_pin[2] = {31,29};//RI,LI

char Head_light_mode = 'L';
char left_side_light = 'N';
char right_side_light = 'F';
char back_light = 'F';
bool drive_mode_change = false;
char previous_drive_mode = 'N';
long int neutral_light_start = 0;
float value = 0;
bool fadding = true;
void setup(){
           
   Serial.begin(115200);
   Serial.setTimeout(100);
   pinMode(Neutral_pin, OUTPUT);
   pinMode(Reverse_pin, OUTPUT);
   pinMode(Brake_pin, OUTPUT);
   digitalWrite(Brake_pin,HIGH);

   throttle_setup();
   brake_setup();
   motor_setup();
   ibus_setup();
   light_setup();
   
   
  
}
void send_data(){
  Serial.print("@@#"+String(throttle)+"#"+String(Brake_Speed)+"#"+String(Drive_mode)
  +"#"+String(Sterring_input)+"# "+String(Current_position_B)+" @ "+String(Current_position)+" @ "+String(Current_position_A)+" #"+String(flysky_connected)
  +"#"+String(B_position_error[1])+"#"+String()+" "+String(loop_time)+" "+String(Drive_mode)+" "+"#@@");
  Serial.println();
//  
 // Serial.println(String(B_Current_position_B[0])+"  "+String(B_Current_position_B[1])+"   "+String(B_Current_position_A[0])+"   "+String(B_Current_position_A[1])+"  "+String(Current_position_B)+" "+ String(Current_position_A));
 // Serial.println();
//  Serial.println(B_position_error[0]);
//  Serial.println(Current_brake_position[0]);
}


void loop(){
loopstart = millis();
ibus_loop();
braking();
drive_mode();
motor_loop();
send_data();
throttling();
lights();

loop_time = millis() - loopstart;
}
