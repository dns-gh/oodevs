// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/decision/functions/DEC_PathFunctions.h $
// $Author: Jvt $
// $Modtime: 11/05/05 16:56 $
// $Revision: 4 $
// $Workfile: DEC_PathFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_PathFunctions_h_
#define __DEC_PathFunctions_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/shared_ptr.hpp>

class MIL_AgentPion;
class MIL_Agent_ABC;
class MT_Vector2D;
class DEC_Knowledge_Object;
class DEC_Path_ABC;
class DEC_PathPoint;
class DEC_Decision_ABC;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_PathFunctions
{
public:
    //! @name Functions
    //@{
    static boost::shared_ptr< DEC_Path_ABC > CreatePathToPointBM( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > end, int pathType );
    static boost::shared_ptr< DEC_Path_ABC > CreatePathToPoint( MIL_AgentPion& callerAgent, MT_Vector2D* pEnd, int pathType );
    static boost::shared_ptr< DEC_Path_ABC > CreatePathToPointList( MIL_AgentPion& callerAgent, std::vector< boost::shared_ptr< MT_Vector2D > > listPt, int pathType );
    static int GetPathState          ( MIL_AgentPion& callerAgent, DEC_Path_ABC* pPath );
    static std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPath( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > oId, float oDistance, const std::vector< std::string >& params );
    static std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPathWithBypassed( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > oId, float oDistance, const std::vector< std::string >& params );
    static std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextRemovableObjectOnPath( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > oId, float oDistance );
    static boost::shared_ptr< MT_Vector2D > ExtrapolatePosition   ( const MIL_AgentPion& callerAgent, const double time, bool bBoundOnPath );
    static boost::shared_ptr< MT_Vector2D > GetLastPointOfPath    ( const MIL_AgentPion& callerAgent, const DEC_Path_ABC* pPath );
    static boost::shared_ptr< MT_Vector2D > GetRepPoint           ( boost::shared_ptr< DEC_PathPoint > pPoint );
    static bool IsMovingOnPath        ( const MIL_AgentPion& callerAgent, const DEC_Path_ABC* pPath );
    static bool IsAvantPoint          ( boost::shared_ptr< DEC_PathPoint > pPoint );
    static bool IsPoint               ( boost::shared_ptr< DEC_PathPoint > pPoint );
    static int  GetTypePoint          ( boost::shared_ptr< DEC_PathPoint > pPoint );
    static boost::shared_ptr< DEC_PathPoint > GetDestPoint( boost::shared_ptr< DEC_PathPoint > pPoint );
    static int  GetTypeLimaPoint      ( boost::shared_ptr< DEC_PathPoint > pPoint );
    static unsigned int GetLimaPoint  ( boost::shared_ptr< DEC_PathPoint > pPoint );
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetClosestPath( DEC_Decision_ABC* callerAgent,
                                                                           const boost::shared_ptr< MT_Vector2D >& begin,
                                                                           const boost::shared_ptr< MT_Vector2D >& end );
     //@}
};

#endif // __DEC_PathFunctions_h_
