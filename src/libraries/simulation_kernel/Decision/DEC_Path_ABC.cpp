//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.cpp $
// $Author: Age $
// $Modtime: 4/03/05 13:48 $
// $Revision: 3 $
// $Workfile: DEC_Path_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_ABC.h"
#include "DEC_PathSection_ABC.h"
#include "DEC_Pathfind_Manager.h"
#include "DEC_PathPoint.h"
#include "DEC_PathResult.h"
#include "MIL_AgentServer.h"

unsigned int DEC_Path_ABC::nIDIdx_ = 0;

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::DEC_Path_ABC()
    : nID_         ( ++ nIDIdx_ )
    , nState_      ( eComputing )
    , bJobCanceled_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::~DEC_Path_ABC()
{
    CleanAfterComputation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::CleanAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_Path_ABC::CleanAfterComputation()
{
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        delete *itPathSection;
    pathSections_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::Cancel
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path_ABC::Cancel()
{
    bJobCanceled_ = true;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        ( *itPathSection )->Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double DEC_Path_ABC::GetLength() const
{
    double rLength = 0.;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        rLength += ( *itPathSection )->GetLength();
    return rLength;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::DoExecute
// Created: AGE 2006-03-07
// -----------------------------------------------------------------------------
void DEC_Path_ABC::DoExecute( TerrainPathfinder& pathfind )
{
    unsigned int nComputationEndTime = 0;
    const unsigned int nMaxComputationDuration = MIL_AgentServer::GetWorkspace().GetPathFindManager().GetMaxComputationDuration();
    if( nMaxComputationDuration == std::numeric_limits< unsigned int >::max() )
        nComputationEndTime = std::numeric_limits< unsigned int >::max();
    else
        nComputationEndTime = static_cast< unsigned int >( time( 0 ) ) + nMaxComputationDuration;

    assert( !pathSections_.empty() );
    const DEC_PathResult::T_PathPointList& pathPoints = dynamic_cast< const DEC_PathResult* >( &pathSections_.front()->GetPath() )->GetResult( false );

    nState_ = eComputing;
    for( T_PathSectionVector::iterator itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
    {
        if( bJobCanceled_ )
        {
            nState_ = eCanceled;
            return;
        }
        DEC_PathSection_ABC& pathSection = **itPathSection;
        if( !pathSection.Execute( pathfind, nComputationEndTime ) )
        {
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
        NotifySectionEnded();
    }
    nState_ = eValid;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::Execute
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path_ABC::Execute( TerrainPathfinder& pathfind )
{
    try
    {
        DoExecute( pathfind );
    }
    catch( std::runtime_error& )
    {
        bJobCanceled_ = true;
        nState_ = eCanceled;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetStateAsString
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
std::string DEC_Path_ABC::GetStateAsString() const
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
// Name: DEC_Path_ABC::GetPathAsString
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
std::string DEC_Path_ABC::GetPathAsString() const
{
    std::stringstream strTmp;
    strTmp << "   Path points : " << pathSections_.front()->GetPosStart();
    for( CIT_PathSectionVector itSection = pathSections_.begin(); itSection != pathSections_.end(); ++itSection )
        strTmp << " -> " << ( *itSection )->GetPosEnd();
    return strTmp.str();
}
