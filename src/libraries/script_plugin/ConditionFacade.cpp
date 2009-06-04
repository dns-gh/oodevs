// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ConditionFacade.h"
#include "SimulationConditions.h"
#include "AgentConditions.h"
#include "AutomatConditions.h"
#include "PopulationConditions.h"
#include "EventCondition.h"
#include "ClientConditions.h"
#include "ScriptConditions.h"
#include "IndicatorConditions.h"
#include "MiscEvents.h"
#include "directia/Brain.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ConditionFacade constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
ConditionFacade::ConditionFacade( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const dispatcher::Model& model )
    : controller_( controller )
{
    Add( new SimulationConditions( controller ) );
    Add( new AgentConditions( controller, converter ) );
    Add( new AutomatConditions( controller ) );
    Add( new PopulationConditions( controller, converter ) );
    Add( new ClientConditions( controller, model ) );
    Add( new ScriptConditions( controller ) );
    Add( new IndicatorConditions( controller ) );
}

// -----------------------------------------------------------------------------
// Name: ConditionFacade destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
ConditionFacade::~ConditionFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConditionFacade::RegisterIn
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
void ConditionFacade::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events", this );
    brain.RegisterFunction( "Once",  &ConditionFacade::Once );
    brain.RegisterFunction( "Timer", &ConditionFacade::Timer );
    CompositeRegistrable::RegisterIn( brain );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > ConditionFacade::Once
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ConditionFacade::Once()
{
    struct Once : public BaseCondition
    {
        Once() : triggered_( false ) {}
        virtual void Activate( bool active )
        {
            BaseCondition::Activate( active );
            if( !triggered_ && active )
            {
                Trigger();
                triggered_ = true;
            }
        }
        bool triggered_;
    };
    return boost::shared_ptr< Condition_ABC >( new Once() );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > ConditionFacade::Timer
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ConditionFacade::Timer( float seconds )
{
    struct Timer : public SimpleEventCondition< events::TimeFlowed >
    {
        Timer( kernel::Controller& controller, float time )
            : SimpleEventCondition( controller )
            , time_( time ){}

        virtual void NotifyUpdated( const events::TimeFlowed& time )
        {
            time_ -= time.delta;
            if( time_ < 0 )
                Trigger();
        };
        float time_;
    };

    return boost::shared_ptr< Condition_ABC >( new Timer( controller_, seconds ) );
}
