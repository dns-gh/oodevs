// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionRadarData.h"
#include "RadarType.h"
#include "RadarClass.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Remove.h"
#include "wrapper/Hook.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind.hpp>
#include <cassert>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const SWORD_Model* localization,
                                                      void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
DECLARE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )

namespace
{
    wrapper::View GetRadar( const wrapper::View& entity, const RadarType& radarType )
    {
        return entity[ "perceptions/radars/acquisitions" ][ radarType.GetName() ];
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PerceptionRadarData::PerceptionRadarData( const wrapper::View& entity, const RadarType& radarType )
    : pRadarType_( &radarType )
{
    GetRadar( entity, radarType ).VisitIdentifiedChildren( boost::bind( &PerceptionRadarData::AddData, this, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData constructor
// Created: SLI 2012-08-31
// -----------------------------------------------------------------------------
PerceptionRadarData::PerceptionRadarData( const RadarType& radarType )
    : pRadarType_( &radarType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PerceptionRadarData::~PerceptionRadarData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData::AddData
// Created: SLI 2012-08-30
// -----------------------------------------------------------------------------
void PerceptionRadarData::AddData( const wrapper::View& acquisition )
{
    sAcquisitionData& agentData = acquisitionData_[ acquisition[ "identifier" ] ];
    agentData.bUpdated_ = false;
    agentData.nFirstTimeStepPerceived_ = acquisition[ "first-step" ];
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData::AcquireTargets
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PerceptionRadarData::AcquireTargets( const wrapper::View& model, const wrapper::View& perceiver, Perception_ABC::T_AgentPtrVector& targets, wrapper::Node& effect )
{
    const unsigned int currentTick = model[ "tick" ];
    for( Perception_ABC::T_AgentPtrVector::const_iterator it = targets.begin(); it != targets.end(); ++it )
    {
        const wrapper::View& target = *it;
        if( GET_HOOK( CanBeSeen )( perceiver, target ) && pRadarType_->CanAcquire( perceiver, target ) )
        {
            const std::size_t& identifier = target[ "identifier" ];
            auto agentData = acquisitionData_.find( identifier );
            if( agentData == acquisitionData_.end() )
            {
                effect[ identifier ][ "identifier" ] = identifier;
                effect[ identifier ][ "first-step" ] = currentTick;
                agentData = acquisitionData_.insert( std::make_pair( identifier, sAcquisitionData() ) ).first;
                agentData->second.nFirstTimeStepPerceived_ = currentTick;
            }
            agentData->second.bUpdated_ = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData::Update
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PerceptionRadarData::Update( const wrapper::View& model, const wrapper::View& acquisitions, PerceptionObserver_ABC& observer )
{
    assert( pRadarType_ );
    for( auto it = acquisitionData_.begin(); it != acquisitionData_.end(); )
    {
        sAcquisitionData& data      =  it->second;
        const wrapper::View& target = model[ "entities" ][ it->first ];
        if( !data.bUpdated_ )
        {
            wrapper::Remove( acquisitions[ pRadarType_->GetName() ][ it->first ] ).Post();
            it = acquisitionData_.erase( it );
            continue;
        }
        observer.NotifyAgentPerception( target, pRadarType_->ComputeAcquisitionLevel( target, data.nFirstTimeStepPerceived_, model[ "tick" ] ) );
        data.bUpdated_ = false;
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData::Acquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PerceptionRadarData::Acquire( const wrapper::View& model, const wrapper::View& perceiver, wrapper::Effect& effect,
    PerceptionObserver_ABC& observer, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition )
{
    wrapper::Node node = effect[ pRadarType_->GetName() ];
    assert( pRadarType_ );
    Perception_ABC::T_AgentPtrVector targets;
    for( T_ZoneSet::const_iterator itZone = zones.begin(); itZone != zones.end(); ++itZone )
    {
        targets.clear();
        ListInCircleVisitor agentVisitor( targets );
        GET_HOOK( GetAgentListWithinLocalisation )( model, *itZone, &ListInCircleVisitor::Add, &agentVisitor );
        AcquireTargets( model, perceiver, targets, node );
    }

    if( bAcquireOnPerceiverPosition )
    {
        targets.clear();
        ListInCircleVisitor agentVisitor( targets );
        GET_HOOK( GetAgentListWithinCircle )( model, MT_Vector2D( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] ), pRadarType_->GetRadius(), &ListInCircleVisitor::Add, &agentVisitor );
        AcquireTargets( model, perceiver, targets, node );
    }

    Update( model, perceiver[ "perceptions/radars/acquisitions" ], observer );
}
