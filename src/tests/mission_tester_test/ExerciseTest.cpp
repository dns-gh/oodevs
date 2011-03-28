// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "mission_tester_test_pch.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/MissionFactory.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/SymbolFactory.h"
#include "mission_tester/Model.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    std::auto_ptr< kernel::DecisionalModel > MakeDecisionalModel()
    {
        const tools::Resolver< kernel::FragOrderType, std::string > fragOrdersResolver;
        const tools::Resolver< kernel::MissionType, std::string > missionResolver;
        kernel::MissionFactory factory( missionResolver, missionResolver, missionResolver, fragOrdersResolver );
        const tools::Resolver< kernel::FragOrderType > fragOrders;
        const std::string xml( "<model name='my_model'/>" );
        xml::xistringstream xis( xml );
        xis >> xml::start( "model" );
        return std::auto_ptr< kernel::DecisionalModel >( new kernel::DecisionalModel( xis, factory, &kernel::MissionFactory::CreateAgentMission, fragOrders ) );
    }

    std::auto_ptr< kernel::AgentType > MakeAgentType()
    {
        const tools::Resolver< kernel::ComponentType, std::string > componentResolver;
        tools::Resolver< kernel::DecisionalModel, std::string > modelResolver;
        std::auto_ptr< kernel::DecisionalModel > model( MakeDecisionalModel() );
        modelResolver.Register( model->GetName(), *model );
        const kernel::SymbolFactory symbolFactory;
        const std::string xml(
            "<type name='my_name' type='my_type' id='42' decisional-model='my_model'>"
                "<nature level='iii' nature-app6='undefined/undefined' atlas-nature='none'/>"
                "<equipments/>"
                "<crew-ranks/>"
            "</type>"
        );
        xml::xistringstream xis( xml );
        xis >> xml::start( "type" );
        return std::auto_ptr< kernel::AgentType >( new kernel::AgentType( xis, componentResolver, modelResolver, symbolFactory ) );
    }
}

BOOST_AUTO_TEST_CASE( model_is_constructed_from_network_messages )
{
    kernel::StaticModel staticModel;
    std::auto_ptr< kernel::AgentType > agentTypeStub( MakeAgentType() );
    staticModel.types_.tools::Resolver< kernel::AgentType >::Register( agentTypeStub->GetId(), *agentTypeStub );
    mission_tester::Model model( staticModel );
    sword::SimToClient container;
    sword::UnitCreation& message = *container.mutable_message()->mutable_unit_creation();
    message.mutable_unit()->set_id( 42u );
    message.set_name( "test" );
    message.mutable_type()->set_id( agentTypeStub->GetId() );
    message.set_pc( true );
    model.OnReceiveMessage( container );
    BOOST_CHECK( model.FindAgent( 42u ) );
    agentTypeStub.release();
}
