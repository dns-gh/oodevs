//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.cpp $
// $Author: Age $
// $Modtime: 4/03/05 13:48 $
// $Revision: 3 $
// $Workfile: DEC_Path.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path.h"
#include "DEC_PathSection_ABC.h"
#include "DEC_Pathfind_Manager.h"
#include "DEC_PathPoint.h"
#include "DEC_PathResult.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path::DEC_Path()
    : nState_      ( eComputing )
    , bJobCanceled_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path::~DEC_Path()
{
    CleanAfterComputation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::CleanAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_Path::CleanAfterComputation()
{
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        delete *itPathSection;
    pathSections_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::Cancel
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path::Cancel()
{
    bJobCanceled_ = true;
    nState_ = eCanceled;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        ( *itPathSection )->Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double DEC_Path::GetLength() const
{
    double rLength = 0.;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        rLength += ( *itPathSection )->GetLength();
    return rLength;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::DoExecute
// Created: AGE 2006-03-07
// -----------------------------------------------------------------------------
void DEC_Path::DoExecute( TER_Pathfinder_ABC& pathfind )
{
    unsigned int nComputationEndTime = 0;
    const unsigned int nMaxComputationDuration = MIL_AgentServer::GetWorkspace().GetPathFindManager().GetMaxComputationDuration();
    if( nMaxComputationDuration == std::numeric_limits< unsigned int >::max() )
        nComputationEndTime = std::numeric_limits< unsigned int >::max();
    else
        nComputationEndTime = static_cast< unsigned int >( time( 0 ) ) + nMaxComputationDuration;

    if( pathSections_.empty() )
        throw MASA_EXCEPTION( "List of path sections is empty" );
    const auto& pathPoints = dynamic_cast< const DEC_PathResult* >( &pathSections_.front()->GetPath() )->GetResult( false );

    lastWaypoint_ = pathSections_.back()->GetPosEnd();
    computedWaypoints_.clear();
    nState_ = eComputing;
    for( auto itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
    {
        if( bJobCanceled_ )
        {
            nState_ = eCanceled;
            return;
        }
        DEC_PathSection_ABC& pathSection = **itPathSection;
        if( !pathSection.Execute( pathfind, nComputationEndTime ) )
        {
            if( !pathPoints.empty() )
                computedWaypoints_.push_back( pathPoints.back()->GetPos() );

            if( bJobCanceled_ )
            {
                nState_ = eCanceled;
                return;
            }
            else if( itPathSection == pathSections_.begin() && pathSection.IsImpossible() )
            {
                nState_ = eImpossible;
                return;
            }
            else
            {
                NotifyPartialSection();
                T_PathSectionVector::iterator itNextPathSection = itPathSection + 1;
                if( itNextPathSection == pathSections_.end() )
                {
                    nState_ = ePartial;
                    return;
                }
                else
                    (*itNextPathSection)->SetPosStart( pathPoints.back()->GetPos() );
            }
        }
        else if( !pathPoints.empty() )
            computedWaypoints_.push_back( pathPoints.back()->GetPos() );
        NotifySectionEnded();
    }
    nState_ = eValid;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::Execute
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path::Execute( TER_Pathfinder_ABC& pathfind )
{
    try
    {
        DoExecute( pathfind );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "DEC_Path::Execute failed: " << e.what() );
        bJobCanceled_ = true;
        nState_ = eCanceled;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetStateAsString
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
std::string DEC_Path::GetStateAsString() const
{
    switch( nState_ )
    {
        case eInvalid    : return "Invalid";
        case eComputing  : return "Computing";
        case eValid      : return "Valid";
        case eImpossible : return "Impossible";
        case ePartial    : return "Partial";
        case eCanceled   : return "Canceled";
        default          : return "UNKNOWN";
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetPathAsString
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
std::string DEC_Path::GetPathAsString() const
{
    std::stringstream strTmp;
    strTmp << "   Path points : " << pathSections_.front()->GetPosStart();
    for( CIT_PathSectionVector itSection = pathSections_.begin(); itSection != pathSections_.end(); ++itSection )
        strTmp << " -> " << ( *itSection )->GetPosEnd();
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::RemoveComputedWaypoint
// Created: CMA 2012-04-26
// -----------------------------------------------------------------------------
void DEC_Path::RemoveComputedWaypoint()
{
    if( !computedWaypoints_.empty() )
        computedWaypoints_.erase( computedWaypoints_.begin() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::GetState
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
DEC_Path_ABC::E_State DEC_Path::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetComputedWaypoints
// Created: CMA 2012-02-23
// -----------------------------------------------------------------------------
const T_PointVector& DEC_Path::GetComputedWaypoints() const
{
    return computedWaypoints_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetLastWaypoint
// Created: CMA 2012-05-21
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Path::GetLastWaypoint() const
{
    return lastWaypoint_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::RegisterPathSection
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path::RegisterPathSection( DEC_PathSection_ABC& section )
{
    pathSections_.push_back( &section );
}
