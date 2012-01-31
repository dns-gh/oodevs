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
void DEC_Path::DoExecute( TerrainPathfinder& pathfind )
{
    unsigned int nComputationEndTime = 0;
    const unsigned int nMaxComputationDuration = MIL_AgentServer::GetWorkspace().GetPathFindManager().GetMaxComputationDuration();
    if( nMaxComputationDuration == std::numeric_limits< unsigned int >::max() )
        nComputationEndTime = std::numeric_limits< unsigned int >::max();
    else
        nComputationEndTime = static_cast< unsigned int >( time( 0 ) ) + nMaxComputationDuration;

    if( pathSections_.empty() )
        throw std::runtime_error( "List of path sections is empty" );
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
// Name: DEC_Path::Execute
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path::Execute( TerrainPathfinder& pathfind )
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
