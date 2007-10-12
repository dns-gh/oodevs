//*****************************************************************************
// 
// $Created: NLD 2002-12-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_DIAFunctions.h $
// $Author: Jvt $
// $Modtime: 18/05/05 14:46 $
// $Revision: 7 $
// $Workfile: DEC_DIAFunctions.h $
// 
//*****************************************************************************

#ifndef __DEC_DIAFunctions_h_
#define __DEC_DIAFunctions_h_

#include "MIL.h"

//=============================================================================
// Created: NLD 2002-12-24
// Last modified: JVT 03-09-25
//=============================================================================
class DEC_DIAFunctions
{
public:
    //! @name DIA Functions
    //@{
    // Debug
    static void PointToString    ( DIA_Call_ABC& );
    static void DirectionToString( DIA_Call_ABC& );
    static void PathToString     ( DIA_Call_ABC& );

    // DIA user types list manipulation
    static void UserTypeList_GetAt   ( DIA_Call_ABC& );
    static void UserTypeList_Contains( DIA_Call_ABC& );
    static void UserTypeList_PushBack( DIA_Call_ABC& );
    static void UserTypeList_Remove  ( DIA_Call_ABC& );
    static void ListPoint_GetAt      ( DIA_Call_ABC& );
    static void ListPoint_Size       ( DIA_Call_ABC& );
    static void ListPoint_PushBack   ( DIA_Call_ABC& );
    static void ListPoint_Remove     ( DIA_Call_ABC& );
    static void ListPoint_Clear      ( DIA_Call_ABC& );

    // Time management
    static void GetSimTime         ( DIA_Call_ABC& );
    static void GetRealTime        ( DIA_Call_ABC& );
    static void GetTimeStepDuration( DIA_Call_ABC& ); 
    static void IsNight            ( DIA_Call_ABC& );

    // Parameters copy
    static void CopyDirection                     ( DIA_Call_ABC& call );
    static void CopyPoint                         ( DIA_Call_ABC& call );
    static void CopyListPoint                     ( DIA_Call_ABC& call );
    static void CopyPointToListPoint              ( DIA_Call_ABC& call );
    static void CopyLocalisation                  ( DIA_Call_ABC& call );
    static void CopyPointToLocalisation           ( DIA_Call_ABC& call );
    static void CopyLocationList              ( DIA_Call_ABC& call );
    static void CopyLocalisationToLocationList( DIA_Call_ABC& call );

    // DIA Thing management
    static void CreateDIAThing ( DIA_Call_ABC& call );    
    static void DestroyDIAThing( DIA_Call_ABC& call );
    //@}
};

#endif // __DEC_DIAFunctions_h_
