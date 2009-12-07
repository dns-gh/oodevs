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
class MIL_Mission_ABC;
class DEC_Path_ABC;

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
    static std::string PointToString    ( MT_Vector2D* p1 );
    static std::string DirectionToString( MT_Vector2D* p1 );
    static std::string PathToString     ( DEC_Path_ABC* pPath );
    static void BreakForDebug           ( uint id, const std::string& message );

    // DIA user types list manipulation
    static boost::shared_ptr< MT_Vector2D > ListPoint_GetAt( std::vector< MT_Vector2D* > list, int nId );
    static int  ListPoint_Size       ( std::vector< MT_Vector2D* > list );

    // Time management
    static float GetSimTime ();
    static float GetRealTime();
    static bool  IsNight    ();

    // Parameters copy
    static void CopyPoint                     ( MT_Vector2D* pPosSource, boost::shared_ptr< MT_Vector2D > pPosDest );
    static void CopyPointMission              ( MT_Vector2D* pPosSource, MIL_Mission_ABC* pMission, const std::string& parameter );
    static void CopyPathMission               ( std::vector< boost::shared_ptr< MT_Vector2D > > pointList, MIL_Mission_ABC* pMission, const std::string& parameter );
    static void CopyPointToListPointMission   ( boost::shared_ptr< MT_Vector2D > pPosSource, MIL_Mission_ABC* pMission, const std::string& parameter );
    static void CopyLocalisation              ( const TER_Localisation* pLocSource, TER_Localisation* pLocDest );
    static void CopyLocalisationMission       ( boost::shared_ptr< TER_Localisation > pLocation, MIL_Mission_ABC* pMission, const std::string& parameter );
    static void CopyLocalisationListMission   ( const std::vector< boost::shared_ptr< TER_Localisation > >& locSource, MIL_Mission_ABC* pMission, const std::string& parameter );
    static void CopyLocalisationToLocationListMission( boost::shared_ptr< TER_Localisation > pLocSource, MIL_Mission_ABC* pMission, const std::string& parameter );
};

#endif // __DEC_DIAFunctions_h_
