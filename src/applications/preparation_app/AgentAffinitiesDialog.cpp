// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentAffinitiesDialog.h"
#include "moc_AgentAffinitiesDialog.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DecimalSpinBoxAndSlider.h"
#include "preparation/AgentAffinities.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::AgentAffinitiesDialog
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinitiesDialog::AgentAffinitiesDialog( QWidget* parent, kernel::Controllers& controllers )
    : QDialog( parent, "AgentAffinitiesDialog" )
    , controllers_( controllers )
{
    setCaption( tools::translate( "AgentAffinitiesDialog", "Change affinities" ) );
    resize( 320, 150 );
    pMainLayout_ = new QVBoxLayout( this );
    pCheckBox_ = new QCheckBox( tools::translate( "AgentAffinitiesDialog", "Activate" ), this );
    connect( pCheckBox_, SIGNAL( toggled( bool ) ), this, SLOT( Actived( bool ) ) );
    pCheckBox_->setChecked( false );
    pMainLayout_->addWidget( pCheckBox_ );
    pButtonLayout_ = new QHBox( this );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), pButtonLayout_ );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), pButtonLayout_ );
    okButton->setDefault( TRUE );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::~AgentAffinitiesDialog
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinitiesDialog::~AgentAffinitiesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::Show
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::Show()
{
    if( !selected_ )
        return;
    Clear();
    selected_->Get< AgentAffinities >().Fill( affinities_ );
    BOOST_FOREACH( const T_Teams::value_type& team, teams_ )
    {
        QLabel* pLabel = new QLabel( team.second.c_str(), pGrid_ );
        pLabel->setEnabled( false );
        gui::DecimalSpinBoxAndSlider* pSplider = new gui::DecimalSpinBoxAndSlider( pGrid_, affinities_[ team.first ], 2, -1.f, 1.f, 0.01f, Qt::Horizontal, Qt::Horizontal, true );
        pSplider->setEnabled( false );
        spinboxs_.push_back( T_QAffinity( team.first, pLabel, pSplider ) );
    }
    pMainLayout_->add( pButtonLayout_ );
    Actived( selected_->Get< AgentAffinities >().HasAffinities() );
    pCheckBox_->setChecked( selected_->Get< AgentAffinities >().HasAffinities() );
    exec();
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::NotifyContextMenu
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Agent_ABC* >( &entity );;
    menu.InsertItem( "Update", tools::translate( "AgentAffinitiesDialog", "Change affinities" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::NotifyCreated
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::NotifyCreated( const kernel::Team_ABC& team )
{
    teams_[ team.GetId() ] = team.GetName();
    affinities_[ team.GetId() ] = 0.f;
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::NotifyUpdated
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::NotifyUpdated( const kernel::Team_ABC& team )
{
    teams_.erase( team.GetId() );
    affinities_.erase( team.GetId() );
    teams_[ team.GetId() ] = team.GetName();
    affinities_[ team.GetId() ] = 0.f;
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::NotifyDeleted
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::NotifyDeleted( const kernel::Team_ABC& team )
{
    teams_.erase( team.GetId() );
    affinities_.erase( team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::Actived
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::Actived( bool value )
{
    BOOST_FOREACH( T_QAffinities::value_type& content, spinboxs_ )
    {
        content.get< 1 >()->setEnabled( value );
        content.get< 2 >()->setEnabled( value );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::Validate
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::Validate()
{
    if( !selected_ )
        return;
    accept();
    selected_->Get< AgentAffinities >().Clear();
    if( pCheckBox_->isChecked () )
        BOOST_FOREACH( const T_QAffinities::value_type& content, spinboxs_ )
            selected_->Get< AgentAffinities >().Add( content.get< 0 >(), content.get< 2 >()->value() );
    controllers_.controller_.Update( *selected_ );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::Reject
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::closeEvent
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::Clear
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::Clear()
{
    pMainLayout_->remove( pGrid_ );
    pMainLayout_->remove( pButtonLayout_ );
    if( pGrid_ != 0 )
        delete pGrid_;
    BOOST_FOREACH( T_Affinities::value_type& content, affinities_ )
        content.second = 0.f;
    pGrid_ = new QGrid( 2, this );
    pMainLayout_->add( pGrid_ );
}
