// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "PerceptionRecoUrbanBlock.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( CanUrbanBlockBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* urbanBlock ) )
DECLARE_HOOK( IsPointInsideLocalisation, bool, ( const SWORD_Model* localisation, const MT_Vector2D* point ) )
DECLARE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const SWORD_Model* localization,
                                                      void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlockReco constructor
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PerceptionRecoUrbanBlockReco::PerceptionRecoUrbanBlockReco( const wrapper::View& perception )
    : pUrbanBlock_ ( perception[ "localization" ][ "block" ] )
    , localisation_( perception[ "localization" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlockReco::IsInside
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
bool PerceptionRecoUrbanBlockReco::IsInside( const MT_Vector2D& vPoint ) const
{
    return GET_HOOK( IsPointInsideLocalisation )( localisation_, &vPoint );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlockReco::GetAgentsInside
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PerceptionRecoUrbanBlockReco::GetAgentsInside( const wrapper::View& root, Perception_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    ListInCircleVisitor< wrapper::View > agentVisitor( result );
    GET_HOOK( GetAgentListWithinLocalisation )( root, localisation_, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlockReco::CanSeeIt
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
bool PerceptionRecoUrbanBlockReco::CanSeeIt( const wrapper::View& perceiver ) const
{
    return GET_HOOK( CanUrbanBlockBeSeen )( perceiver, pUrbanBlock_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlock constructor
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PerceptionRecoUrbanBlock::PerceptionRecoUrbanBlock( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
{
    entity[ "perceptions/urban" ].VisitIntegerChildren( boost::bind( &PerceptionRecoUrbanBlock::AddLocalisation, this, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlock::~PerceptionRecoUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PerceptionRecoUrbanBlock::~PerceptionRecoUrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlock::AddLocalisation
// Created: SLI 2012-08-27
// -----------------------------------------------------------------------------
void PerceptionRecoUrbanBlock::AddLocalisation( const wrapper::View& perception )
{
    Add( std::auto_ptr< PerceptionRecoUrbanBlockReco >( new PerceptionRecoUrbanBlockReco( perception ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlock::Compute
// Created: JVT 2004-10-21
// Modified: MGD 2010-02-11
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoUrbanBlock::Compute( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const MT_Vector2D& vPoint ) const
{
    for( T_RecoVector::const_iterator it = recos_.begin(); it != recos_.end(); ++it )
        if( ( *it )->IsInside( vPoint ) )
            return PerceptionLevel::recognized_;
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlock::Execute
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PerceptionRecoUrbanBlock::Execute( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    T_AgentPtrVector perceivableAgents;
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        ( *itReco )->GetAgentsInside( model, perceivableAgents );
        for( T_AgentPtrVector::const_iterator it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            const wrapper::View& agent = *it;
            if( GET_HOOK( CanBeSeen )( perceiver, agent ) && ( *itReco )->CanSeeIt( perceiver ) )
                observer_.NotifyPerception( agent, PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoUrbanBlock::Compute
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoUrbanBlock::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{
    return Compute( perceiver, surfaces, MT_Vector2D( target[ "movement/position/x" ], target[ "movement/position/y" ] ) );
}
