// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Client.h"
#include "Exercise.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "client_proxy/SwordMessagePublisher_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Time_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace mission_tester;

namespace
{
    class SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
    };
    class AgentKnowledgeConverter : public kernel::AgentKnowledgeConverter_ABC
    {
    public:
        virtual const kernel::AgentKnowledge_ABC* FindAgent( unsigned long /*id*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long /*id*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::AgentKnowledge_ABC* Find( const kernel::AgentKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::AgentKnowledge_ABC* Find( const kernel::Agent_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
    };
    class ObjectKnowledgeConverter : public kernel::ObjectKnowledgeConverter_ABC
    {
        virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long /*id*/, const kernel::Team_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long /*id*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const { return 0; }
        virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const { return 0; }
    };
}

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: PHC 2011-03-21
// -----------------------------------------------------------------------------
Exercise::Exercise( kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel )
    : controller_              ( new kernel::Controller() )
    , time_                    ( new SimulationTime() )
    , agentKnowledgeConverter_ ( new AgentKnowledgeConverter() )
    , objectKnowledgeConverter_( new ObjectKnowledgeConverter() )
    , parameters_              ( new actions::ActionParameterFactory( staticModel.coordinateConverter_, entities, staticModel, *agentKnowledgeConverter_, *objectKnowledgeConverter_, *controller_ ) )
    , factory_                 ( new actions::ActionFactory( *controller_, *parameters_, entities, staticModel, *time_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: PHC 2011-03-21
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise::SendOrder
// Created: PHC 2011-03-23
// -----------------------------------------------------------------------------
void Exercise::SendOrder( const std::string& message, Client& client ) const
{
    xml::xistringstream xis( message );
    xis >> xml::start( "action" );
    std::auto_ptr< actions::Action_ABC > action( factory_->CreateAction( xis ) );
    if( action.get() )
        action->Publish( client );
}
