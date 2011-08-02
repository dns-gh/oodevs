// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::PresetDialog */

#include "clients_gui_pch.h"
#include "PresetDialog.h"
#include "moc_PresetDialog.cpp"
#include "Gradient.h"
#include "clients_kernel/Options.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PresetDialog constructor
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
PresetDialog::PresetDialog( QWidget* parent, Gradient& gradient, kernel::Options& options,
                            const std::vector< std::string >& presets )
    : QDialog( parent )
    , pLayout_ ( new Q3VBoxLayout( this ) )
    , gradient_( gradient )
    , options_ ( options )
    , presets_ ( presets )
{
    setFixedSize( 300, 50 );
    setCaption( tr( "Rename preset" ) );
    setModal( true );
    CreateContent();
    CreateButton();
}

// -----------------------------------------------------------------------------
// Name: PresetDialog destructor
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
PresetDialog::~PresetDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PresetDialog::CreateContent
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void PresetDialog::CreateContent()
{
    Q3HBoxLayout* pEditLayout = new Q3HBoxLayout( pLayout_ );
    QLabel* pLabel = new QLabel( this );
    pLabel->setText( tr( "New Name:" ) );
    pEditLayout->addWidget( pLabel );
    pLineEdit_ = new QLineEdit( this );
    pEditLayout->addWidget( pLineEdit_ );
}

// -----------------------------------------------------------------------------
// Name: PresetDialog::CreateButton
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void PresetDialog::CreateButton()
{
    Q3HBoxLayout* pButtonLayout = new Q3HBoxLayout( pLayout_ );
    pAcceptButton_ = new QPushButton( tr( "Ok" ), this );
    pAcceptButton_->setDefault( true );
    pAcceptButton_->setMaximumWidth( 100 );
    pRejectButton_ = new QPushButton( tr( "Cancel" ), this );
    pRejectButton_->setMaximumWidth( 100 );
    pButtonLayout->addWidget( pAcceptButton_ );
    pButtonLayout->addWidget( pRejectButton_ );
    connect( pAcceptButton_, SIGNAL( clicked() ), this, SLOT( Accept() ) );
    connect( pRejectButton_, SIGNAL( clicked() ), this, SLOT( Reject() ) );
}

// -----------------------------------------------------------------------------
// Name: PresetDialog::Accept
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void PresetDialog::Accept()
{
    if( pLineEdit_->text().isEmpty() )
        QMessageBox::warning( this, tr( "Warning" ), tr( "Preset name is empty.") );
    else if( std::find( presets_.begin(), presets_.end(), pLineEdit_->text().ascii() ) != presets_.end() )
        QMessageBox::warning( this, tr( "Warning" ), tr( "Preset name already exists." ) );
    else
    {
        options_.Remove( std::string( "Gradients/" ) + gradient_.GetName().ascii() );
        gradient_.SetName( pLineEdit_->text() );
        QDialog::accept();
    }
}

// -----------------------------------------------------------------------------
// Name: PresetDialog::Reject
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void PresetDialog::Reject()
{
    QDialog::reject();
}
