//*****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Types.h $
// $Author: Sbo $
// $Modtime: 19/07/05 16:42 $
// $Revision: 7 $
// $Workfile: Types.h $
//
//*****************************************************************************

#ifndef __Types_h_
#define __Types_h_

#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Assert.h"

typedef uint32      T_NetContextId;

typedef uint32      T_EntityId;

typedef int32       T_Direction;
typedef int32       T_Height;
typedef double      T_Speed;

enum E_Tristate
{
    eTristate_True = 0,
    eTristate_False,
    eTristate_DontKnow,
};


// archive logger
#include "MT_Tools/MT_InputArchive_Logger.h"

// archive logger XML
class MT_XXmlInputArchive;
typedef MT_InputArchive_Logger< MT_XXmlInputArchive > XmlInputArchive;

#include "MT_Tools/MT_Scipio_enum.h"

#endif // __Types_h_