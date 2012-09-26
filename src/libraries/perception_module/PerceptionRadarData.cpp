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
#include "wrapper/Hook.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind.hpp>

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
PerceptionRadarData::PerceptionRadarData( const wrapper::View& entity, wrapper::Effect& effect, const RadarType& radarType )
    : pRadarType_( &radarType )
    , effect_    ( effect )
{
    GetRadar( entity, radarType ).VisitIdentifiedChildren( boost::bind( &PerceptionRadarData::AddData, this, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData constructor
// Created: SLI 2012-08-31
// -----------------------------------------------------------------------------
PerceptionRadarData::PerceptionRadarData( wrapper::Effect& effect, const RadarType& radarType )
    : pRadarType_( &radarType )
    , effect_    ( effect )
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
            T_AgentAcquisitionMap::iterator agentData = acquisitionData_.find( identifier );
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
void PerceptionRadarData::Update( const wrapper::View& model, PerceptionObserver_ABC& observer, wrapper::Node& effect )
{
    assert( pRadarType_ );
    for( T_AgentAcquisitionMap::iterator itAcquisitionData = acquisitionData_.begin(); itAcquisitionData != acquisitionData_.end(); )
    {
        sAcquisitionData& data      =  itAcquisitionData->second;
        const wrapper::View& target = model[ "entities" ][ itAcquisitionData->first ];
        if( !data.bUpdated_ )
        {
            effect[ itAcquisitionData->first ].MarkForRemove();
            itAcquisitionData = acquisitionData_.erase( itAcquisitionData );
            continue;
        }
        observer.NotifyAgentPerception( target, pRadarType_->ComputeAcquisitionLevel( target, data.nFirstTimeStepPerceived_, model[ "tick" ] ) );
        data.bUpdated_ = false;
        ++itAcquisitionData;
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRadarData::Acquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PerceptionRadarData::Acquire( const wrapper::View& model, const wrapper::View& perceiver, PerceptionObserver_ABC& observer, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition )
{
    wrapper::Node effect = effect_[ pRadarType_->GetName() ];
    assert( pRadarType_ );
    Perception_ABC::T_AgentPtrVector targets;
    for( T_ZoneSet::const_iterator itZone = zones.begin(); itZone != zones.end(); ++itZone )
    {
        targets.clear();
        ListInCircleVisitor< wrapper::View > agentVisitor( targets );
        GET_HOOK( GetAgentListWithinLocalisation )( model, *itZone, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
        AcquireTargets( model, perceiver, targets, effect );
    }

    if( bAcquireOnPerceiverPosition )
    {
        targets.clear();
        ListInCircleVisitor< wrapper::View > agentVisitor( targets );
        GET_HOOK( GetAgentListWithinCircle )( model, MT_Vector2D( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] ), pRadarType_->GetRadius(), &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
        AcquireTargets( model, perceiver, targets, effect );
    }

    Update( model, observer, effect );
}
