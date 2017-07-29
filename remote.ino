   #include <PS2X_lib.h>  //for v1.6

#define PS2_DAT        13   //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17
#define pwm_2 5
#define pwm_1 6 
#define um_1  2 
#define um_2  3
#define dir_1 1
#define dir_2 0 

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class


short int count=0;
int error = 0;
byte type = 0;
byte vibrate = 0;
/*------------------Motor variables----------------*/
#define motor1  A0            //analog out for motors i.e, pwm pins
#define motor2  A1
#define motor3  A3
#define motor4  A4
/*--------------------------------------------------*/
void setup()

{
   pinMode(pwm_2,OUTPUT); 
   pinMode(pwm_1,OUTPUT); 
  
  Serial.begin(9600); 
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
 // Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
   }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */
/*------------------OUTER IF BLOCK------------------*/
  if(error == 1) //skip loop if no controller found
    return; 
    /*------------------INNER IF BLOCK------------------*/
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");

/*------------------OUTER ELSE BLOCK------------------*/
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    /*------------------INNER IF BLOCK------------------*/
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");      

    if(ps2x.Button(PSB_PAD_UP))       //will be TRUE as long as button is pressed
    {
     
    }
    if(ps2x.Button(PSB_PAD_RIGHT))
    {
       count=count+10;
      int swap=count/10;
      Serial.print(swap);
    //Serial.print("  held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      analogWrite(pwm_2,swap );
      analogWrite(pwm_1,swap ); 
    }
   if(count>0){
    if(ps2x.Button(PSB_PAD_LEFT))
    {
      count=count-10;
      int swap=count/10;
      Serial.print(swap);
    //Serial.print("  held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      analogWrite(pwm_2,swap );
      analogWrite(pwm_1,swap ); 
    } 
   }
    if(ps2x.Button(PSB_PAD_DOWN))
    {
      
    }   

    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) 
    {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3)){
        
         digitalWrite(dir_1,LOW);
         analogWrite(um_1,100);
         delay(200);
         analogWrite(um_1,0);
       
        }
     
      
      if(ps2x.Button(PSB_R3)){
       /* digitalWrite(dir_2,HIGH);
         analogWrite(um_2,100);
         delay(200);
         analogWrite(um_2,0);*/
         Serial.print("hii");
        
        
      }
      
     
      
      if(ps2x.Button(PSB_L2)){
         digitalWrite(dir_1,LOW);
         analogWrite(um_1,100);
         delay(200);
         analogWrite(um_1,0);
         
        
      }
        
      if(ps2x.Button(PSB_R2))
      
         {
          digitalWrite(dir_2,LOW);
         analogWrite(um_2,100);
         delay(200);
         analogWrite(um_2,0);
    }

    if(ps2x.Button(PSB_TRIANGLE))     //will be TRUE if button pressed & take continuous readings until released
    {
          digitalWrite(dir_2,LOW);
         analogWrite(um_2,-100);
         delay(200);
         analogWrite(um_2,0);   
    }
    if(ps2x.Button(PSB_CIRCLE)) {             //will be TRUE if button pressed & take continuous readings until released
        int DCV_pwm=90;
      
      digitalWrite(39,LOW);
      analogWrite(8,DCV_pwm);
      delay(500);
    
      digitalWrite(38,LOW);
      
    }
    if(ps2x.Button(PSB_CROSS)) 
    {//will be TRUE if button pressed & take continuous readings until released
      count=count-count;
      //count=count/10;
      if(count<=0)
      {
      Serial.print(count);
      
    //Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      analogWrite(pwm_2,count );
      analogWrite(pwm_1,count );
      }
    //Serial.println("X just changed");
    }
    if(ps2x.Button(PSB_SQUARE))              //will be TRUE if button pressed & take continuous readings until released
      { int DCV_pwm=90;

      digitalWrite(3
,HIGH);
     // digitalWrite(39,LOW);
      analogWrite(8,DCV_pwm);
      delay(500);
    
      // digitalWrite(38,LOW);
      digitalWrite(39,HIGH); 
      }
    
  }
   if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)){
         digitalWrite(dir_1,HIGH);
         digitalWrite(dir_2,HIGH);
         analogWrite(um_1,100);
         analogWrite(um_2,100);
         delay(200);
         analogWrite(um_1,0);
         analogWrite(um_2,0);
    
    
    }
    

  }}
