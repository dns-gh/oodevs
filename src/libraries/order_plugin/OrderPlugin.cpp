// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "OrderPlugin.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "dispatcher/AgentKnowledgeConverter.h"
#include "dispatcher/Config.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Time_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/MessageDispatcher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::order;

namespace
{
    struct SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
        virtual QString GetTimeAsString() const { return QString(); }
    };
}

struct OrderPlugin::Publisher : public Publisher_ABC
{
    explicit Publisher( dispatcher::SimulationPublisher_ABC& simulation )
        : pSimulation_( &simulation )
    {
        // NOTHING
    }
    virtual void Send( const sword::ClientToSim& message )
    {
        pSimulation_->Send( message );
    }
    virtual void Send( const sword::ClientToAuthentication&  ) {}
    virtual void Send( const sword::ClientToReplay& ){}
    virtual void Send( const sword::ClientToAar& ) {}
    virtual void Send( const sword::ClientToMessenger& ) {}

    dispatcher::SimulationPublisher_ABC* pSimulation_;
};

// -----------------------------------------------------------------------------
// Name: OrderPlugin constructor
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
OrderPlugin::OrderPlugin( const dispatcher::Config& config, const kernel::StaticModel& staticModel,
                          const dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& simulation )
    : pConverter_        ( new kernel::CoordinateConverter( config ) )
    , pEntities_         ( new dispatcher::ModelAdapter( model ) )
    , pAgentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , pObjectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , pController_       ( new kernel::Controller() )
    , pParameters_       ( new actions::ActionParameterFactory( *pConverter_, *pEntities_, staticModel, *pAgentsKnowledges_, *pObjectsKnowledges_, *pController_ ) )
    , pTime_             ( new SimulationTime() )
    , pFactory_          ( new actions::ActionFactory( *pController_, *pParameters_, *pEntities_, staticModel, *pTime_ ) )
    , pPublisher_        ( new Publisher( simulation ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin destructor
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
OrderPlugin::~OrderPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::Resolve
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::Resolve( const sword::ClientToSim& message )
{
    if( message.message().has_order_stream() )
    {
        try
        {
            xml::xistringstream xis( message.message().order_stream().serialized_order() );
            xis >> xml::start( "actions" )
                    >> xml::list( "action", *this, &OrderPlugin::ReadAction )
                >> xml::end;
        }
        catch( std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Error in OrderStream message: " << e.what() )
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::ReadAction
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::ReadAction( xml::xistream& xis )
{
    std::auto_ptr< actions::Action_ABC > action( pFactory_->CreateAction( xis ) );
    if( action.get() )
        action->Publish( *pPublisher_ );
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::NotifyClientAuthenticated
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::NotifyClientLeft
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::Receive
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::Receive( const sword::SimToClient& /*message*/ )
{
    // NOTHING
}
