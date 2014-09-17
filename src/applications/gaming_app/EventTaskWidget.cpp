// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventTaskWidget.h"
#include "moc_EventTaskWidget.cpp"
#include "ENT/ENT_Tr.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventTaskPresenter.h"
#include "clients_gui/EventTaskViewState.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichTextEdit.h"
#include "clients_gui/SignalAdapter.h"
#include "clients_gui/TaskerWidget.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: EventTaskWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTaskWidget::EventTaskWidget( gui::EventPresenter& presenter,
                                  kernel::Controllers& controllers,
                                  const gui::EntitySymbols& symbols,
                                  const kernel::Profile_ABC& profile,
                                  const kernel::EntityResolver_ABC& model,
                                  const kernel::Time_ABC& simulation )
    : EventTaskWidget_ABC( presenter )
    , controllers_( controllers )
    , profile_( profile )
    , model_( model )
    , simulation_( simulation )
    , selectedEntity_( controllers )
{
    // Presenter
    taskPresenter_ = boost::make_shared< gui::EventTaskPresenter >( *this, controllers, model );
    presenter_.AddSubPresenter( taskPresenter_ );

    // Tasker
    taskerWidget_ = new gui::TaskerWidget( "task-tasker", controllers, symbols, tr( "Recipient" ) );

    // Editors
    label_ = new gui::RichLineEdit( "task-label" );
    description_ = new gui::RichTextEdit( "task-description" );
    url_ = new gui::RichLineEdit( "task-url" );
    bytes_ = new QLabel();
    bytes_->setEnabled( false );
    showButton_ = new QPushButton( tr( "Show" ) );
    payload_ = new gui::RichTextEdit( "task-payload" );
    payload_->setVisible( false );
    payloadLabel_ = new QLabel( tr( "Base64 Payload" ) );

    // Layouts
    QGridLayout* gridLayout = new QGridLayout( 3, 2 );
    gridLayout->setSpacing( 5 );
    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );

    QGroupBox* actionBox = new QGroupBox( tr( "Action" ) );
    QVBoxLayout* actionLayout = new QVBoxLayout( actionBox );
    QHBoxLayout* urlLayout = new QHBoxLayout( actionLayout );
    QHBoxLayout* payloadLayout = new QHBoxLayout( actionLayout );

    urlLayout->addWidget( new QLabel( tr( "Url" ) ) );
    urlLayout->addWidget( url_ );

    payloadLayout->addWidget( payloadLabel_ );
    payloadLayout->addWidget( bytes_ );
    payloadLayout->addWidget( showButton_ );

    actionLayout->addWidget( payload_ );

    mainLayout_->addWidget( taskerWidget_ );
    mainLayout_->addLayout( gridLayout, 1 );
    mainLayout_->addWidget( actionBox );

    // Connections
    gui::connect( taskerWidget_, SIGNAL( ClearClicked() ), [&](){ OnTargetChanged( 0 ); } );
    connect( label_,       SIGNAL( textChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnLabelChanged( const QString& ) ) );
    connect( url_,         SIGNAL( textChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnUrlChanged( const QString& ) ) );
    connect( showButton_,  SIGNAL( clicked() ),                     taskPresenter_.get(), SLOT( OnShowClicked() ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnDescriptionChanged( const QString& ) ) );
    connect( payload_,     SIGNAL( TextChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnPayloadChanged( const QString& ) ) );
    // We want theses slots to be called after those from task presenter
    connect( label_,       SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( url_,         SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( payload_,     SIGNAL( TextChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTaskWidget::~EventTaskWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Purge
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::Purge()
{
    taskerWidget_->SetTasker( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::BlockSignals
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::BlockSignals( bool blocked )
{
    label_->blockSignals( blocked );
    description_->blockSignals( blocked );
    url_->blockSignals( blocked );
    payload_->blockSignals( blocked );
    bytes_->blockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Build
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::Build( const gui::EventTaskViewState& state )
{
    taskerWidget_->SetTasker( model_.FindEntity( state.target_ ) );
    lastTaskerId_ = state.target_;
    label_->SetText( QString::fromStdString( state.label_ ) );
    description_->SetText( QString::fromStdString( state.description_ ) );
    url_->SetText( QString::fromStdString( state.url_ ) );
    bytes_->clear();
    if( state.bytes_ != 0 )
        bytes_->setText( QString::number( state.bytes_ ) + QString( " bytes" ) );
    payload_->SetText( QString::fromStdString( state.payload_ ) );
    showButton_->setText( state.isPayloadVisible_ ? tr( "Hide" ) : tr( "Show" ) );
    showButton_->setVisible( state.isUrlValid_ );
    bytes_->setVisible( state.isUrlValid_ );
    payloadLabel_->setVisible( state.isUrlValid_ );
    payload_->setVisible( state.isUrlValid_ && state.isPayloadVisible_ );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    AddContextMenu( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    AddContextMenu( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    AddContextMenu( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyContextMenu( const kernel::Inhabitant_ABC& inhabitant, kernel::ContextMenu& menu )
{
    AddContextMenu( inhabitant, menu );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyContextMenu( const kernel::Population_ABC& population, kernel::ContextMenu& menu )
{
    AddContextMenu( population, menu );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    AddContextMenu( team, menu );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::AddContextMenu
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::AddContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selectedEntity_ = &entity;
    if( profile_.CanBeOrdered( entity ) )
    {
        menu.InsertItem( "Order", tr( "New task" ), this, SLOT( OnNewTaskClicked() ), false, 5 );
        if( isVisible() )
        {
            auto* tasker = taskerWidget_->GetTasker();
            if( !tasker || tasker->GetId() != selectedEntity_->GetId() )
                menu.InsertItem( "Order", tr( "Replace task recipient" ), this, SLOT( OnReplaceTaskerClicked() ), false, 7 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::NotifyDeleted
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( lastTaskerId_ == entity.GetId() )
        OnTargetChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnTargetChanged
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskWidget::OnTargetChanged( const kernel::Entity_ABC* entity )
{
    taskPresenter_->OnTargetChanged( entity );
    presenter_.OnEventContentChanged();
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnNewTaskClicked
// Created: ABR 2014-09-10
// -----------------------------------------------------------------------------
void EventTaskWidget::OnNewTaskClicked()
{
    presenter_.StartCreation( eEventTypes_Task, simulation_.GetDateTime() );
    OnTargetChanged( selectedEntity_ );
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnReplaceTaskerClicked
// Created: ABR 2014-09-10
// -----------------------------------------------------------------------------
void EventTaskWidget::OnReplaceTaskerClicked()
{
    OnTargetChanged( selectedEntity_ );
    selectedEntity_ = 0;
}
