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
#include "Listener_ABC.h"
#include "ParameterFactory.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/Parameter_ABC.h"
#include "protocol/Protocol.h"
#include "client_proxy/SwordMessagePublisher_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Time_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace mission_tester;

namespace
{
    class SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
        virtual QString GetTimeAsString() const { return QString(); }
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
Exercise::Exercise( kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel, Publisher_ABC& publisher, xml::xistream& xis )
    : publisher_               ( publisher )
    , controller_              ( new kernel::Controller() )
    , time_                    ( new SimulationTime() )
    , agentKnowledgeConverter_ ( new AgentKnowledgeConverter() )
    , objectKnowledgeConverter_( new ObjectKnowledgeConverter() )
    , parameterFactory_        ( new actions::ActionParameterFactory( staticModel.coordinateConverter_, entities, staticModel,
                                                                      *agentKnowledgeConverter_, *objectKnowledgeConverter_, *controller_ ) )
    , actionFactory_           ( new actions::ActionFactory( *controller_, *parameterFactory_, entities, staticModel, *time_ ) )
    , factory_                 ( new ParameterFactory( staticModel.coordinateConverter_, *controller_, entities, xis/*, staticModel.objectTypes_*/ ) )
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

namespace
{
    struct Logger : public Publisher_ABC
    {
        Logger() : os_( "output.log" ) {}

        virtual void Send( const sword::ClientToSim& message ) { Log( message ); }
        virtual void Send( const sword::ClientToAuthentication& message ) { Log( message ); }
        virtual void Send( const sword::ClientToReplay& message ) { Log( message ); }
        virtual void Send( const sword::ClientToAar& message ) { Log( message ); }
        virtual void Send( const sword::ClientToMessenger& message ) { Log( message ); }

        template< typename T >
        void Log( const T& message )
        {
            os_ << "-------------------------" << std::endl
                << message.DebugString() << std::endl
                << "-------------------------" << std::endl;
        }

        std::ofstream os_;
    };
}

// -----------------------------------------------------------------------------
// Name: Exercise::CreateMission
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
bool Exercise::CreateMission( const kernel::Entity_ABC& target, const kernel::MissionType& mission )
{
    std::auto_ptr< actions::Action_ABC > action( actionFactory_->CreateAction( target, mission ) );
    if( CreateOrder( target, mission, action.get() ) )
    {
        NotifyMissionCreated( target, mission );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Exercise::CreateFragOrder
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
bool Exercise::CreateFragOrder( const kernel::Entity_ABC& target, const kernel::FragOrderType& mission )
{
    std::auto_ptr< actions::Action_ABC > action( actionFactory_->CreateAction( target, mission ) );
    if( CreateOrder( target, mission, action.get() ) )
    {
        NotifyFragOrderCreated( target, mission );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Exercise::CreateOrder
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
bool Exercise::CreateOrder( const kernel::Entity_ABC& target, const kernel::OrderType& order, actions::Action_ABC* action ) const
{
    tools::Iterator< const kernel::OrderParameter& > params( order.Resolver< kernel::OrderParameter >::CreateIterator() );
    while( params.HasMoreElements() )
    {
        const kernel::OrderParameter& param = params.NextElement();
        std::auto_ptr< actions::Parameter_ABC > parameter( factory_->CreateParameter( param ) );
        if( parameter.get() )
            action->AddParameter( *parameter.release() );
        else
        {
            NotifyInvalidParameter( target, order, param );
            return false;
        }
    }
    Logger logger;
    action->Publish( logger );
    action->Publish( publisher_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Exercise::Register
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Exercise::Register( Listener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: Exercise::NotifyInvalidParameter
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Exercise::NotifyInvalidParameter( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter ) const
{
    BOOST_FOREACH( Listener_ABC* listener, listeners_ )
        listener->ParameterCreationFailed( target, mission, parameter );
}

// -----------------------------------------------------------------------------
// Name: Exercise::NotifyMissionCreated
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Exercise::NotifyMissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    BOOST_FOREACH( Listener_ABC* listener, listeners_ )
        listener->MissionCreated( target, mission );
}

// -----------------------------------------------------------------------------
// Name: Exercise::NotifyFragOrderCreated
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Exercise::NotifyFragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    BOOST_FOREACH( Listener_ABC* listener, listeners_ )
        listener->FragOrderCreated( target, mission );
}
