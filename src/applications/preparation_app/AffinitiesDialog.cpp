// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AffinitiesDialog.h"
#include "moc_AffinitiesDialog.cpp"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DecimalSpinBoxAndSlider.h"
#include "preparation/Affinities.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog constructor
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
AffinitiesDialog::AffinitiesDialog( QWidget* parent, kernel::Controllers& controllers, bool optional )
    : QDialog( parent, "AffinitiesDialog" )
    , controllers_( controllers )
    , pGrid_      ( 0 )
    , selected_   ( controllers )
{
    setCaption( tools::translate( "AffinitiesDialog", "Change affinities" ) );
    resize( 320, 150 );
    pMainLayout_ = new QVBoxLayout( this );
    pCheckBox_ = new QCheckBox( tools::translate( "AffinitiesDialog", "Activate" ), this );
    connect( pCheckBox_, SIGNAL( toggled( bool ) ), this, SLOT( Activated( bool ) ) );
    pCheckBox_->setChecked( false );
    pCheckBox_->setShown( optional );
    pMainLayout_->addWidget( pCheckBox_ );
    pButtonLayout_ = new QHBox( this );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), pButtonLayout_ );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), pButtonLayout_ );
    okButton->setDefault( TRUE );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog destructor
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
AffinitiesDialog::~AffinitiesDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::NotifyCreated
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::NotifyCreated( const kernel::Team_ABC& team )
{
    teams_[ team.GetId() ] = team.GetName();
    affinities_[ team.GetId() ] = 0.f;
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::NotifyUpdated
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::NotifyUpdated( const kernel::Team_ABC& team )
{
    teams_.erase( team.GetId() );
    teams_[ team.GetId() ] = team.GetName();
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::NotifyDeleted
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::NotifyDeleted( const kernel::Team_ABC& team )
{
    teams_.erase( team.GetId() );
    affinities_.erase( team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::Show
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::Show()
{
    if( !selected_ )
        return;
    Clear();
    selected_.ConstCast()->Get< Affinities >().Fill( affinities_ );
    BOOST_FOREACH( const T_Teams::value_type& team, teams_ )
    {
        QLabel* pLabel = new QLabel( team.second.c_str(), pGrid_ );
        pLabel->setEnabled( false );
        gui::DecimalSpinBoxAndSlider* pSplider = new gui::DecimalSpinBoxAndSlider( pGrid_, affinities_[ team.first ], 2, -1.f, 1.f, 0.01f, Qt::Horizontal, Qt::Horizontal, true );
        pSplider->setEnabled( false );
        spinboxs_.push_back( T_QAffinity( team.first, pLabel, pSplider ) );
    }
    pMainLayout_->add( pButtonLayout_ );
    Activated( selected_->Get< Affinities >().HasAffinities() );
    pCheckBox_->setChecked( selected_->Get< Affinities >().HasAffinities() );
    exec();
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::Activated
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::Activated( bool value )
{
    BOOST_FOREACH( T_QAffinities::value_type& content, spinboxs_ )
    {
        content.get< 1 >()->setEnabled( value );
        content.get< 2 >()->setEnabled( value );
    }
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::Validate
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::Validate()
{
    if( !selected_ )
        return;
    accept();
    selected_.ConstCast()->Get< Affinities >().Clear();
    if( pCheckBox_->isChecked () )
        BOOST_FOREACH( const T_QAffinities::value_type& content, spinboxs_ )
            selected_.ConstCast()->Get< Affinities >().Add( content.get< 0 >(), content.get< 2 >()->value() );
    controllers_.controller_.Update( *selected_ );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::Reject
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::closeEvent
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: AffinitiesDialog::Clear
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AffinitiesDialog::Clear()
{
    pMainLayout_->remove( pGrid_ );
    pMainLayout_->remove( pButtonLayout_ );
    if( pGrid_ )
        delete pGrid_;
    spinboxs_.clear();
    BOOST_FOREACH( T_Affinities::value_type& content, affinities_ )
        content.second = 0.f;
    pGrid_ = new QGrid( 2, this );
    pMainLayout_->add( pGrid_ );
}
