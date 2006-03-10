//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_Path.cpp $
// $Author: Age $
// $Modtime: 16/06/05 15:23 $
// $Revision: 28 $
// $Workfile: DEC_Population_Path.cpp $
// 
//*****************************************************************************

#include "MIL_pch.h"

#include "DEC_Population_Path.h"

#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Decision/Path/Population/DEC_Population_PathSection.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "MIL_AgentServer.h"

//-----------------------------------------------------------------------------
// Name: DEC_Population_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Population_Path::DEC_Population_Path( const MIL_PopulationFlow& flow, const MT_Vector2D& destination ) 
    : flow_    ( flow )
    , profiler_()
{
    T_PointVector pointsTmp;
    pointsTmp.push_back( flow_.GetPosition() ); // Head position
    pointsTmp.push_back( destination );
    Initialize( pointsTmp );
}

//-----------------------------------------------------------------------------
// Name: DEC_Population_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Population_Path::~DEC_Population_Path()
{
}

//=============================================================================
// INIT
//=============================================================================

 // -----------------------------------------------------------------------------
// Name: DEC_Population_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Population_Path::Initialize( const T_PointVector& points )
{
    // initialize channeling points
    const DEC_PopulationKnowledge::T_LocationVector& channelingLocations = flow_.GetPopulation().GetKnowledge().GetChannelingLocations();
    for( DEC_PopulationKnowledge::CIT_LocationVector itChanLocation = channelingLocations.begin(); 
        itChanLocation != channelingLocations.end(); ++itChanLocation )
        channelers_.push_back( DEC_Population_Path_Channeler( *itChanLocation ) );

    assert( !points.empty() );
    const MT_Vector2D* pLastPoint = 0;
    for( CIT_PointVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        if( pLastPoint )
        {
            DEC_Population_PathSection* pSection = new DEC_Population_PathSection( *this, *pLastPoint, *itPoint );
            RegisterPathSection( *pSection );
        }
        pLastPoint = &*itPoint;
    }
}

// =============================================================================
// PATH CALCULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::CleanAfterComputation
// Created: NLD 2006-01-24
// -----------------------------------------------------------------------------
void DEC_Population_Path::CleanAfterComputation()
{
    DEC_Path_ABC::CleanAfterComputation(); 
    channelers_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::Execute
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
void DEC_Population_Path::Execute( TerrainPathfinder& pathfind )
{    
    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        MT_LOG_MESSAGE_MSG( "DEC_Population_Path::Compute: " << this << " : computation begin" );
        MT_LOG_MESSAGE_MSG( "   Thread          : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() );
        MT_LOG_MESSAGE_MSG( "   Flux population : " << flow_.GetID() );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }

    assert( resultList_.empty() );

    DEC_Path_ABC::Execute( pathfind );

    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        double rComputationTime = profiler_.Stop();

        std::stringstream stream;
        if ( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "DEC_Population_Path::Compute: " << this << 
                            ", Thread : "  << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Time : " << rComputationTime << 
                            ", State : " << GetStateAsString() <<
                            ", Result : " << stream.str() );
    }
    MIL_AgentServer::GetWorkspace().GetPathFindManager().CleanPathAfterComputation( *this );
    DecRef(); // We are no longer in the pathfind queue
}
