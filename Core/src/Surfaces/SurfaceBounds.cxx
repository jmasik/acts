///////////////////////////////////////////////////////////////////
// SurfaceBounds.cxx, ACTS project
///////////////////////////////////////////////////////////////////


//Trk
#include "ACTS/Surfaces/SurfaceBounds.h"

std::ostream& Acts::operator << ( std::ostream& sl, const SurfaceBounds& sb)
{ return sb.dump(sl); }