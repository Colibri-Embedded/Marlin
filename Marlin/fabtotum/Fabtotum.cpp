#include "Fabtotum.h"
#include <MarlinConfig.h>
#include <fabtotum/macros.h>

/* static variables */
int Fabtotum::servo_extended_angle;
int Fabtotum::servo_retracted_angle;

unsigned long Fabtotum::fab_serial_code;
unsigned long Fabtotum::fab_control_serial_code;
unsigned int  Fabtotum::fab_board_version;
unsigned int  Fabtotum::fab_batch_number;
unsigned long Fabtotum::fab_control_batch_number;
unsigned int  Fabtotum::installed_head_id;

unsigned int  Fabtotum::led_board_version;
unsigned int  Fabtotum::flex_board_version;
unsigned int  Fabtotum::plateconn_board_version;
unsigned int  Fabtotum::hotplate_board_version;
unsigned int  Fabtotum::general_assembly_version;

Fabtotum::Fabtotum()
{
  fading_speed      = 100;
  led_update_cycles = 0;
  red_fading        = false;
  green_fading      = false;
  blue_fading       = false;
  slope             = true;
  fading_started    = false;
  
  FabSoftPwm_LMT    = MAX_PWM;
  FabSoftPwm_TMR    = 0;
  HeadLightSoftPwm  = 0;
  LaserSoftPwm      = 0;
  RedSoftPwm        = 0;
  GreenSoftPwm      = 0;
  BlueSoftPwm       = 0;
}

void Fabtotum::init()
{
  SET_OUTPUT(RED_PIN);
  SET_OUTPUT(GREEN_PIN);
  SET_OUTPUT(BLUE_PIN);
  pinMode(HOT_LED_PIN,OUTPUT);
  pinMode(DOOR_OPEN_PIN,INPUT);
  pinMode(HEAD_LIGHT_PIN,OUTPUT);
  pinMode(LASER_GATE_PIN,OUTPUT);
  pinMode(MILL_MOTOR_ON_PIN,OUTPUT);
  pinMode(NOT_SERVO1_ON_PIN,OUTPUT);
  pinMode(NOT_SERVO2_ON_PIN,OUTPUT);

  //setting analog as input
  //~ SET_ANALOG(MAIN_CURRENT_SENSE_PIN);
  //~ SET_ANALOG(MON_5V_PIN);
  //~ SET_ANALOG(MON_24V_PIN);
  //~ SET_ANALOG(PRESSURE_ANALOG_PIN);

  //POWER MABNAHGEMENT
  pinMode(PWR_OUT_PIN, OUTPUT);  //set external PSU shutdown pin (Optional on I2C)
  pinMode(PWR_IN_PIN,INPUT);  //set external PSU shutdown pin (Optional on I2C)
  digitalWrite(PWR_OUT_PIN, HIGH);

  //fastio init
  // SET_INPUT(IO)  ; SET_OUTPUT(IO);
  SET_INPUT(WIRE_END_PIN);
  SET_OUTPUT(NOT_RASPI_PWR_ON_PIN);
  SET_INPUT(NOT_SECURE_SW_PIN);
  SET_OUTPUT(NOT_REEL_LENS_OPEN_PIN);
  SET_OUTPUT(LIGHT_SIGN_ON_PIN);
  SET_OUTPUT(RPI_RECOVERY_PIN);

  //set output
  RED_OFF();
  GREEN_OFF();
  BLUE_OFF();
  RPI_ERROR_ACK_OFF();

  HOT_LED_OFF();

  HEAD_LIGHT_OFF();
  LASER_GATE_OFF();

  MILL_MOTOR_OFF();
  SERVO1_OFF();
  #if (NUM_SERVOS > 1)
    SERVO2_OFF();
  #endif

  RASPI_PWR_ON();

  LIGHT_SIGN_ON();

  FabSoftPwm_LMT    = MAX_PWM;
  FabSoftPwm_TMR    = 0;
  HeadLightSoftPwm  = 0;
  LaserSoftPwm      = 0;
  RedSoftPwm        = 0;
  GreenSoftPwm      = 0;
  BlueSoftPwm       = 0;
}

void Fabtotum::manage_inactivity()
{
  manage_soft_pwm();
  manage_amb_color_fading();
}

