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

#include "MIL.h"
#include <boost/shared_ptr.hpp>

class MIL_AgentPion;
class MIL_Automate;
class MT_Vector2D;
class DEC_Agent_Path;
class DEC_Knowledge_Object;
class DEC_PathResult;
class DEC_Path_ABC;
class DEC_PathPoint;
class DEC_PathType;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_PathFunctions
{
public:
    //! @name Functions
    //@{
    static boost::shared_ptr< DEC_Path_ABC > CreatePathToPointBM( MIL_AgentPion& callerAgent, float x, float y, float z, int pathType );
    static boost::shared_ptr< DEC_Path_ABC > CreatePathToPoint( MIL_AgentPion& callerAgent, MT_Vector2D* pEnd, int pathType );

    static boost::shared_ptr< DEC_Path_ABC > CreatePathToPointList( MIL_AgentPion& callerAgent, std::vector< boost::shared_ptr< MT_Vector2D > > listPt, int pathType );

    static int GetPathState          ( MIL_AgentPion& callerAgent, DEC_Path_ABC* pPath );
    static std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPath( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > oId, float oDistance, const std::vector< std::string >& params );
    static boost::shared_ptr< MT_Vector2D > ExtrapolatePosition   ( const MIL_AgentPion& callerAgent, const MT_Float time, bool bBoundOnPath );
    static boost::shared_ptr< MT_Vector2D > GetLastPointOfPath    ( const MIL_AgentPion& callerAgent, const DEC_Path_ABC* pPath );
    static bool IsMovingOnPath        ( const MIL_AgentPion& callerAgent, const DEC_Path_ABC* pPath );
    static MT_Vector2D* GetRepPoint   ( DEC_PathPoint* pPoint );
    static bool IsAvantPoint          ( DEC_PathPoint* pPoint );
    static bool IsPoint               ( DEC_PathPoint* pPoint );
    static int  GetTypePoint          ( DEC_PathPoint* pPoint );
    static DEC_PathPoint* GetDestPoint( DEC_PathPoint* pPoint );
    static int  GetTypeLimaPoint      ( DEC_PathPoint* pPoint );
    static unsigned int GetLimaPoint  ( DEC_PathPoint* pPoint );
    //@}
};

#endif // __DEC_PathFunctions_h_
