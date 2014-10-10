// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MissionParameters.h"
#include "LogTools.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory_ABC.h"
#include "actions/ActionTiming.h"
#include "actions/ActionError.h"
#include "actions/ActionTasker.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Tools.h"
#include <boost/foreach.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: MissionParameters constructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::MissionParameters( kernel::Controller& controller, const actions::ActionFactory_ABC& factory, unsigned long entityId )
    : controller_( controller )
    , factory_( factory )
    , entityId_( entityId )
    , currentMission_( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters destructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::~MissionParameters()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::UpdateMessage
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
template< typename T >
void MissionParameters::UpdateMessage( const T& message )
{
    if( message.type().id() == 0 )
        return;

    if( message.has_start_time() )
    {
        QDateTime time = tools::GDHStringToQDateTime( message.start_time().data() );
        tools::Iterator< const Action_ABC& > it = CreateIterator();
        while( it.HasMoreElements() )
        {
            const Action_ABC& action = it.NextElement();
            const actions::ActionTiming* timing = action.Retrieve< actions::ActionTiming>();
            if( timing && action.GetType() && action.GetType()->GetId() == message.type().id() && timing->GetTime() == time )
                return;
        }
    }

    try
    {
        std::unique_ptr< Action_ABC > action( factory_.CreateAction( message, true ) );
        if( !action.get() )
            return;
        Register( action->GetId(), *action );
        action.release();
        controller_.Update( *this );
    }
    catch( ... )
    {
        // $$$$ SBO 2010-05-14: Warning/invalid mission
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::UnitOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::AutomatOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::CrowdOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::FragOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::Draw
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !elements_.empty() && tools.ShouldDisplay( "MissionParameters" ) )
        BOOST_REVERSE_FOREACH( const auto& content, elements_ )
            if( const Action_ABC* action = content.second )
            {
                const auto& orderType = action->GetType();
                if( orderType && orderType->GetType() != eMissionType_FragOrder )
                {
                    action->Draw( where, viewport, tools );
                    return;
                }
            }
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::TaskCreationRequestAck& message )
{
    UpdateMessageAck( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::FragOrderAck& message )
{
    UpdateMessageAck( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::UpdateMessageAck
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
template< typename T >
void MissionParameters::UpdateMessageAck( const T& message )
{
    if( currentMission_ )
    {
        if( message.error_code() != sword::OrderAck_ErrorCode_no_error )
        {
            Action_ABC* action = factory_.CreateInvalidAction( *currentMission_ );
            action->Attach( *new ActionError( log_tools::Convert( message.error_code() ) ) );
            Register( action->GetId(), *action );
        }
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::NotifyCreated
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
void MissionParameters::NotifyCreated( const actions::Action_ABC& action )
{
    const ActionTasker* tasker = action.Retrieve< ActionTasker >();
    if( tasker && tasker->GetId() == entityId_ && action.GetType() )
        currentMission_ = &*action.GetType();
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::NotifyDeleted
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
void MissionParameters::NotifyDeleted( const actions::Action_ABC& action )
{
    const ActionTasker* tasker = action.Retrieve< ActionTasker >();
    if( tasker && tasker->GetId() == entityId_ )
        currentMission_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::GetEntityId
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
unsigned long MissionParameters::GetEntityId() const
{
    return entityId_;
}
