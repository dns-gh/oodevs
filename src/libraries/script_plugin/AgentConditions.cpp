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
#include "directia/brain/Brain.h"
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
void AgentConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.agents" ] = this ;
    brain.Register( "AgentCreated",            &AgentConditions::AgentCreated );
    brain.Register( "AgentEnters",             &AgentConditions::AgentEnters );
    brain.Register( "KnowledgeEnters",         &AgentConditions::KnowledgeEnters );
    brain.Register( "OperationalStateChanged", &AgentConditions::OperationalStateChanged );
    brain.Register( "ForceRatioChanged",       &AgentConditions::ForceRatioChanged );
    brain.Register( "MissionStarted",          &AgentConditions::MissionStarted );
    brain.Register( "KnowledgeCreated",        &AgentConditions::KnowledgeCreated );
    brain.Register( "KnowledgeCreatedIn",      &AgentConditions::KnowledgeCreatedIn );
    brain.Register( "PerceptionChanged",       &AgentConditions::PerceptionChanged );
    brain.Register( "MountedStateChanged",     &AgentConditions::MountedStateChanged );
}

namespace directia
{
    void UsedByDIA    ( dispatcher::Agent* ) { }
    void ReleasedByDIA( dispatcher::Agent* ) { }
    void UsedByDIA( AgentConditions* ) {}
    void ReleasedByDIA( AgentConditions* ) {}
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
            // $$$$ _RC_ SBO 2010-05-27: reverse position_ lat/long
            const geometry::Point2d pos( move.entity.GetPosition().Y(), move.entity.GetPosition().X() );
            const geometry::Point2f position = converter_.ConvertFromGeo( pos );
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
// Name: AgentConditions::AgentCreated
// Created: SBO 2009-08-19
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::AgentCreated()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::AgentCreated >( controller_ ) );
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
// Name: AgentConditions::KnowledgeEnters
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeEnters( const dispatcher::Zone& zone )
{
    return boost::shared_ptr< Condition_ABC >( new ::Enters< events::KnowledgeMoved >( zone, controller_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::KnowledgeCreatedIn
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeCreatedIn( const dispatcher::Zone& zone )
{
    return boost::shared_ptr< Condition_ABC >( new ::Enters< events::KnowledgeCreated >( zone, controller_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::MissionStarted
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::MissionStarted()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::MissionStarted >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::OperationalStateChanged
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::OperationalStateChanged()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::OperationalStateChanged >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::MountedStateChanged
// Created: SBO 2010-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::MountedStateChanged()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::MountedStateChanged >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::ForceRatioChanged
// Created: SBO 2009-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::ForceRatioChanged()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::ForceRatioChanged >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::KnowledgeCreated
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::KnowledgeCreated()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::KnowledgeCreated >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentConditions::PerceptionChanged
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AgentConditions::PerceptionChanged()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::PerceptionChanged >( controller_ ) );
}