void Fabtotum::manage_soft_pwm() {

  if (FabSoftPwm_TMR == 0)
  {
#ifdef ENABLE_LASER_MODE
    if (Laser::power > 0) WRITE(SERVO0_PIN,1);
    if (LaserSoftPwm>0) { 
      LASER_GATE_ON();  
    }
#endif

    if (HeadLightSoftPwm>0) {
      HEAD_LIGHT_ON();
    }
    if (RedSoftPwm>0) {
      RED_ON();
    }
    if (GreenSoftPwm>0) {
      GREEN_ON();
    }
    if (BlueSoftPwm>0) {
      BLUE_ON();
    }
  }
  else
  {
#ifdef ENABLE_LASER_MODE
    if (FabSoftPwm_TMR > Laser::power) WRITE(SERVO0_PIN,0);
    if ( FabSoftPwm_TMR>LaserSoftPwm && LaserSoftPwm<MAX_PWM ) { 
      LASER_GATE_OFF(); 
    }
#endif


    if ( FabSoftPwm_TMR>HeadLightSoftPwm && HeadLightSoftPwm<MAX_PWM ) {
      HEAD_LIGHT_OFF();
    }
    if ( FabSoftPwm_TMR>RedSoftPwm && RedSoftPwm<MAX_PWM) {
      RED_OFF();
    }
    if ( FabSoftPwm_TMR>GreenSoftPwm && GreenSoftPwm<MAX_PWM ) {
      GREEN_OFF();
    }
    if ( FabSoftPwm_TMR>BlueSoftPwm && BlueSoftPwm<MAX_PWM ) {
      BLUE_OFF();
    }
  }

  if (++FabSoftPwm_TMR > FabSoftPwm_LMT)
    FabSoftPwm_TMR = 0;
}

void Fabtotum::manage_amb_color_fading()
{
  if(red_fading || green_fading || blue_fading) {
    fading_started=true;
    if(led_update_cycles>fading_speed) {
      
      if(slope) 
      {
        if(red_fading) {RedSoftPwm=RedSoftPwm+1;}
        if(green_fading){GreenSoftPwm=GreenSoftPwm+1;}
        if(blue_fading){BlueSoftPwm=BlueSoftPwm+1;}
      } 
      else 
      {
        if(red_fading){RedSoftPwm=RedSoftPwm-1;}
        if(green_fading){GreenSoftPwm=GreenSoftPwm-1;}
        if(blue_fading){BlueSoftPwm=BlueSoftPwm-1;}
      }
      
      if(( (RedSoftPwm==MAX_PWM || RedSoftPwm==0) && red_fading) 
        || ((GreenSoftPwm==MAX_PWM || GreenSoftPwm==0) && green_fading) 
        || ((BlueSoftPwm==MAX_PWM || BlueSoftPwm==0) && blue_fading)
        ) 
      {
          slope=!slope;
      }

      led_update_cycles=0;
    }
    led_update_cycles=led_update_cycles+1;
  }
  
  if(!red_fading && !green_fading && !blue_fading && fading_started) {
    led_update_cycles=0;
    fading_started=false;
    slope=true;
  }
}

void Fabtotum::setAmbientColor(unsigned int red, unsigned int green, unsigned int blue) {
  
  RedSoftPwm    = red;
  GreenSoftPwm  = green;
  BlueSoftPwm   = blue;
}

void Fabtotum::setAmbientColorFading(bool red_bool, bool green_bool, bool blue_bool, unsigned int fading_speed_in) {
  
  setAmbientColor(0,0,0);

  led_update_cycles = 0;
  fading_started    = false;
  slope             = true;

  red_fading    = red_bool;
  green_fading  = green_bool;
  blue_fading   = blue_bool;

  fading_speed  = fading_speed_in;
}

void Fabtotum::store_ambient_color() {
  
}

void Fabtotum::restore_ambient_color() {
  
}

bool Fabtotum::isAmbientFading()
{
  return (red_fading || green_fading || blue_fading);
}

void Fabtotum::setAmbientRed(unsigned int value) {
  if(value >= 255) {
    RedSoftPwm  = MAX_PWM;
  } else {
    RedSoftPwm  = (unsigned int)(value/2);
  }
}

void Fabtotum::setAmbientGreen(unsigned int value) {
  if(value >= 255) {
    GreenSoftPwm  = MAX_PWM;
  } else {
    GreenSoftPwm  = (unsigned int)(value/2);
  }
}

void Fabtotum::setAmbientBlue(unsigned int value) {
  if(value >= 255) {
    BlueSoftPwm  = MAX_PWM;
  } else {
    BlueSoftPwm  = (unsigned int)(value/2);
  }
}

void Fabtotum::stopFading()
{
  
}
