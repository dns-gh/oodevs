// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoSummariesWidget.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "SummariesDisplayer.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget constructor
// Created: SBO 2007-02-28
// -----------------------------------------------------------------------------
InfoSummariesWidget::InfoSummariesWidget( QWidget* parent, Controllers& controllers )
    : QVBox( parent, "InfoSummariesWidget" )
    , controllers_( controllers )
    , selected_( controllers )
{
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, this );
    group->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
    group->setFixedWidth( 250 );
    display_.reset( new SummariesDisplayer( group ) );
    controllers_.Register( *this );
    group->hide();
}

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget destructor
// Created: SBO 2007-02-28
// -----------------------------------------------------------------------------
InfoSummariesWidget::~InfoSummariesWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget::showEvent
// Created: SBO 2007-02-28
// -----------------------------------------------------------------------------
void InfoSummariesWidget::showEvent( QShowEvent* )
{
    const Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget::NotifySelected
// Created: SBO 2007-02-28
// -----------------------------------------------------------------------------
void InfoSummariesWidget::NotifySelected( const Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        display_->Hide();
        if( selected_ )
            entity->Interface().Apply( &Displayable_ABC::DisplayInSummary, *display_ );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget::NotifyUpdated
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoSummariesWidget::NotifyUpdated( const Entity_ABC& entity )
{
    if( !selected_ || selected_ != &entity )
        return;
    entity.Interface().Apply( &Displayable_ABC::DisplayInSummary, *display_ );
}

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget::NotifyUpdated
// Created: SBO 2007-02-28
// -----------------------------------------------------------------------------
void InfoSummariesWidget::NotifyUpdated( const Attributes_ABC& extension )
{
    if( !selected_ || ( selected_->Retrieve< Attributes_ABC >() != &extension ) )
        return;
    extension.DisplayInSummary( *display_ );
}
