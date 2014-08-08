// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ItineraryParameter.h"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Itinerary.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/Protocol.h"

using namespace actions::gui;

ItineraryParameter::ItineraryParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , controller_( builder.GetControllers().controller_ )
    , converter_( builder.GetStaticModel().coordinateConverter_ )
    , resolver_( builder.GetPathfindResolver() )
    , entityLabel_( 0 )
    , potential_( 0 )
    , selected_( 0 )
{
    controller_.Register( *this );
}

ItineraryParameter::~ItineraryParameter()
{
    controller_.Unregister( *this );
}

QWidget* ItineraryParameter::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    entityLabel_ = new QLabel( "---", parent );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    layout->addWidget( entityLabel_ );
    return group_;
}

bool ItineraryParameter::InternalCheckValidity() const
{
    return selected_ || pathfind_;
}

void ItineraryParameter::NotifyContextMenu( const kernel::Pathfind_ABC& entity, kernel::ContextMenu& menu )
{
    potential_ = &entity;
    kernel::ContextMenu::T_MenuVariant variant = Param_ABC::CreateMenu( menu );
    if( QAction* const* action = boost::get< QAction* >( &variant ) )
    {
        if( *action )
        {
            (*action)->setCheckable( true );
            (*action)->setChecked( selected_ != 0 );
        }
    }
}

void ItineraryParameter::OnMenuClick()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName() : "---" );
    if( group_ && IsOptional() )
        group_->setChecked( selected_ != 0 );
    Update();
}

void ItineraryParameter::NotifyUpdated( const kernel::Pathfind_ABC& entity )
{
    if( &entity == selected_ )
        Display( entity.GetName() );
}

void ItineraryParameter::NotifyDeleted( const kernel::Pathfind_ABC& entity )
{
    if( &entity == potential_ )
        potential_ = 0;
    if( &entity == selected_ )
    {
        selected_ = 0;
        Display( tools::translate( "ItineraryParameter", "New itinerary") );
    }
}

void ItineraryParameter::Display( const QString& what )
{
    entityLabel_->setText( what );
}

void ItineraryParameter::Draw( const geometry::Point2f& /*point*/, const ::gui::Viewport_ABC& /*viewport*/, ::gui::GlTools_ABC& /*tools*/ ) const
{
    // $$$$ MCO 2014-08-08: TODO
}

void ItineraryParameter::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( IsChecked() )
    {
        if( selected_ )
            parameter.AddParameter( *new actions::parameters::Itinerary( parameter_, converter_, selected_->GetCreationMessage() ) );
        else if( pathfind_ )
            parameter.AddParameter( *new actions::parameters::Itinerary( parameter_, converter_, *pathfind_ ) );
        else
            parameter.AddParameter( *new actions::parameters::Itinerary( parameter_, converter_ ) );
    }
    else
        parameter.AddParameter( *new actions::parameters::Itinerary( parameter_, converter_ ) );
}

void ItineraryParameter::Visit( const actions::parameters::Itinerary& param )
{
    ActivateOptionalIfNeeded( param );
    if( auto pathfind = resolver_.Find( param.GetId() ) )
    {
        selected_ = pathfind;
        Display( selected_->GetName() );
    }
    else
    {
        pathfind_.reset( new sword::Pathfind( param.GetPathfind() ) );
        Display( tools::translate( "ItineraryParameter", "New itinerary") );
    }
}
