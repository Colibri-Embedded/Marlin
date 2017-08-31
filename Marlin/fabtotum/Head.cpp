#include <fabtotum/Head.h>
#include <MarlinConfig.h>
#include <fabtotum/macros.h>

namespace FABtotum {

  Head head;

  unsigned int  Head::installed_head_id = 0;

  Head::Head()
  {
    head_in_place = true;
    head_check_type = NO_CHECK;
  }

  void Head::set24VPower(bool state)
  {
    if(state)
      HEAD_24V_ON();
    else
      HEAD_24V_OFF();
  }

  void Head::setLightPower(bool state)
  {

  }

  void Head::setLineLaserPower(bool state)
  {

  }

  void Head::setMillingMotorPower(bool state)
  {
    if(state)
      MILL_MOTOR_ON();
    else
      MILL_MOTOR_OFF();
  }

  void Head::setServo1Power(bool state)
  {
    if(state)
      SERVO1_ON();
    else
      SERVO1_OFF();
  }

  void Head::setServo2Power(bool state)
  {
    if(state)
      SERVO2_ON();
    else
      SERVO2_OFF();
  }

  bool Head::isInPlace()
  {
    return true;
  }

}
