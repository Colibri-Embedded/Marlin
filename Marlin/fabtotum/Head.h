#ifndef FABTOTUM_HEAD_H
#define FABTOTUM_HEAD_H

namespace FABtotum {

enum head_check_e {
  NO_CHECK = 0,
  THERMISTOR_CHECK = 1,
  TWI_CHECK = 2
};

class Head {
  public:
    Head();

    static unsigned int  installed_head_id;

    // Power State
    void set24VPower(bool state);
    void setLightPower(bool state);
    void setLineLaserPower(bool state);
    void setMillingMotorPower(bool state);
    void setServo1Power(bool state);
    void setServo2Power(bool state);

    // Check
    bool isInPlace();

  private:
    bool          head_in_place;
    head_check_e  head_check_type;
};

}

#endif /* FABTOTUM_HEAD_H */
