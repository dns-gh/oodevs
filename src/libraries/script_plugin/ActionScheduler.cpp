// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ActionScheduler.h"
#include "MiscEvents.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/Controller.h"
#include "tools/ExerciseConfig.h"

using namespace plugins::script;

bool ActionScheduler::sTimingComparator::operator()( const actions::Action_ABC* lhs, const actions::Action_ABC* rhs ) const
{
    if( !lhs || !rhs )
        return false;
    const actions::ActionTiming* lTiming = lhs->Retrieve< actions::ActionTiming >();
    const actions::ActionTiming* rTiming = rhs->Retrieve< actions::ActionTiming >();
    if( !lTiming || !rTiming )
        return false;
    const QDateTime ltime = lTiming->GetTime();
    const QDateTime rtime = rTiming->GetTime();
    if( ltime == rtime )
        return lhs->GetId() < rhs->GetId();
    return ltime < rtime;
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler constructor
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
ActionScheduler::ActionScheduler( const tools::ExerciseConfig& config, const std::string& filename, kernel::Controller& controller, actions::ActionFactory_ABC& factory, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_ ( publisher )
    , model_     ( new actions::ActionsModel( factory, publisher_, publisher_ ) )
{
    model_->Load( config.BuildExerciseChildFile( "scripts/resources/" + filename + ".ord" ), config.GetLoader() );
    tools::Iterator< const actions::Action_ABC& > it = model_->CreateIterator();
    while( it.HasMoreElements() )
    {
        const actions::Action_ABC& action = it.NextElement();
        actions_.insert( &action );
    }
    cursor_ = actions_.begin();
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler destructor
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
ActionScheduler::~ActionScheduler()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler::Tick
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
void ActionScheduler::Tick( const QDateTime& time )
{
    while( cursor_ != actions_.end() && (*cursor_)->Get< actions::ActionTiming >().GetTime() <= time )
    {
        (*cursor_)->Publish( publisher_ );
        ++cursor_;
    }
}

namespace
{
    QDateTime MakeDate( const std::string& str )
    {
        QString extended( str.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' );
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        return QDateTime::fromString( extended, Qt::ISODate );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler::NotifyUpdated
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
void ActionScheduler::NotifyUpdated( const events::SimulationTimeChanged& event )
{
    Tick( MakeDate( event.time ) );
}
