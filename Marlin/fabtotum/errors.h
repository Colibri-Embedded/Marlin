#ifndef FABTOTUM_ERRORS_H
#define FABTOTUM_ERRORS_H

//error codes
#define ERROR_KILLED      100
#define ERROR_STOPPED     101
#define ERROR_DOOR_OPEN   102
#define ERROR_MIN_TEMP    103
#define ERROR_MAX_TEMP    104
#define ERROR_MAX_BED_TEMP  105
#define ERROR_X_MAX_ENDSTOP  106
#define ERROR_X_MIN_ENDSTOP  107
#define ERROR_Y_MAX_ENDSTOP  108
#define ERROR_Y_MIN_ENDSTOP  109
#define ERROR_IDLE_SAFETY    110
#define ERROR_WIRE_END       111

//error codes for FABUI configurable functionalities
#define ERROR_Y_BOTH_TRIGGERED   120
#define ERROR_Z_BOTH_TRIGGERED   121

//temperature related messages
#define ERROR_AMBIENT_TEMP   122
#define ERROR_EXTRUDE_MINTEMP   123
#define ERROR_LONG_EXTRUSION   124
#define ERROR_HEAD_ABSENT    125

//POWER SHUTDOWN REQUEST:
#define ERROR_PWR_OFF    999

#endif /* FABTOTUM_ERRORS_H */
