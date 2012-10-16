// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionRecoPoint.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
DECLARE_HOOK( GetObjectListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* object, void* userData ), void* userData ) )
DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( IsKnowledgeObjectIntersectingWithCircle, bool, ( const MT_Vector2D* center, double radius, const SWORD_Model* knowledgeObject ) )

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPointReco::PerceptionRecoPointReco
// Created: SLI 2012-08-23
// -----------------------------------------------------------------------------
PerceptionRecoPointReco::PerceptionRecoPointReco( const wrapper::View& perception, const wrapper::View& entity )
    : vCenter_     ( perception[ "center/x" ], perception[ "center/y" ] )
    , rCurrentSize_( perception[ "radius" ] )
    , rFinalSize_  ( perception[ "max-radius" ] )
    , rGrowthSpeed_( perception[ "growth-speed" ] )
    , bProcessed_  ( perception[ "max-radius-reached" ] )
{
    assert( rGrowthSpeed_ > 0.f );
    assert( rFinalSize_ > 0.f );

    if( !bProcessed_ )
    {
        // Agrandissement de la zone de reconnaissance
        if( rCurrentSize_ < rFinalSize_ )
        {
            rCurrentSize_ += rGrowthSpeed_;
            if( rCurrentSize_ >= rFinalSize_ )
            {
                rCurrentSize_ = rFinalSize_;
                wrapper::Effect effect( perception[ "max-radius-reached" ] );
                effect = true;
                effect.Post();
                wrapper::Event event( "perception callback" );
                event[ "entity/data" ] = entity[ "data" ];
                event[ "perception" ] = static_cast< std::size_t >( perception[ "perception-id" ] );
                event.Post();
                bProcessed_ = true;
            }
            wrapper::Effect effect( perception[ "radius" ] );
            effect = rCurrentSize_;
            effect.Post();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::PerceptionRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoPoint::PerceptionRecoPoint( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
{
    entity[ "perceptions/recognition-point" ].VisitIdentifiedChildren( boost::bind( &PerceptionRecoPoint::AddLocalisation, this, _2, boost::cref( entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::~PerceptionRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoPoint::~PerceptionRecoPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::AddLocalisation
// Created: SLI 2012-08-23
// -----------------------------------------------------------------------------
void PerceptionRecoPoint::AddLocalisation( const wrapper::View& perception, const wrapper::View& entity )
{
    Add( std::auto_ptr< PerceptionRecoPointReco >( new PerceptionRecoPointReco( perception, entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::ComputePoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoPoint::ComputePoint( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const MT_Vector2D& vPoint ) const
{
    for( T_RecoVector::const_iterator it = recos_.begin(); it != recos_.end(); ++it )
        if( (*it)->vCenter_.SquareDistance( vPoint ) <= (*it)->rCurrentSize_ * (*it)->rCurrentSize_ )
            return PerceptionLevel::recognized_;
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::ExecuteAgents
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionRecoPoint::ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    T_AgentPtrVector perceivableAgents;
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableAgents.clear();
        ListInCircleVisitor agentVisitor( perceivableAgents );
        GET_HOOK( GetAgentListWithinCircle )( model, (*itReco)->vCenter_, (*itReco)->rCurrentSize_, &ListInCircleVisitor::Add, &agentVisitor );
        for( T_AgentPtrVector::const_iterator it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            const wrapper::View& agent = *it;
            if( GET_HOOK( CanBeSeen )( perceiver, agent ) )
                observer_.NotifyAgentPerception( agent, PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::ComputeAgent
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoPoint::ComputeAgent( const wrapper::View& /*model*/, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{
    return ComputePoint( perceiver, surfaces, MT_Vector2D( target[ "movement/position/x" ], target[ "movement/position/y" ] ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoPoint::ComputeObject( const wrapper::View& /*perceiver*/, const SurfacesObject_ABC& /*surfaces*/, const wrapper::View& knowledge ) const
{
    for( T_RecoVector::const_iterator it = recos_.begin(); it != recos_.end(); ++it )
        if( GET_HOOK( IsKnowledgeObjectIntersectingWithCircle )( &(*it)->vCenter_, (*it)->rCurrentSize_, knowledge ) )
            return PerceptionLevel::recognized_;
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoPoint::ExecuteObjects
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionRecoPoint::ExecuteObjects( const wrapper::View& model, const wrapper::View& /*perceiver*/, const SurfacesObject_ABC& /*surfaces*/, const T_ObjectVector& /*perceivableObjects*/ )
{
    T_ObjectVector perceivableObjects;
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableObjects.clear();
        ListInCircleVisitor objectVisitor( perceivableObjects );
        GET_HOOK( GetObjectListWithinCircle )( model, (*itReco)->vCenter_, (*itReco)->rCurrentSize_, &ListInCircleVisitor::Add, &objectVisitor );
        for( T_ObjectVector::const_iterator it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
        {
            const wrapper::View& object = *it;
            if( object[ "can-be-perceived" ] )
                observer_.NotifyObjectPerception( object, PerceptionLevel::identified_ ); // Identifié ou not seen pour les objets
        }
    }
}
