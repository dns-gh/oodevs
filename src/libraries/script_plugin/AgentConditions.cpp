// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "AgentConditions.h"
#include "SimpleEntityCondition.h"
#include "AgentEvents.h"
#include "AgentManipulator.h"
#include "directia/Brain.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentConditions constructor
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
AgentConditions::AgentConditions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_ ( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentConditions destructor
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
AgentConditions::~AgentConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::RegisterIn
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
void AgentConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.agents", this );
    brain.RegisterFunction( "AgentEnters",             &AgentConditions::AgentEnters );
    brain.RegisterFunction( "KnowledgeEnters",         &AgentConditions::KnowledgeEnters );
    brain.RegisterFunction( "OperationalStateChanged", &AgentConditions::OperationalStateChanged );
    brain.RegisterFunction( "MissionStarted",          &AgentConditions::MissionStarted );
    brain.RegisterFunction( "KnowledgeCreated",        &AgentConditions::KnowledgeCreated );
    brain.RegisterFunction( "KnowledgeCreatedIn",      &AgentConditions::KnowledgeCreatedIn );
    brain.RegisterFunction( "PerceptionChanged",       &AgentConditions::PerceptionChanged );
}

namespace directia
{
    template< > inline void UsedByDIA    ( dispatcher::Agent* ) { }
    template< > inline void ReleasedByDIA( dispatcher::Agent* ) { }
}

namespace
{
    template< typename E >
    class Enters : public SimpleEntityCondition< E >
    {
    public:
        Enters( const Zone& zone, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
            : SimpleEntityCondition< E >( controller )
            , zone_      ( ToPolygon( zone ) )
            , box_       ( zone_.BoundingBox() )
            , converter_ ( converter )
        {
            // NOTHING
        }

        virtual void NotifyUpdated( const E& move )
        {
            const geometry::Point2f position = converter_.ConvertToXY( move.entity.position_ );
            if( box_.IsInside( position ) && zone_.IsInside( position ) )
                Trigger( move.entity );
        }

    private:
        Enters& operator=( const Enters& );

    private:
        geometry::Polygon2f zone_;
        geometry::Rectangle2f box_;
        const kernel::CoordinateConverter_ABC& converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::AgentEnters
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::AgentEnters( const Zone& zone )
{
    return boost::shared_ptr< Condition_ABC >( new ::Enters< events::AgentMoved >( zone, controller_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeEnters
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeEnters( const dispatcher::Zone& zone )
{
    return boost::shared_ptr< Condition_ABC >( new ::Enters< events::KnowledgeMoved >( zone, controller_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeCreatedIn
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeCreatedIn( const dispatcher::Zone& zone )
{
    return boost::shared_ptr< Condition_ABC >( new ::Enters< events::KnowledgeCreated >( zone, controller_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > AgentConditions::MissionStarted
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::MissionStarted()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::MissionStarted >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > AgentConditions::OperationalStateChanged
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::OperationalStateChanged()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::OperationalStateChanged >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeCreated
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeCreated()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::KnowledgeCreated >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > AgentConditions::PerceptionChanged
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::PerceptionChanged()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::PerceptionChanged >( controller_ ) );
}
