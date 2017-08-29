#ifndef FABTOTUM_H
#define FABTOTUM_H

class Fabtotum {
  public:

    // variables

    static int servo_extended_angle;
    static int servo_retracted_angle;

    static unsigned long fab_serial_code;
    static unsigned long fab_control_serial_code;
    static unsigned int  fab_board_version;
    static unsigned int  fab_batch_number;
    static unsigned long fab_control_batch_number;
    static unsigned int  installed_head_id;

    static unsigned int  led_board_version;
    static unsigned int  flex_board_version;
    static unsigned int  plateconn_board_version;
    static unsigned int  hotplate_board_version;
    static unsigned int  general_assembly_version;
    
    // soft pwm
    unsigned int LaserSoftPwm;
    unsigned int HeadLightSoftPwm;
    unsigned int  FabSoftPwm_TMR;
    unsigned int FabSoftPwm_LMT;
    unsigned int RedSoftPwm;
    unsigned int GreenSoftPwm;
    unsigned int BlueSoftPwm;
    unsigned int RedSoftPwm_old;
    unsigned int GreenSoftPwm_old;
    unsigned int BlueSoftPwm_old;
    
    // fading
    unsigned int fading_speed;
    unsigned int led_update_cycles;
    bool red_fading;
    bool green_fading;
    bool blue_fading;
    bool slope;
    bool fading_started;
    
    // functions
    Fabtotum();
    void init();
    
    void manage_inactivity();
    
    void setAmbientColor(unsigned int red, unsigned int green, unsigned int blue);
    
    void setAmbientColorFading(bool red_bool, bool green_bool, bool blue_bool, unsigned int fading_speed_in);
    
    void store_ambient_color();
    void restore_ambient_color();
    
    void setAmbientRed(unsigned int);
    void setAmbientGreen(unsigned int);
    void setAmbientBlue(unsigned int);
    
    bool isAmbientFading();
    void stopFading();
    
  private:
    void manage_soft_pwm();
    void manage_amb_color_fading();
};

#endif /* FABTOTUM_H */
