// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeAffinitiesDialog.h"
#include "moc_ChangeAffinitiesDialog.cpp"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_gui/DecimalSpinBoxAndSlider.h"
#include "gaming/Affinities.h"

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
ChangeAffinitiesDialog::ChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers )
    : QDialog( pParent, tools::translate( "ChangeAffinitiesDialog", "Change affinities" ), true )
    , selected_      ( controllers )
    , affinitiesGrid_( 0 )
{
    setCaption( tools::translate( "ChangeAffinitiesDialog", "Change affinities" ) );
    resize( 320, 150 );
    mainLayout_ = new QVBoxLayout( this );
    buttonLayout_ = new QHBox( this );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), buttonLayout_ );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout_ );
    okButton->setDefault( TRUE );
    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    selected_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
ChangeAffinitiesDialog::~ChangeAffinitiesDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog::Visit
// Created: LGY 2011-05-06
// -----------------------------------------------------------------------------
void ChangeAffinitiesDialog::Visit( unsigned long id, const std::string& team, float& affinity )
{
    new QLabel( team.c_str(), affinitiesGrid_ );
    affinitiesSpinboxs_[ id ] = new gui::DecimalSpinBoxAndSlider( affinitiesGrid_, affinity, 2, -1.f, 1.f, 0.01f, Qt::Horizontal, Qt::Horizontal, true );
    affinitiesSpinboxs_[ id ]->setMargin( 5 );
    affinitiesSpinboxs_[ id ]->setSpacing( 5 );
}

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeAffinitiesDialog::Show()
{
    if( !selected_ )
        return;
    affinitiesSpinboxs_.clear();
    mainLayout_->remove( affinitiesGrid_ );
    mainLayout_->remove( buttonLayout_ );
    if( affinitiesGrid_ != 0 )
        delete affinitiesGrid_;
    affinitiesGrid_ = new QGrid( 2, this );
    mainLayout_->add( affinitiesGrid_ );
    mainLayout_->add( buttonLayout_ );
    selected_.ConstCast()->Get< Affinities >().Accept( *this );
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeAffinitiesDialog::Validate()
{
    if( ! selected_ )
        return;
    accept();
    DoValidate();
}

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeAffinitiesDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ChangeAffinitiesDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeAffinitiesDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}
