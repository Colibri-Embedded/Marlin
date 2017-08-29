#if ENABLED(TOTUMDUINO)

#include "fabtotum/Fabtotum.h"
Fabtotum fabtotum;

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
      fabtotum.stopFading();
      fabtotum.setAmbientColor(red, green, blue);
    } else {
      fabtotum.setAmbientColorFading(red, green, blue, speed);
    }
  } else {
    fabtotum.setAmbientColor(red, green, blue);
  }
}

/**
 * M701 S<0-255> - Ambient Light, Set Red
 */
inline void gcode_M701() {
  
  if( fabtotum.isAmbientFading() ) {
    fabtotum.stopFading();
  }
  
  if (parser.seen('S'))
  {
    unsigned value = parser.intval('S');
    fabtotum.setAmbientRed(value);
  }
}

/**
 * M702 S<0-255> - Ambient Light, Set Green
 */
inline void gcode_M702() {
  
  if( fabtotum.isAmbientFading() ) {
    fabtotum.stopFading();
  }
  
  if (parser.seen('S'))
  {
    unsigned value = parser.intval('S');
    fabtotum.setAmbientGreen(value);
  }
}

/**
 * M703 S<0-255> - Ambient Light, Set Blue
 */
inline void gcode_M703() {
  
  if( fabtotum.isAmbientFading() ) {
    fabtotum.stopFading();
  }
  
  if (parser.seen('S'))
  {
    unsigned value = parser.intval('S');
    fabtotum.setAmbientBlue(value);
  }
}

/**
 * M760 : read FABtotum Personal Fabricator Main Controller serial ID
 */
inline void gcode_M760() {
  SERIAL_ECHOLN(fabtotum.fab_serial_code);
}

/**
 * M761: read FABtotum Personal Fabricator Main Controller control code of serial ID
 */
inline void gcode_M761() {
  SERIAL_ECHOLN(fabtotum.fab_control_serial_code);
}

/**
 * M763: read/write FABtotum Personal Fabricator Main Controller production batch number
 */
inline void gcode_M763() {
  if (parser.seen('S')) {
    fabtotum.fab_batch_number = parser.seenval('S');
  }
  SERIAL_ECHOLN(fabtotum.fab_batch_number);
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
