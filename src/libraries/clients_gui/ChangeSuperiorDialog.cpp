// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ChangeSuperiorDialog.h"
#include "moc_ChangeSuperiorDialog.cpp"
#include "ChangeSuperior_ABC.h"
#include "tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog constructor
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
ChangeSuperiorDialog::ChangeSuperiorDialog( QWidget* parent, kernel::Controllers& controllers, ChangeSuperior_ABC& changeSuperior, bool communication )
    : QDialog( parent )
    , controllers_   ( controllers )
    , changeSuperior_( changeSuperior )
    , entity_        ( 0 )
    , selected_      ( 0 )
{
    if( communication )
        setWindowTitle( tr( "Select new group") );
    else
        setWindowTitle( tr( "Select new superior") );
    setMinimumWidth( 300 );
    QGridLayout* layout = new QGridLayout( this );
    QGroupBox* box = new QGroupBox( tr( "Superior" ), this );
    QBoxLayout* boxLayout = new QBoxLayout( QBoxLayout::TopToBottom, box );
    superiorLabel_ = new QLabel( "---" );
    boxLayout->addWidget( superiorLabel_ );
    superiorLabel_->setAlignment( Qt::AlignCenter );
    okButton_ = new QPushButton( tr( "Ok" ), this );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), this );
    layout->addWidget( box, 0, 0, 1, 2 );
    layout->addWidget( okButton_, 1, 0 );
    layout->addWidget( cancelButton, 1, 1 );
    okButton_->setDefault( true );
    connect( okButton_, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog destructor
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
ChangeSuperiorDialog::~ChangeSuperiorDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::Show
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::Show( kernel::Entity_ABC& entity )
{
    superiorLabel_->setText( "---" );
    selected_ = 0;
    entity_ = &entity;
    okButton_->setEnabled( false );
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::NotifySelected
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    okButton_->setEnabled( false );
    selected_ = 0;
    if( entity_ && isVisible() )
    {
        if( element )
        {
            bool canChange = changeSuperior_.CanChangeSuperior( *entity_, *element );
            if( canChange )
            {
                selected_ = const_cast< kernel::Entity_ABC* >( element );
                okButton_->setEnabled( true );
            }
            superiorLabel_->setText( QString( "<font color=\"#" ) + ( canChange ? "000000\">" : "FF0000\">" ) + element->GetName() + "</font>" );
        }
        else
            superiorLabel_->setText( "---" );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::NotifyDeleted
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( isVisible() )
    {
        if( &entity == entity_ )
            hide();
        else if( &entity == selected_ )
            Show( *entity_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::OnOk
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::OnOk()
{
    if( entity_ && selected_ )
        changeSuperior_.DoChangeSuperior( *entity_, *selected_ );
    hide();
    entity_ = 0;
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ChangeSuperiorDialog::OnCancel
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void ChangeSuperiorDialog::OnCancel()
{
    hide();
}
