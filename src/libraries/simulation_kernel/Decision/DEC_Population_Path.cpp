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

#include "simulation_kernel_pch.h"
#include "DEC_Population_Path.h"
#include "DEC_Population_PathClass.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_Path_KnowledgeObjectFlood.h"
#include "DEC_Path_KnowledgeObjectBurnSurface.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Decision/DEC_Population_PathSection.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/FloodCapacity.h"
#include "Entities/Objects/BurnSurfaceCapacity.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"

//-----------------------------------------------------------------------------
// Name: DEC_Population_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Population_Path::DEC_Population_Path( const MIL_Population& population, const MT_Vector2D& start, const MT_Vector2D& destination )
    : DEC_PathResult( DEC_PathType::movement_ )
    , id_        ( population.GetID() )
    , pathClass_ ( DEC_Population_PathClass::GetPathClass( "base" ) ) //$$$ n'importe quoi
    , population_( population )
    , rCostOutsideOfAllObjects_( 0. )
    , profiler_()
{
    T_PointVector pointsTmp;
    pointsTmp.reserve( 2 );
    pointsTmp.push_back( start );
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
    // NOTHING
}

 // -----------------------------------------------------------------------------
// Name: DEC_Population_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Population_Path::Initialize( const T_PointVector& points )
{
    // initialize channeling points
    const DEC_PopulationKnowledge::T_LocationVector& channelingLocations = population_.GetKnowledge().GetChannelingLocations();
    channelers_.reserve( channelingLocations.size() );
    for( auto it = channelingLocations.begin(); it != channelingLocations.end(); ++it )
        channelers_.push_back( DEC_Population_Path_Channeler( pathClass_, *it ) );
    InitializePathKnowledges( points );
    if( points.empty() )
        throw MASA_EXCEPTION( "List of points is empty" );
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

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::InitializePathKnowledges
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
void DEC_Population_Path::InitializePathKnowledges( const T_PointVector& /*pathPoints*/ )
{
    // Objects
    if( pathClass_.AvoidObjects() )
    {
        T_KnowledgeObjectVector knowledgesObject;
        auto knowledges = population_.GetArmy().GetKnowledgeGroups();
        for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            if( it->second->IsJammed() )
                continue;
            auto knowledge = it->second->GetKnowledge();
            if( knowledge )
            {
                T_KnowledgeObjectVector knowledgesObjectTmp;
                knowledge->GetKnowledgeObjectContainer().GetObjects( knowledgesObjectTmp );
                knowledgesObject.insert( knowledgesObject.end(), knowledgesObjectTmp.begin(), knowledgesObjectTmp.end() );
            }
        }
        for( auto itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
            if( knowledge.CanCollideWithEntity() )
            {
                if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );
                if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    throw MASA_EXCEPTION( "Size of path knowledge objects list is invalid" );

                T_PathKnowledgeObjectVector& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
                if( knowledge.GetType().GetCapacity< FloodCapacity >() )
                    pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObjectFlood( eCrossingHeightNever, knowledge ) ) );
                else if( knowledge.GetType().GetCapacity< BurnSurfaceCapacity >() )
                    pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObjectBurnSurface( knowledge ) ) );
                else
                    pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObject( pathClass_, knowledge ) ) );
                if( pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
                    rCostOutsideOfAllObjects_ += pathKnowledges.front()->GetCostOut();
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::GetCostOutsideOfChanneling
// Created: NLD 2007-03-07
// -----------------------------------------------------------------------------
double DEC_Population_Path::GetCostOutsideOfChanneling() const
{
    return pathClass_.GetCostOutsideOfChanneling();
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::CleanAfterComputation
// Created: NLD 2006-01-24
// -----------------------------------------------------------------------------
void DEC_Population_Path::CleanAfterComputation()
{
    DEC_Path::CleanAfterComputation();
    channelers_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::IsPathForUnit
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
bool DEC_Population_Path::IsPathForUnit( MIL_Agent_ABC* /*pion*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::Execute
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
void DEC_Population_Path::Execute( TER_Pathfinder_ABC& pathfind )
{
    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        MT_LOG_MESSAGE_MSG( "DEC_Population_Path::Compute: " << this << " : computation begin" );
        MT_LOG_MESSAGE_MSG( "   Thread      : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() );
        MT_LOG_MESSAGE_MSG( "   Population  : " << population_.GetID() );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }
    if( !resultList_.empty() )
        throw MASA_EXCEPTION( "List of path points is not empty before running pathfind" );
    pathfind.SetId( id_ );
    DEC_Path::Execute( pathfind );
    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        double rComputationTime = profiler_.Stop();

        std::stringstream stream;
        for( auto it = resultList_.begin(); it != resultList_.end(); ++it )
            stream << "[" << (**it).GetPos() << "] ";
        MT_LOG_MESSAGE_MSG( "DEC_Population_Path::Compute: " << this <<
                            ", Thread : "  << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Time : " << rComputationTime <<
                            ", State : " << GetStateAsString() <<
                            ", Result : " << stream.str() );
    }
}
