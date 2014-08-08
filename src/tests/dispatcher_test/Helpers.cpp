// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "Helpers.h"
#include "MockResolver.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/MissionFactory.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/XmlTranslations.h"
#include <boost/shared_ptr.hpp>
#include <xeumeuleu/xml.hpp>
#include <vector>

using namespace dispatcher_test;

boost::shared_ptr< kernel::DecisionalModel > StaticModel::MakeDecisionalModel()
{
    const tools::StringResolver< kernel::FragOrderType > fragOrdersResolver;
    const tools::StringResolver< kernel::MissionType > missionResolver;
    kernel::MissionFactory factory( missionResolver, missionResolver, missionResolver, fragOrdersResolver );
    const std::string xml( "<model name='my_model'/>" );
    xml::xistringstream xis( xml );
    xis >> xml::start( "model" );
    return boost::make_shared< kernel::DecisionalModel >( xis, factory, &kernel::MissionFactory::CreateAgentMission, fragOrdersResolver );
}

boost::shared_ptr< kernel::AgentType > StaticModel::MakeAgentType()
{
    boost::shared_ptr< kernel::DecisionalModel > model( MakeDecisionalModel() ) ;
    decisionalModels_.push_back( model );
    const tools::Resolver< kernel::ComponentType, std::string > componentResolver;
    tools::Resolver< kernel::DecisionalModel, std::string > modelResolver;
    modelResolver.Register( model->GetName(), *model );
    const kernel::SymbolFactory symbolFactory;
    const std::string xml(
        "<type name='my_name' type='my_type' id='42' decisional-model='my_model'>"
            "<nature level='iii' nature-app6='undefined/undefined' atlas-nature='none'/>"
            "<equipments/>"
            "<crew-ranks/>"
            "<nbc suit='level1'/>"
        "</type>"
    );
    kernel::XmlTranslations translations;
    xml::xistringstream xis( xml );
    xis >> xml::start( "type" );
    return boost::make_shared< kernel::AgentType >( xis, componentResolver, modelResolver, symbolFactory, translations );
}

boost::shared_ptr< kernel::AutomatType > StaticModel::MakeAutomatType( const kernel::SymbolFactory& symbolFactory )
{
    boost::shared_ptr< kernel::DecisionalModel > model( MakeDecisionalModel() ) ;
    decisionalModels_.push_back( model );
    tools::Resolver< kernel::DecisionalModel, std::string > modelResolver;
    modelResolver.Register( model->GetName(), *model );
    const std::string xml(
        "<type name='my_name' type='my_type' id='42' decisional-model='my_model'>"
            "<unit command-post='true' max-occurs='3' min-occurs='3' type='automat_pc'/>"
        "</type>"
    );
    tools::MockResolver< kernel::AgentType, std::string > agentResolver;
    xml::xistringstream xis( xml );
    xis >> xml::start( "type" );

    boost::shared_ptr< kernel::AgentType > agent( MakeAgentType() );
    MOCK_EXPECT( agentResolver.Get ).once().with( "automat_pc" ).returns( boost::ref( *agent ) );
    return boost::make_shared< kernel::AutomatType >( xis, agentResolver, modelResolver, symbolFactory );
}
