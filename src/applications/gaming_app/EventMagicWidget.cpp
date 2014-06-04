// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventMagicWidget.h"
#include "moc_EventMagicWidget.cpp"

#include "actions/ActionsModel.h"

#include "clients_gui/EventMagicPresenter.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventMagicViewState.h"
#include "clients_gui/TaskerWidget.h"

#include "clients_kernel/AgentTypes.h"

#include "gaming/Model.h"

#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: EventMagicWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMagicWidget::EventMagicWidget( gui::EventPresenter& presenter,
                                    kernel::Controllers& controllers,
                                    Model& model,
                                    const gui::EntitySymbols& symbols )
    : EventMagicWidget_ABC( presenter )
    , model_( model )
    , selectedEntity_( controllers )
    , lastTaskerId_( 0 )
{
    // Presenter
    magicPresenter_ = boost::make_shared< gui::EventMagicPresenter >( *this, controllers, model.actions_ );
    presenter_.AddSubPresenter( magicPresenter_ );

    // Layout
    taskerWidget_ = new gui::TaskerWidget( "magic-tasker", controllers, symbols, tr( "Recipient" ), true, false );

    QGridLayout* layout = new QGridLayout( 0, 1, 2 );
    name_ = new QLabel();
    layout->addWidget( new QLabel( tr( "Supervisor action:" ) ), 0, 0, Qt::AlignRight );
    layout->addWidget( name_, 0, 1, Qt::AlignLeft );

    mainLayout_->addWidget( taskerWidget_ );
    mainLayout_->addLayout( layout );
    mainLayout_->addStretch( 1 );

    // Connections
    connect( taskerWidget_, SIGNAL( ClearClicked() ), this, SLOT( OnClearTaskerClicked() ) );
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMagicWidget::~EventMagicWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::Purge
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::Purge()
{
    taskerWidget_->SetTasker( 0 );
    selectedEntity_ = 0;
    name_->clear();
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::BlockSignals
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::BlockSignals( bool blocked )
{
    taskerWidget_->BlockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::Build
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::Build( const gui::EventMagicViewState& state )
{
    // Target
    taskerWidget_->SetTasker( model_.FindEntity( state.target_ ) );
    taskerWidget_->setVisible( state.hasTarget_ );
    lastTaskerId_ = state.target_;

    // Name
    name_->setText( QString::fromStdString( state.name_ ) );
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::NotifyContextMenu
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& /*menu*/ )
{
    selectedEntity_ = &entity;
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::NotifyDeleted
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( lastTaskerId_ == entity.GetId() )
        OnTargetChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::OnTargetChanged
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::OnTargetChanged( const kernel::Entity_ABC* entity )
{
    magicPresenter_->OnTargetChanged( entity );
    presenter_.OnEventContentChanged();
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::OnReplaceTargetClicked
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::OnReplaceTargetClicked()
{
    OnTargetChanged( selectedEntity_ );
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget::OnClearTaskerClicked
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicWidget::OnClearTaskerClicked()
{
    OnTargetChanged( 0 );
}
