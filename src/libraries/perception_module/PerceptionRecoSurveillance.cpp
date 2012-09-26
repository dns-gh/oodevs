// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionRecoSurveillance.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "VisionObject.h"
#include "wrapper/Hook.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( GetVisionObject, unsigned char, ( const MT_Vector2D* point ) )
DECLARE_HOOK( IsPointInsideLocalisation, bool, ( const SWORD_Model* localisation, const MT_Vector2D* point ) )
DECLARE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const SWORD_Model* localization,
                                                      void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillanceReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoSurveillanceReco::PerceptionRecoSurveillanceReco( const wrapper::View& perception, unsigned int currentTimeStep )
    : localisation_             ( perception[ "localization" ] )
    , currentTimeStep_          ( currentTimeStep )
    , nForestDetectionTimeStep_ ( perception[ "forest-detection-time-step" ] )
    , nUrbanDetectionTimeStep_  ( perception[ "urban-detection-time-step" ] )
    , nEmptyDetectionTimeStep_  ( perception[ "empty-detection-time-step" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillanceReco::IsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
bool PerceptionRecoSurveillanceReco::IsInside( const MT_Vector2D& vPoint ) const
{
    if( !GET_HOOK( IsPointInsideLocalisation )( localisation_, &vPoint ) )
        return false;

    const unsigned int nCurrentTime = currentTimeStep_;
    const unsigned char env = GET_HOOK( GetVisionObject )( &vPoint );

    if( env == eVisionEmpty )
        return nCurrentTime >= nEmptyDetectionTimeStep_;

    bool bResult = true;

    if( env & eVisionUrban )
        bResult &= nCurrentTime >= nUrbanDetectionTimeStep_;

    if( env & eVisionForest )
        bResult &= nCurrentTime >= nForestDetectionTimeStep_;

    return bResult;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillanceReco::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void PerceptionRecoSurveillanceReco::GetAgentsInside( const wrapper::View& model, Perception_ABC::T_AgentPtrVector& result ) const
{
    Perception_ABC::T_AgentPtrVector agents;
    ListInCircleVisitor< wrapper::View > agentVisitor( agents );
    GET_HOOK( GetAgentListWithinLocalisation )( model, localisation_, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
    BOOST_FOREACH( wrapper::View agent, agents )
        if( IsInside( MT_Vector2D( agent[ "movement/position/x" ], agent[ "movement/position/y" ] ) ) )
            result.push_back( agent );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoSurveillance::PerceptionRecoSurveillance( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
{
    entity[ "perceptions/alat/monitoring" ].VisitIntegerChildren( boost::bind( &PerceptionRecoSurveillance::AddLocalisation, this, _2, model[ "tick" ] ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance::~PerceptionRecoSurveillance
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoSurveillance::~PerceptionRecoSurveillance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PerceptionRecoSurveillance::AddLocalisation( const wrapper::View& perception, unsigned int currentTimeStep )
{
    Add( std::auto_ptr< PerceptionRecoSurveillanceReco >( new PerceptionRecoSurveillanceReco( perception, currentTimeStep ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoSurveillance::Compute( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const MT_Vector2D& vPoint ) const
{
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
        if( (*itReco)->IsInside( vPoint ) )
            return PerceptionLevel::recognized_;
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionRecoSurveillance::Execute( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    Perception_ABC::T_AgentPtrVector perceivableAgents;
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        (*itReco)->GetAgentsInside( model, perceivableAgents );
        for( Perception_ABC::T_AgentPtrVector::const_iterator it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            const wrapper::View& agent = *it;
            if( GET_HOOK( CanBeSeen )( perceiver, agent ) )
                observer_.NotifyPerception( agent, PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoSurveillance::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{
    const MT_Vector2D location( target[ "movement/position/x" ], target[ "movement/position/y" ] );
    return Compute( perceiver, surfaces, location );
}
