//*****************************************************************************
//
// $Created: FBD 02-11-15 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Tools_Types.h $
// $Author: Nld $
// $Modtime: 19/08/04 16:34 $
// $Revision: 17 $
// $Workfile: MT_Tools_Types.h $
//
//*****************************************************************************

#ifndef __MT_Tools_Types_h_
#define __MT_Tools_Types_h_

#include <cmath>

#define MT_PI 3.14159265358979323

inline bool MT_IsZero( double r )
{
    return fabs( r ) < 1e-8;
}

#endif // __MT_Tools_Types_h_
