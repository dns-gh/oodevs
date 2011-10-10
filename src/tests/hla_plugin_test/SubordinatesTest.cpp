// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Subordinates.h"
#include "MockCallsignResolver.h"
#include "MockResolver.h"
#include "MockTransportedUnitsVisitor.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Agent_ABC.h"

using namespace plugins::hla;

namespace dispatcher
{
    MOCK_BASE_CLASS( MockAutomat, Automat_ABC )
    {
        MockAutomat( unsigned long id )
            : Automat_ABC( id )
        {}
        MOCK_METHOD( IsEngaged, 0 );
        MOCK_METHOD( GetTeam, 0 );
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( GetLogisticLevel, 0 );
        MOCK_METHOD( GetKnowledgeGroup, 0 );
        MOCK_METHOD( GetAgents, 0 );
        MOCK_METHOD( GetAutomats, 0 );
        MOCK_METHOD( GetParentAutomat, 0 );
        MOCK_METHOD( GetFormation, 0 );
        MOCK_METHOD( GetLogisticEntity, 0 );
        MOCK_METHOD( SendCreation, 1 );
        MOCK_METHOD( SendFullUpdate, 1 );
        MOCK_METHOD( SendDestruction, 1 );
        MOCK_METHOD( Accept, 1 );
        MOCK_METHOD_EXT( Send, 1, void( sword::ParentEntity& ), Send );
        MOCK_METHOD_EXT( Register   , 1, void( dispatcher::Automat_ABC& ), RegisterAutomat );
        MOCK_METHOD_EXT( Remove     , 1, void( dispatcher::Automat_ABC& ), RemoveAutomat );
        MOCK_METHOD_EXT( Register   , 1, void( dispatcher::Agent_ABC& ), RegisterAgent );
        MOCK_METHOD_EXT( Remove     , 1, void( dispatcher::Agent_ABC& ), RemoveAgent );
        MOCK_METHOD_EXT( SetSuperior, 1, void( dispatcher::Formation_ABC& ), SetSuperiorFormation );
        MOCK_METHOD_EXT( SetSuperior, 1, void( dispatcher::Automat_ABC& ), SetSuperiorAutomat );
    };
    MOCK_BASE_CLASS( MockAgent, Agent_ABC )
    {
        MockAgent( unsigned long id )
            : Agent_ABC( id )
        {}
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( GetPosition, 0 );
        MOCK_METHOD( GetSuperior, 0 );
        MOCK_METHOD( GetOperationalState, 0 );
        MOCK_METHOD( GetOperationalStateValue, 0 );
        MOCK_METHOD( Accept, 1 );
        MOCK_METHOD( IsCommandPost, 0 );
        MOCK_METHOD( Equipments, 0 );
        MOCK_METHOD( Troops, 0 );
        MOCK_METHOD( GetAltitude, 0 );
        MOCK_METHOD( GetSpeed, 0 );
        MOCK_METHOD( GetDirection, 0 );
        MOCK_METHOD( GetForceRatio, 0 );
        MOCK_METHOD( GetOrder, 0 );
        MOCK_METHOD( IsMounted, 0 );
        MOCK_METHOD( SetSuperior, 1 );
        MOCK_METHOD( SendCreation, 1 );
        MOCK_METHOD( SendFullUpdate, 1 );
        MOCK_METHOD( SendDestruction, 1 );
    };
}

BOOST_AUTO_TEST_CASE( subordinates_notifies_automat_subordinates_callsign_and_unique_identifier )
{
    MockCallsignResolver callsignResolver;
    tools::MockResolver< dispatcher::Automat_ABC > automatResolver;
    Subordinates subordinates( callsignResolver, automatResolver );
    MockTransportedUnitsVisitor visitor;
    const unsigned int automatId = 42;
    const unsigned int agentId = 43;
    dispatcher::MockAutomat automat( automatId );
    dispatcher::MockAgent agent( agentId );
    tools::Resolver< dispatcher::Agent_ABC > agentsResolver;
    agentsResolver.Register( agentId, agent );
    MOCK_EXPECT( automatResolver, Get ).once().with( automatId ).returns( boost::ref( automat ) );
    MOCK_EXPECT( automat, GetAgents ).once().returns( boost::ref( agentsResolver ) );
    MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( agentId ).returns( "callsign" );
    MOCK_EXPECT( callsignResolver, ResolveUniqueId ).once().with( agentId ).returns( "unique" );
    MOCK_EXPECT( visitor, Notify ).once().with( "callsign", "unique" );
    subordinates.Apply( automatId, visitor );
}
