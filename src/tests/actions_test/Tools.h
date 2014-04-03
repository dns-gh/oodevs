// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "actions_test_pch.h"

#include "actions/Action_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include "protocol/Protocol.h"

#include <protocol/ServerPublisher_ABC.h>
#include <xeumeuleu/xml.hpp>

namespace
{

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

MOCK_BASE_CLASS( MockAgent, kernel::Agent_ABC )
{
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetType, 0 );
};

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

std::string Serialize( const actions::Action_ABC& action )
{
    xml::xostringstream xos;
    action.Serialize( xos << xml::start( "action" ) );
    return xos.str();
}

void CheckXml( const std::string expected, const std::string& obtained )
{
    xml::xistringstream xis1( expected ), xis2( obtained );
    xml::xostringstream xos1, xos2;
    xos1 << xis1;
    xos2 << xis2;
    BOOST_REQUIRE_EQUAL( xos1.str(), xos2.str() );
}

sword::ClientToSim Publish( const actions::Action_ABC& action )
{
    sword::ClientToSim msg;
    MockPublisher publisher;
    MOCK_EXPECT( publisher.ClientToSim ).once().with( mock::retrieve( msg ) );
    action.Publish( publisher, 42 );
    return msg;
}

}
