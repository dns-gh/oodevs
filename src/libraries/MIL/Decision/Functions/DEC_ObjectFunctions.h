// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.h $
// $Author: Nld $
// $Modtime: 10/03/05 18:17 $
// $Revision: 4 $
// $Workfile: DEC_ObjectFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_ObjectFunctions_h_
#define __DEC_ObjectFunctions_h_

#include "MIL.h"

class MIL_AgentPion;
class MIL_Automate;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_ObjectFunctions
{
public:
    //! @name Functions
    //@{
    template< typename T > static void MagicCreateObject ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void MagicDestroyObject( DIA_Call_ABC& call, const T& caller );

    static void ActivateObject         ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void GetPrisonerCampPosition( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void GetRefugeeCampPosition ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    //@}
};

#include "DEC_ObjectFunctions.inl"

#endif // __DEC_ObjectFunctions_h_
