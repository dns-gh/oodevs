// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionObserver.h"
#include "PerceptionLevel.h"
#include "wrapper/View.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/foreach.hpp>

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: PerceptionObserver constructor
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
PerceptionObserver::PerceptionObserver( const wrapper::View& perceiver )
    : recordModeEnabled_         ( perceiver[ "perceptions/record-mode/activated" ] )
    , agentNotifications_        ( perceiver[ "perceptions/notifications/agents" ] )
    , objectNotifications_       ( perceiver[ "perceptions/notifications/objects" ] )
    , urbanBlockNotifications_   ( perceiver[ "perceptions/notifications/urban-blocks" ] )
    , flowNotifications_         ( perceiver[ "perceptions/notifications/population-flows" ] )
    , concentrationNotifications_( perceiver[ "perceptions/notifications/population-concentrations" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver destructor
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
PerceptionObserver::~PerceptionObserver()
{
    agentNotifications_.Post();
    objectNotifications_.Post();
    urbanBlockNotifications_.Post();
    flowNotifications_.Post();
    concentrationNotifications_.Post();
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver::NotifyUrbanPerception
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
void PerceptionObserver::NotifyUrbanPerception( const wrapper::View& urbanBlock, const PerceptionLevel& level )
{
    wrapper::Node notification = urbanBlockNotifications_.AddElement();
    notification[ "target" ] = urbanBlock;
    notification[ "level" ] = level.GetID();
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver::NotifyAgentPerception
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
void PerceptionObserver::NotifyAgentPerception( const wrapper::View& agent, const PerceptionLevel& level )
{
    NotifyAgentPerception( agent, level, recordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver::NotifyAgentPerception
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
void PerceptionObserver::NotifyAgentPerception( const wrapper::View& agent, const PerceptionLevel& level, bool perceptionRecorded )
{
    wrapper::Node notification = agentNotifications_.AddElement();
    notification[ "target" ] = agent[ "pion" ];
    notification[ "level" ] = level.GetID();
    notification[ "recorded" ] = perceptionRecorded;
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver::NotifyObjectPerception
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
void PerceptionObserver::NotifyObjectPerception( const wrapper::View& object, const PerceptionLevel& level )
{
    wrapper::Node notification = objectNotifications_.AddElement();
    notification[ "target" ] = object;
    notification[ "level" ] = level.GetID();
    notification[ "recorded" ] = recordModeEnabled_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver::NotifyFlowPerception
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
void PerceptionObserver::NotifyFlowPerception( const wrapper::View& flow, const PerceptionLevel& level, const std::vector< MT_Vector2D >& shape )
{
    wrapper::Node notification = flowNotifications_.AddElement();
    notification[ "target" ] = flow;
    notification[ "level" ] = level.GetID();
    notification[ "recorded" ] = recordModeEnabled_;
    wrapper::Node points = notification[ "shape" ];
    BOOST_FOREACH( const MT_Vector2D& p, shape )
    {
        wrapper::Node point = points.AddElement();
        point[ "x" ] = p.rX_;
        point[ "y" ] = p.rY_;
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionObserver::NotifyConcentrationPerception
// Created: SLI 2012-06-19
// -----------------------------------------------------------------------------
void PerceptionObserver::NotifyConcentrationPerception( const wrapper::View& concentration, const PerceptionLevel& level )
{
    wrapper::Node notification = concentrationNotifications_.AddElement();
    notification[ "target" ] = concentration;
    notification[ "level" ] = level.GetID();
    notification[ "recorded" ] = recordModeEnabled_;
}
