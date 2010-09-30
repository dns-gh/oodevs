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

#include "MT_Assert.h"

typedef double        MT_Float;
#define MT_Epsilon  1e-8
#define MT_PI 3.14159265358979323
inline  MT_Float     MT_flabs( MT_Float rF )    { return ( rF >= 0. ? rF : -rF ); }
inline  bool         MT_IsZero( MT_Float rF )    { return MT_flabs( rF ) < MT_Epsilon; }
inline  bool         MT_IsSameSigns( MT_Float rA, MT_Float rB ) { return ( rA * rB ) >= 0.; }

typedef unsigned uint;
typedef long long int int64;

#define MT_COPYNOTALLOWED(cls)      \
private:                            \
    cls( const cls& );              \
    cls& operator=( const cls& );    

#endif // __MT_Tools_Types_h_
