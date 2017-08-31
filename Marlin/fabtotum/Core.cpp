#include <fabtotum/Core.h>
#include <MarlinConfig.h>
#include <fabtotum/macros.h>

namespace FABtotum {
  Core core;

  /* static variables */
  int Core::servo_extended_angle;
  int Core::servo_retracted_angle;

  unsigned long Core::fab_serial_code;
  unsigned long Core::fab_control_serial_code;
  unsigned int  Core::fab_board_version;
  unsigned int  Core::fab_batch_number;
  unsigned long Core::fab_control_batch_number;

  unsigned int  Core::led_board_version;
  unsigned int  Core::flex_board_version;
  unsigned int  Core::plateconn_board_version;
  unsigned int  Core::hotplate_board_version;
  unsigned int  Core::general_assembly_version;

  Core::Core()
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

    error_code        = 0;
  }

  void Core::init()
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

  void Core::manage_inactivity()
  {
    manage_soft_pwm();
    manage_amb_color_fading();
  }

  void Core::manage_soft_pwm() {

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

  void Core::manage_amb_color_fading()
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

  void Core::setAmbientColor(unsigned int red, unsigned int green, unsigned int blue) {

    RedSoftPwm    = red;
    GreenSoftPwm  = green;
    BlueSoftPwm   = blue;
  }

  void Core::setAmbientColorFading(bool red_bool, bool green_bool, bool blue_bool, unsigned int fading_speed_in) {

    setAmbientColor(0,0,0);

    led_update_cycles = 0;
    fading_started    = false;
    slope             = true;

    red_fading    = red_bool;
    green_fading  = green_bool;
    blue_fading   = blue_bool;

    fading_speed  = fading_speed_in;
  }

  void Core::store_ambient_color() {

  }

  void Core::restore_ambient_color() {

  }

  bool Core::isAmbientFading()
  {
    return (red_fading || green_fading || blue_fading);
  }

  void Core::setAmbientRed(unsigned int value) {
    if(value >= 255) {
      RedSoftPwm  = MAX_PWM;
    } else {
      RedSoftPwm  = (unsigned int)(value/2);
    }
  }

  void Core::setAmbientGreen(unsigned int value) {
    if(value >= 255) {
      GreenSoftPwm  = MAX_PWM;
    } else {
      GreenSoftPwm  = (unsigned int)(value/2);
    }
  }

  void Core::setAmbientBlue(unsigned int value) {
    if(value >= 255) {
      BlueSoftPwm  = MAX_PWM;
    } else {
      BlueSoftPwm  = (unsigned int)(value/2);
    }
  }

  void Core::stopFading()
  {

  }

  void Core::setRPiPower(bool state)
  {
    if(state)
      RASPI_PWR_ON();
    else
      RASPI_PWR_OFF();
  }

  bool Core::isWireEndTriggered()
  {
    #if defined(WIRE_END_INVERTING)
      return WIRE_END_STATUS() ^ WIRE_END_INVERTING;
    #else
      return WIRE_END_STATUS();
    #endif
  }

  bool Core::isDoorOpen()
  {
    return DOOR_OPEN_STATUS();
  }

  bool Core::isReelLensOpen()
  {
    return REEL_LENS_OPEN_STATUS();
  }

  bool Core::isSecureSwitchTriggered()
  {
    return SECURE_SW_STATUS();
  }

  void Core::setErrorCode(unsigned int error)
  {
    error_code = error;
  }

  unsigned int Core::getErrorCode()
  {
    return error_code;
  }

  bool Core::triggerEvent(unsigned int error)
  {
    return true;
  }

} /* namespace FABtotum */
