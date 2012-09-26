// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionRecoLocalisation.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "wrapper/Hook.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( IsPointInsideLocalisation, bool, ( const SWORD_Model* localisation, const MT_Vector2D* point ) )
DECLARE_HOOK( GetLocalizationRadius, double, ( const SWORD_Model* localization ) )
DECLARE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const SWORD_Model* localization,
                                                      void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
DECLARE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisationReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoLocalisationReco::PerceptionRecoLocalisationReco( const wrapper::View& perception, const wrapper::View& entity )
    : localisation_    ( perception[ "localization" ] )
    , bShouldUseRadius_( perception[ "has-growth-speed" ] )
    , rRadius_         ( GET_HOOK( GetLocalizationRadius )( localisation_ ) )
    , rCurrentRadius_  ( perception[ "radius" ] )
    , rGrowthSpeed_    ( perception[ "growth-speed" ] )
{
    if( perception[ "max-radius-reached" ] || !bShouldUseRadius_ )
        return;
    // Agrandissement de la zone de reconnaissance
    if( rCurrentRadius_ < rRadius_ )
        rCurrentRadius_ += rGrowthSpeed_;
    if( rCurrentRadius_ >= rRadius_ )
    {
        rCurrentRadius_ = rRadius_;
        wrapper::Effect effect( perception[ "max-radius-reached" ] );
        effect = true;
        effect.Post();
        wrapper::Event event( "perception callback" );
        event[ "entity" ] = static_cast< std::size_t >( entity[ "identifier" ] );
        event[ "perception" ] = static_cast< std::size_t >( perception[ "perception-id" ] );
        event.Post();
    }
    wrapper::Effect effect( perception[ "radius" ] );
    effect = rCurrentRadius_;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisationReco::IsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
bool PerceptionRecoLocalisationReco::IsInside( const wrapper::View& perceiver, const MT_Vector2D& point ) const
{
    if( bShouldUseRadius_ )
    {
        const double rRadius = rRadius_ < 0. ? perceiver[ "perceptions/max-agent-perception-distance" ] : rRadius_;
        if( MT_Vector2D( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] ).SquareDistance( point ) > rRadius * rRadius )
            return false;
    }
    return GET_HOOK( IsPointInsideLocalisation )( localisation_, &point );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisationReco::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void PerceptionRecoLocalisationReco::GetAgentsInside( const wrapper::View& model, const wrapper::View& perceiver, Perception_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    ListInCircleVisitor< wrapper::View > agentVisitor( result );
    if( bShouldUseRadius_ )
    {
        const double rRadius = rRadius_ < 0. ? perceiver[ "perceptions/max-agent-perception-distance" ] : rRadius_;
        GET_HOOK( GetAgentListWithinCircle )( model, MT_Vector2D( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] ), rRadius, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
        for( Perception_ABC::T_AgentPtrVector::iterator it = result.begin(); it != result.end(); )
        {
            const MT_Vector2D position( (*it)[ "movement/position/x" ], (*it)[ "movement/position/y" ] );
            if( GET_HOOK( IsPointInsideLocalisation )( localisation_, &position ) )
                ++it;
            else
                it = result.erase( it );
        }
    }
    else
        GET_HOOK( GetAgentListWithinLocalisation )( model, localisation_, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisation constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoLocalisation::PerceptionRecoLocalisation( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
{
    entity[ "perceptions/reco" ].VisitIntegerChildren( boost::bind( &PerceptionRecoLocalisation::AddLocalisation, this, _2, boost::cref( entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisation::~PerceptionRecoLocalisation
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoLocalisation::~PerceptionRecoLocalisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisation::AddLocalisation
// Created: SLI 2012-08-28
// -----------------------------------------------------------------------------
void PerceptionRecoLocalisation::AddLocalisation( const wrapper::View& perception, const wrapper::View& entity )
{
    Add( std::auto_ptr< PerceptionRecoLocalisationReco >( new PerceptionRecoLocalisationReco( perception, entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoLocalisation::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const MT_Vector2D& vPoint ) const
{
    for( T_RecoVector::const_iterator it = recos_.begin(); it != recos_.end(); ++it )
        if( (*it)->IsInside( perceiver, vPoint ) )
            return PerceptionLevel::recognized_;
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisation::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionRecoLocalisation::Execute( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    Perception_ABC::T_AgentPtrVector perceivableAgents;
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        (*itReco)->GetAgentsInside( model, perceiver, perceivableAgents );
        for( T_AgentPtrVector::const_iterator it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            const wrapper::View& agent = *it;
            if( GET_HOOK( CanBeSeen )( perceiver, agent ) )
                observer_.NotifyPerception( agent, PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoLocalisation::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{//@TODO move to  private
    const MT_Vector2D location( target[ "movement/position/x" ], target[ "movement/position/y" ] );
    return Compute( perceiver, surfaces, location );
}
