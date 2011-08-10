// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "ActionPublisher.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/AgentKnowledgeConverter.h"
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "actions/Action_ABC.h"
#include "tools/ExerciseConfig.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionFactory.h"
#include "protocol/ServerPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/noncopyable.hpp>

using namespace plugins::timeline;

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

namespace
{
    class Publisher : public Publisher_ABC, private boost::noncopyable
    {
    public:
        explicit Publisher( dispatcher::SimulationPublisher_ABC& sim ) : sim_( sim ) {}

        virtual void Send( const sword::ClientToSim& message )
        {
            sim_.Send( message );
        }
        virtual void Send( const sword::ClientToAuthentication&  ) {}
        virtual void Send( const sword::ClientToReplay& ){}
        virtual void Send( const sword::ClientToAar& ) {}
        virtual void Send( const sword::ClientToMessenger& ) {}

    private:
        dispatcher::SimulationPublisher_ABC& sim_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher constructor
// Created: JCR 2010-09-07
// -----------------------------------------------------------------------------
ActionPublisher::ActionPublisher( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& sim )
    : publisher_        ( new Publisher( sim ) )
    , entities_         ( new dispatcher::ModelAdapter( model ) )
    , agentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , objectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , converter_        ( new kernel::CoordinateConverter( config ) )
    , time_             ( new SimulationTime() )
    , parameters_       ( new actions::ActionParameterFactory( *converter_, *entities_, staticModel, *agentsKnowledges_, *objectsKnowledges_, controller ) )
    , factory_          ( new actions::ActionFactory( controller, *parameters_, *entities_, staticModel, *time_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher destructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
ActionPublisher::~ActionPublisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Execute
// Created: JCR 2010-09-07
// -----------------------------------------------------------------------------
void ActionPublisher::Execute( xml::xistream& xis )
{
    std::auto_ptr< actions::Action_ABC > action( factory_->CreateAction( xis ) );
    if( action.get() )
        action->Publish( *publisher_ );
}
