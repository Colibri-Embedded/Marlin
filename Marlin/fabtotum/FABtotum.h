#ifndef FABTOTUM_H
#define FABTOTUM_H

#include <fabtotum/Core.h>
#include <fabtotum/Head.h>
#include <fabtotum/Bed.h>
#include <fabtotum/enums.h>
#include <fabtotum/errors.h>

namespace FABtotum {
  extern Core core;
  extern Head head;
  extern Bed  bed;
}

#define FABTOTUM_EEPROM_OFFSET 70

#endif /* FABTOTUM_H */
