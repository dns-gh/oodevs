// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_FrontAndBackLinesComputer.cpp $
// $Author: Nld $
// $Modtime: 21/10/04 13:41 $
// $Revision: 2 $
// $Workfile: DEC_FrontAndBackLinesComputer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_FrontAndBackLinesComputer.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer constructor
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
DEC_FrontAndBackLinesComputer::DEC_FrontAndBackLinesComputer( const MIL_Automate& caller, const std::vector< MIL_AgentPion*>& pions )
    : refAutomate_      ( caller )
    , nLastTimeComputed_( 0 )
    , pions_            ( pions )
    , automates_        ()
    , backLineDroite_   ()
    , frontLineDroite_  ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer constructor
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
DEC_FrontAndBackLinesComputer::DEC_FrontAndBackLinesComputer( const MIL_Automate& caller, const std::vector< MIL_Automate*>& automats )
    : refAutomate_      ( caller )
    , nLastTimeComputed_( 0 )
    , pions_            ()
    , automates_        ( automats )
    , backLineDroite_   ()
    , frontLineDroite_  ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer destructor
// Created: NLD 2004-10-19
// -----------------------------------------------------------------------------
DEC_FrontAndBackLinesComputer::~DEC_FrontAndBackLinesComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::Compute
// Created: NLD 2004-10-19
// -----------------------------------------------------------------------------
void DEC_FrontAndBackLinesComputer::Compute()
{
    uint nCurrentTime = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    assert( nLastTimeComputed_ <=  nCurrentTime );
    if( nLastTimeComputed_ == nCurrentTime )
        return;

    nLastTimeComputed_ = nCurrentTime;
    
    if( refAutomate_.GetOrderManager().GetFuseau().IsNull() || ( pions_.empty() && automates_.empty() ) )
        return;

    const MT_Line& fuseauGlobalDirLine = refAutomate_.GetOrderManager().GetFuseau().GetGlobalDirection();

    // Vecteur perpendiculaire direction globale du fuseau
    MT_Vector2D vDirPerpendicularFuseau( fuseauGlobalDirLine.GetPosEnd() - fuseauGlobalDirLine.GetPosStart() );
    vDirPerpendicularFuseau.Normalize();
    MT_Float rTmp                 = vDirPerpendicularFuseau.rX_;
    vDirPerpendicularFuseau.rX_   = vDirPerpendicularFuseau.rY_;
    vDirPerpendicularFuseau.rY_   = -rTmp; 

    TER_DistanceLess cmp ( fuseauGlobalDirLine.GetPosStart() );
    T_PointSet projectedPointSet( cmp );
                     
    // Project all the positions on the fuseau global direction
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
    {
        if( !(**it).IsDead() )
            projectedPointSet.insert( fuseauGlobalDirLine.ClosestPointOnLine( (**it).GetRole< PHY_RoleInterface_Location >().GetPosition() ) );
    }
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
    {
        MT_Vector2D barycenter;
        if( (**it).GetAlivePionsBarycenter( barycenter ) )
            projectedPointSet.insert( barycenter );
    }

    if( projectedPointSet.empty() )
    {
        backLineDroite_  = MT_Droite();
        frontLineDroite_ = MT_Droite();
    }
    else
    {
        const MT_Vector2D& vBackLinePoint_  = *projectedPointSet.begin ();
        const MT_Vector2D& vFrontLinePoint_ = *projectedPointSet.rbegin();   

        backLineDroite_  = MT_Droite( vBackLinePoint_ , vBackLinePoint_  + vDirPerpendicularFuseau );
        frontLineDroite_ = MT_Droite( vFrontLinePoint_, vFrontLinePoint_ + vDirPerpendicularFuseau );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::GetDistanceFromFrontLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
MT_Float DEC_FrontAndBackLinesComputer::ComputeDistanceFromFrontLine( const MT_Vector2D& vPoint )
{
    Compute();

    const MT_Float          rDistFromFrontLine = frontLineDroite_.GetDistanceToPoint( vPoint );
    const MT_Droite::E_Side nFrontLineSide     = frontLineDroite_.GetSide( vPoint );

    // Front line behind
    if( nFrontLineSide == MT_Droite::eOnNegativeSide )
        return -rDistFromFrontLine;

    // Front line ahead
    else // MT_Droite::eOnPositiveSide || MT_Droite::eOnBoundary
        return rDistFromFrontLine;
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::GetDistanceFromBackLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
MT_Float DEC_FrontAndBackLinesComputer::ComputeDistanceFromBackLine( const MT_Vector2D& vPoint )
{
    Compute();

    const MT_Float          rDistFromBackLine  = backLineDroite_ .GetDistanceToPoint( vPoint );
    const MT_Droite::E_Side nBackLineSide      = backLineDroite_ .GetSide( vPoint );

    // Back line behind
    if( nBackLineSide == MT_Droite::eOnNegativeSide )
        return -rDistFromBackLine;

    // Back line ahead
    else // MT_Droite::eOnPositiveSide || MT_Droite::eOnBoundary
        return rDistFromBackLine;
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::UsedByDIA
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
void DEC_FrontAndBackLinesComputer::UsedByDIA()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::ReleasedByDIA
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
void DEC_FrontAndBackLinesComputer::ReleasedByDIA()
{
    delete this;
}
