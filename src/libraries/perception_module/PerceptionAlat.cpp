// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionAlat.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "VisionObject.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const SWORD_Model* localization,
                                                      void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( GetVisionObject, unsigned char, ( const MT_Vector2D* point ) )

namespace
{
    void AddLocalization( std::vector< wrapper::View >& localizations, const wrapper::View& perception )
    {
        localizations.push_back( perception[ "localization" ] );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionAlat constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionAlat::PerceptionAlat( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
{
    entity[ "perceptions/alat/reco" ].VisitIdentifiedChildren( boost::bind( &::AddLocalization, boost::ref( localisations_ ), _2 ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionAlat destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionAlat::~PerceptionAlat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionAlat::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PerceptionAlat::Execute( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MT_Vector2D perceiverPosition( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    const MT_Vector2D perceiverDirection( perceiver[ "movement/direction/x" ], perceiver[ "movement/direction/y" ] );
    const double rDetectionSemiHeight = perceiver[ "perceptions/max-agent-perception-distance" ] / 2.;

    // Recherche des pions dans la localisation
    Perception_ABC::T_AgentPtrVector agentsDetected;
    ListInCircleVisitor< wrapper::View > agentVisitor( agentsDetected );
    BOOST_FOREACH( const wrapper::View& localisation, localisations_ )
        GET_HOOK( GetAgentListWithinLocalisation )( model, localisation, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );

    // Enregistrement des pions vus
    BOOST_FOREACH( const wrapper::View& target, agentsDetected )
    {
        const MT_Vector2D targetPosition( target[ "movement/position/x" ], target[ "movement/position/y" ] );
        if( GET_HOOK( CanBeSeen )( perceiver, target ) && fabs( ( targetPosition - perceiverPosition ) * perceiverDirection ) <= rDetectionSemiHeight )
        {
            const bool bPerceptionDelayed = GET_HOOK( GetVisionObject )( &targetPosition ) != eVisionEmpty;
            observer_.NotifyPerception( target, PerceptionLevel::recognized_, bPerceptionDelayed );
        }
    }
}
