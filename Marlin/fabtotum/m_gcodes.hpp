#if ENABLED(TOTUMDUINO)

#include "fabtotum/FABtotum.h"

/**
 * M150: Set Status LED Color - Use R-U-B-W for R-G-B-W
 *
 * Always sets all 3 or 4 components. If a component is left out, set to 0.
 *
 * Examples:
 *
 *   M150 R255       ; Turn LED red
 *   M150 R255 U127  ; Turn LED orange (PWM only)
 *   M150            ; Turn LED off
 *   M150 R U B      ; Turn LED white
 *   M150 W          ; Turn LED white using a white LED
 *
 */
inline void gcode_M150() {

  byte red = 0;
  byte green = 0;
  byte blue = 0;

  if (parser.seen('R')) red   = parser.intval('R');
  if (parser.seen('U')) green = parser.intval('U');
  if (parser.seen('B')) blue  = parser.intval('B');

  if (parser.seen('S')) {

    byte speed = parser.intval('S');
    if(speed == 0) {
      FABtotum::core.stopFading();
      FABtotum::core.setAmbientColor(red, green, blue);
    } else {
      FABtotum::core.setAmbientColorFading(red, green, blue, speed);
    }
  } else {
    FABtotum::core.setAmbientColor(red, green, blue);
  }
}

/**
 * M701 S<0-255> - Ambient Light, Set Red
 */
inline void gcode_M701() {

  if( FABtotum::core.isAmbientFading() ) {
    FABtotum::core.stopFading();
  }

  if (parser.seen('S'))
  {
    unsigned value = parser.intval('S');
    FABtotum::core.setAmbientRed(value);
  }
}

/**
 * M702 S<0-255> - Ambient Light, Set Green
 */
inline void gcode_M702() {

  if( FABtotum::core.isAmbientFading() ) {
    FABtotum::core.stopFading();
  }

  if (parser.seen('S'))
  {
    unsigned value = parser.intval('S');
    FABtotum::core.setAmbientGreen(value);
  }
}

/**
 * M703 S<0-255> - Ambient Light, Set Blue
 */
inline void gcode_M703() {

  if( FABtotum::core.isAmbientFading() ) {
    FABtotum::core.stopFading();
  }

  if (parser.seen('S'))
  {
    unsigned value = parser.intval('S');
    FABtotum::core.setAmbientBlue(value);
  }
}

/**
 * M720 - 24VDC head power ON
 */
inline void gcode_M720() {
  FABtotum::head.set24VPower(true);
  FABtotum::head.setServo1Power(true);
}

/**
 * M721 - 24VDC head power ON
 */
inline void gcode_M721() {
  FABtotum::head.set24VPower(false);
  FABtotum::head.setServo1Power(false);
}

/**
 * M722 - 5VDC SERVO_1 power ON
 */
inline void gcode_M722() {
  FABtotum::head.setServo1Power(true);
}

/**
 * M723 - 5VDC SERVO_1 power OFF
 */
inline void gcode_M723() {
  FABtotum::head.setServo1Power(false);
}

/**
 * M724 - 5VDC SERVO_2 power ON
 */
inline void gcode_M724() {
  FABtotum::head.setServo2Power(true);
}

/**
 * M725 - 5VDC SERVO_2 power OFF
 */
inline void gcode_M725() {
  FABtotum::head.setServo2Power(false);
}

/**
 * M726 - 5VDC RASPBERRY PI power ON
 */
inline void gcode_M726() {
  FABtotum::core.setRPiPower(true);
}

/**
 * M727 - 5VDC RASPBERRY PI power OFF
 */
inline void gcode_M727() {
  FABtotum::core.setRPiPower(false);
}

/**
 * M740 - read WIRE_END sensor
 */
inline void gcode_M740() {
  SERIAL_ECHOLN( FABtotum::core.isWireEndTriggered()?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN);
}

/**
 * M741 - read DOOR_OPEN sensor
 */
inline void gcode_M741() {
  SERIAL_ECHOLN( FABtotum::core.isDoorOpen()?MSG_ENDSTOP_OPEN:MSG_ENDSTOP_HIT);
}

/**
 * M742 - read REEL_LENS_OPEN sensor
 */
inline void gcode_M742() {
  SERIAL_ECHOLN( FABtotum::core.isReelLensOpen()?MSG_ENDSTOP_OPEN:MSG_ENDSTOP_HIT);
}

/**
 * M743 - read SECURE_SWITCH sensor
 */
inline void gcode_M743() {
  SERIAL_ECHOLN( FABtotum::core.isSecureSwitchTriggered()?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN);
}

/**
 * M744 - read HOT_BED placed on the printing side
 */
inline void gcode_M744() {
  SERIAL_ECHOLN( FABtotum::bed.isInPlace()?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN);
}

/**
 * M745 - read Head placed in place
 */
inline void gcode_M745() {
  SERIAL_ECHOLN( FABtotum::head.isInPlace()?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN);
}

/**
 * M760 : read FABtotum Personal Fabricator Main Controller serial ID
 */
inline void gcode_M760() {
  SERIAL_ECHOLN(FABtotum::core.fab_serial_code);
}

/**
 * M761: read FABtotum Personal Fabricator Main Controller control code of serial ID
 */
inline void gcode_M761() {
  SERIAL_ECHOLN(FABtotum::core.fab_control_serial_code);
}

/**
 * M763: read/write FABtotum Personal Fabricator Main Controller production batch number
 */
inline void gcode_M763() {
  if (parser.seen('S')) {
    FABtotum::core.fab_batch_number = parser.seenval('S');
  }
  SERIAL_ECHOLN(FABtotum::core.fab_batch_number);
}

/**
 * M765: read FABtotum Personal Fabricator Firmware Version
 */
inline void gcode_M765() {
  SERIAL_ECHOLN(STRING_BUILD_VERSION);
}

/**
 * M766: read FABtotum Personal Fabricator Firmware Version
 */
inline void gcode_M766() {
  SERIAL_ECHOLN(STRING_BUILD_DATE);
}

/**
 * M767: read FABtotum Personal Fabricator Firmware Version
 */
inline void gcode_M767() {
  SERIAL_ECHOLN(STRING_CONFIG_H_AUTHOR);
}

#endif /* TOTUMDUINO */
