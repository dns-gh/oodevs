// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MockEventTools_h_
#define __MockEventTools_h_

#include "actions/ActionFactory.h"
#include "actions/ActionsModel.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionPublisher.h"

#include "clients_gui/EventFactory.h"
#include "clients_gui/EventView_ABC.h"

#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Time_ABC.h"

#include "ENT/ENT_Tr.h"

#include "protocol/ServerPublisher_ABC.h"

#include <boost/assign.hpp>
#include <timeline/api.h>

MOCK_BASE_CLASS( MockTime, kernel::Time_ABC )
{
    MOCK_METHOD( GetDateTime, 0 );
    MOCK_METHOD( GetInitialDateTime, 0 );
    MOCK_METHOD( GetTickDuration, 0 );
    MOCK_METHOD( GetTimeAsString, 0 );
};

MOCK_BASE_CLASS( MockPublisher, Publisher_ABC )
{
    MOCK_METHOD( Send, 1, void( const sword::ClientToSim& ),            ClientToSim );
    MOCK_METHOD( Send, 1, void( const sword::ClientToAuthentication& ), ClientToAuthentication );
    MOCK_METHOD( Send, 1, void( const sword::ClientToReplay& ),         ClientToReplay );
    MOCK_METHOD( Send, 1, void( const sword::ClientToAar& ),            ClientToAar );
    MOCK_METHOD( Send, 1, void( const sword::ClientToMessenger& ),      ClientToMessenger );
    MOCK_METHOD( Register, 1, void( T_SimHandler ),    SimRegister );
    MOCK_METHOD( Register, 1, void( T_ReplayHandler ), ReplayRegister );
};

MOCK_BASE_CLASS( MockEntityResolver, kernel::EntityResolver_ABC )
{
    MOCK_METHOD( FindAgent, 1 );
    MOCK_METHOD( GetAgent, 1 );
    MOCK_METHOD( FindAutomat, 1 );
    MOCK_METHOD( GetAutomat, 1 );
    MOCK_METHOD( FindKnowledgeGroup, 1 );
    MOCK_METHOD( GetKnowledgeGroup, 1 );
    MOCK_METHOD( FindObject, 1 );
    MOCK_METHOD( GetObject, 1 );
    MOCK_METHOD( FindUrbanObject, 1 );
    MOCK_METHOD( GetUrbanObject, 1 );
    MOCK_METHOD( FindInhabitant, 1 );
    MOCK_METHOD( GetInhabitant, 1 );
    MOCK_METHOD( FindPopulation, 1 );
    MOCK_METHOD( GetPopulation, 1 );
    MOCK_METHOD( FindFormation, 1 );
    MOCK_METHOD( GetFormation, 1 );
    MOCK_METHOD( FindTeam, 1 );
    MOCK_METHOD( GetTeam, 1 );
    MOCK_METHOD( FindEntity, 1 );
};

MOCK_BASE_CLASS( MockAgentKnowledgeConverter, kernel::AgentKnowledgeConverter_ABC )
{
    MOCK_METHOD( FindAgent, 2 );
    MOCK_METHOD( FindPopulation, 2 );
    MOCK_METHOD( Find, 2, const kernel::AgentKnowledge_ABC*( const kernel::AgentKnowledge_ABC&, const kernel::Entity_ABC& ), FindAgentKnowledgeFromKnowledge );
    MOCK_METHOD( Find, 2, const kernel::AgentKnowledge_ABC*( const kernel::Agent_ABC&, const kernel::Entity_ABC& ), FindAgentKnowledgeFromAgent );
    MOCK_METHOD( Find, 2, const kernel::PopulationKnowledge_ABC*( const kernel::PopulationKnowledge_ABC&, const kernel::Entity_ABC& ), FindPopulationKnowledgeFromKnowledge );
    MOCK_METHOD( Find, 2, const kernel::PopulationKnowledge_ABC*( const kernel::Population_ABC&, const kernel::Entity_ABC& ), FindPopulationKnowledgeFromPopulation );
};

MOCK_BASE_CLASS( MockObjectKnowledgeConverter, kernel::ObjectKnowledgeConverter_ABC )
{
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( unsigned long, const kernel::KnowledgeGroup_ABC& ), FindObjectKnowledgeFromId );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( unsigned long, const kernel::Entity_ABC& ), FindObjectKnowledgeFromIdWithEntity );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::ObjectKnowledge_ABC&, const kernel::KnowledgeGroup_ABC& ), FindObjectKnowledgeFromKnowledge );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::Object_ABC&, const kernel::KnowledgeGroup_ABC& ), FindObjectKnowledgeFromObject );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::Object_ABC&, const kernel::Entity_ABC& ), FindObjectKnowledgeFromObjectWithEntity );
};

template< typename ViewState >
MOCK_BASE_CLASS( MockEventView, gui::EventView_ABC< ViewState > )
{
    MOCK_METHOD( Purge, 0, void() );
    MOCK_METHOD( BlockSignals, 1, void( bool ) );
    MOCK_METHOD( Draw, 1, void( gui::Viewport_ABC& ) );

    MOCK_METHOD_TPL( Build, 1, void( const ViewState& ) );
};

struct ApplicationFixture
{
    ApplicationFixture()
        : argc( 1 )
        , args( boost::assign::list_of< char* >( "" )( "" ) )
        , app ( argc, &args[0] )
    {}

    std::vector< char* > args;
    int argc;
    QApplication app;
};

struct PresenterBaseFixture : public ApplicationFixture
{
public:
    PresenterBaseFixture()
        : parameterFactory( coordinateConverter, entityResolver, staticModel, agentKnowledgeConverter, objectKnowledgeConverter, controllers.controller_ )
        , actionFactory( controllers.controller_, parameterFactory, entityResolver, staticModel, time )
        , actionPublisher( publisher, controllers, time )
        , actionsModel( actionFactory, publisher, controllers, time )
        , eventFactory( actionsModel, controllers )
    {}

    kernel::CoordinateConverter coordinateConverter;
    MockEntityResolver entityResolver;
    kernel::StaticModel staticModel;
    MockAgentKnowledgeConverter agentKnowledgeConverter;
    MockObjectKnowledgeConverter objectKnowledgeConverter;
    kernel::Controllers controllers;
    actions::ActionParameterFactory parameterFactory;
    MockTime time;
    actions::ActionFactory actionFactory;
    MockPublisher publisher;
    actions::ActionPublisher actionPublisher;
    actions::ActionsModel actionsModel;
    gui::EventFactory eventFactory;
};

#endif // __MockEventTools_h_
