// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PerceptionCoupDeSonde.h"
#include "PerceptionObserver_ABC.h"
#include "PerceptionLevel.h"
#include "ListInCircleVisitor.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include "wrapper/Event.h"
#include "MT_Tools/MT_Vector2D.h"

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( BelongsToKnowledgeGroup, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )

// -----------------------------------------------------------------------------
// Name: PerceptionCoupDeSonde constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionCoupDeSonde::PerceptionCoupDeSonde( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
    , rWidth_  ( entity[ "perceptions/drill-blow/width" ] )
    , rLength_ ( entity[ "perceptions/drill-blow/length" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCoupDeSonde destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionCoupDeSonde::~PerceptionCoupDeSonde()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCoupDeSonde::ComputePoint
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionCoupDeSonde::ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const MT_Vector2D& vTargetPos ) const
{
    const MT_Vector2D vSourcePos( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    if( vSourcePos.Distance( vTargetPos ) > rLength_ )
        return PerceptionLevel::notSeen_;
    const MT_Vector2D direction( perceiver[ "movement/direction/x" ], perceiver[ "movement/direction/y" ] );

    if( fabs( ( vSourcePos - vTargetPos ) * direction ) <= rWidth_ )
        return PerceptionLevel::recognized_;

    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionCoupDeSonde::ComputeAgent
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionCoupDeSonde::ComputeAgent( const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const wrapper::View& target ) const
{
    if( GET_HOOK( BelongsToKnowledgeGroup )( perceiver, target ) )
        return PerceptionLevel::recognized_;

    const MT_Vector2D vSourcePos( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    const MT_Vector2D vTargetPos( target[ "movement/position/x" ], target[ "movement/position/y" ] );
    if( vSourcePos.Distance( vTargetPos ) >  rLength_ )
        return PerceptionLevel::notSeen_;

    const MT_Vector2D direction( perceiver[ "movement/direction/x" ], perceiver[ "movement/direction/y" ] );
    if( fabs( ( vSourcePos - vTargetPos ) * direction ) <= rWidth_ )
        return PerceptionLevel::recognized_;

    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionCoupDeSonde::ExecuteAgents
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PerceptionCoupDeSonde::ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    Perception_ABC::T_AgentPtrVector vAgentDetectedList;
    ListInCircleVisitor< wrapper::View > agentVisitor( vAgentDetectedList );
    const MT_Vector2D vSourcePos( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    GET_HOOK( GetAgentListWithinCircle )( model, vSourcePos, rLength_, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );

    for ( Perception_ABC::T_AgentPtrVector::const_iterator itAgent = vAgentDetectedList.begin(); itAgent != vAgentDetectedList.end(); ++itAgent )
    {
        const wrapper::View& agent = *itAgent;
        if( GET_HOOK( CanBeSeen )( perceiver, agent ) )
            observer_.NotifyAgentPerception( agent, ComputeAgent( perceiver, surfaces, agent ) );
    }
}
