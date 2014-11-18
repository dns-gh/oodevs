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

#include <boost/shared_ptr.hpp>

namespace sword
{
    class Brain;
}

class MIL_Mission_ABC;
class DEC_Decision_ABC;
class DEC_Gen_Object;
class DEC_Knowledge_Object;
class TER_Localisation;
class TER_Path_ABC;
class MT_Vector2D;

//=============================================================================
// Created: NLD 2002-12-24
// Last modified: JVT 03-09-25
//=============================================================================
class DEC_DIAFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name DIA Functions
    //@{
    // Debug
    static std::string PointToString( MT_Vector2D* p1 );
    static std::string DirectionToString( MT_Vector2D* p1 );
    static std::string PathToString( TER_Path_ABC* pPath );
    static void BreakForDebug( DEC_Decision_ABC* actor, const std::string& message );

    // DIA user types list manipulation
    static boost::shared_ptr< MT_Vector2D > ListPoint_GetAt( std::vector< MT_Vector2D* > list, int nId );
    static int ListPoint_Size( std::vector< MT_Vector2D* > list );

    static int GetRandomValue( int min, int max );

    // Time management
    static float GetSimTime();
    static float GetRealTime();
    static bool  IsNight();

    // Parameters copy
    static void CopyPoint( MT_Vector2D* pPosSource, boost::shared_ptr< MT_Vector2D > pPosDest );
    static void CopyPointMission( MT_Vector2D* pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );
    static void CopyPathMission( std::vector< boost::shared_ptr< MT_Vector2D > > pointList, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );
    static void CopyPointToListPointMission( boost::shared_ptr< MT_Vector2D > pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );
    static void CopyLocalisation( const TER_Localisation* pLocSource, TER_Localisation* pLocDest );
    static void CopyLocalisationMission( boost::shared_ptr< TER_Localisation > pLocation, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );
    static void CopyLocalisationListMission( const std::vector< boost::shared_ptr< TER_Localisation > >& locSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );
    static void CopyLocalisationToLocationListMission( boost::shared_ptr< TER_Localisation > pLocSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );
    static void CopyKnowledgeObjectToKnowledgeObjectListMission( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObjectSource );
    static void CopyGenObjectToGenObjectListMission( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Gen_Object > pGenObjectSource );
};

#endif // __DEC_DIAFunctions_h_
