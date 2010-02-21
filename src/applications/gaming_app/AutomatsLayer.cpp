// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AutomatsLayer.h"
#include "gaming/MissionParameters.h"
#include "gaming/ConvexHulls.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_gui/ValuedDragObject.h"
#include "protocol/simulationsenders.h"
#include "protocol/publisher_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const Profile_ABC& profile, gui::AgentsLayer& agents, Publisher_ABC& publisher, const CoordinateConverter_ABC& converter )
    : gui::AutomatsLayer( controllers, tools, strategy, view, profile, agents )
    , tools_( tools )
    , publisher_( publisher )
    , converter_( converter )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
AutomatsLayer::~AutomatsLayer()
{
    // NOTHING
}

// $$$$ AGE 2007-06-14: pas super terrible tout ca

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AutomatsLayer::Draw( const Entity_ABC& entity, Viewport_ABC& viewport )
{
    gui::AutomatsLayer::Draw( entity, viewport );
    if( !ShouldDisplay( entity ) && selected_ == &entity )
    {
        SelectColor( entity );
        const Positions& positions = entity.Get< Positions >();
        const geometry::Point2f position = positions.GetPosition();
        viewport.SetHotpoint( position );
        selected_->Get< MissionParameters >().Draw( position, viewport, tools_ );
        selected_->Get< ConvexHulls >().Draw( position, viewport, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifySelected
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifySelected( const Automat_ABC* automat )
{
    selected_ = automat;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleEnterDragEvent
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    return selected_ && gui::ValuedDragObject::Provides< const AgentType >( event );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleDropEvent
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( !selected_ )
        return false;
    if( const AgentType* droppedItem = gui::ValuedDragObject::GetValue< const AgentType >( event ) )
    {
        RequestCreation( point, *droppedItem );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::RequestCreation
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AutomatsLayer::RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type )
{
    simulation::UnitCreationRequest message;
    message().set_oid_automate( selected_->GetId() );
    converter_.ConvertToGeo< Common::MsgCoordLatLong >( point, *message().mutable_position() );
    message().set_type_pion( type.GetId() );
    message.Send( publisher_ );
}
