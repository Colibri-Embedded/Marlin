#include <fabtotum/Bed.h>
#include <temperature.h>

namespace FABtotum {

Bed bed;

bool Bed::isInPlace()
{
  return thermalManager.degBed() > 0;
}


}
