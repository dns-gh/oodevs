// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PopulationContext.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_Path_KnowledgeObjectFlood.h"
#include "DEC_Path_KnowledgeObjectBurnSurface.h"
#include "Decision/DEC_Population_PathClass.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/Objects/FloodCapacity.h"
#include "Entities/Objects/BurnSurfaceCapacity.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include <boost/make_shared.hpp>

DEC_PopulationContext::DEC_PopulationContext( const MIL_Population& population, const T_PointVector& points )
    : pathClass_( DEC_Population_PathClass::GetPathClass( "base" ) ) //$$$ n'importe quoi
    , type_( population.GetType() )
    , rCostOutsideOfAllObjects_( 0 )
{
    const auto& channelingLocations = population.GetKnowledge().GetChannelingLocations();
    channelers_.reserve( channelingLocations.size() );
    for( auto it = channelingLocations.begin(); it != channelingLocations.end(); ++it )
        channelers_.push_back( DEC_Population_Path_Channeler( pathClass_, *it ) );
    InitializePathKnowledges( population, points );
}

void DEC_PopulationContext::InitializePathKnowledges( const MIL_Population& population, const T_PointVector& points )
{
    // Objects
    if( !pathClass_.AvoidObjects() )
        return;
    T_KnowledgeObjectVector knowledgesObject;
    auto knowledges = population.GetArmy().GetKnowledgeGroups();
    for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
    {
        if( it->second->IsJammed() )
            continue;
        auto knowledge = it->second->GetKnowledge();
        if( !knowledge )
            continue;
        T_KnowledgeObjectVector knowledgesObjectTmp;
        knowledge->GetKnowledgeObjectContainer().GetObjects( knowledgesObjectTmp );
        knowledgesObject.insert( knowledgesObject.end(), knowledgesObjectTmp.begin(), knowledgesObjectTmp.end() );
    }
    T_PointVector firstPointVector;
    if( !points.empty() )
        firstPointVector.push_back( *points.begin() );
    for( auto itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
    {
        const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
        if( !knowledge.CanCollideWithEntity() )
            continue;
        if( knowledge.IsObjectInsidePathPoint( firstPointVector, 0 ) )
        {
            double rMaxSpeed = pathClass_.GetObjectCost( knowledge.GetType() );
            if( rMaxSpeed <= 0. || rMaxSpeed == std::numeric_limits< double >::max() || rMaxSpeed >= pathClass_.GetThreshold() )
                continue;
        }
        if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
            pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );
        if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
            throw MASA_EXCEPTION( "Size of path knowledge objects list is invalid" );

        auto& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
        if( knowledge.GetType().GetCapacity< FloodCapacity >() )
            pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectFlood >( eCrossingHeightNever, knowledge ) );
        else if( knowledge.GetType().GetCapacity< BurnSurfaceCapacity >() )
            pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectBurnSurface >( knowledge ) );
        else
            pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObject >( pathClass_, knowledge ) );
        if( pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
            rCostOutsideOfAllObjects_ += pathKnowledges.front()->GetCostOut();
    }
}

const DEC_PopulationContext::T_PopulationPathChannelers& DEC_PopulationContext::GetChannelers() const
{
    return channelers_;
}

double DEC_PopulationContext::GetCostOutsideOfChanneling() const
{
    return pathClass_.GetCostOutsideOfChanneling();
}

double DEC_PopulationContext::GetCostOutsideOfAllObjects() const
{
    return rCostOutsideOfAllObjects_;
}

const DEC_PopulationContext::T_PathKnowledgeObjectsByTypes& DEC_PopulationContext::GetPathKnowledgeObjects() const
{
    return pathKnowledgeObjects_;
}

double DEC_PopulationContext::GetMaxSpeed() const
{
    return type_.GetMaxSpeed();
}

double DEC_PopulationContext::GetMaxSpeed( const TerrainData& terrainData ) const
{
    return type_.GetMaxSpeed( terrainData );
}
