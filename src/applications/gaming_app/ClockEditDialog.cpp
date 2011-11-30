// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ClockEditDialog.h"
#include "moc_ClockEditDialog.cpp"
#include "gaming/ActionsScheduler.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ClockEditDialog constructor
// Created: SBO 2010-05-31
// -----------------------------------------------------------------------------
ClockEditDialog::ClockEditDialog( QWidget* parent, kernel::Controllers& controllers, ActionsScheduler& scheduler )
    : QDialog( parent, "ClockEditDialog" )
    , controllers_( controllers )
    , scheduler_  ( scheduler )
{
    setCaption( tr( "Edit current time" ) );
    Q3GridLayout* pLayout = new Q3GridLayout( this, 2, 1 );
    pLayout->setMargin( 10 );
    pLayout->setSpacing( 10 );

    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "ClockEditDialog", "Set current date and time: " ), this );
    {
        new QLabel( tools::translate( "ClockEditDialog", "New date and time: " ), box );
        editor_ = new Q3DateTimeEdit( box );
    }
    pLayout->addWidget( box, 0, 0 );
    Q3HBox* buttons = new Q3HBox( this );
    {
        QPushButton* ok = new QPushButton( tools::translate( "ClockEditDialog", "Ok" ), buttons );
        QPushButton* cancel = new QPushButton( tools::translate( "ClockEditDialog", "Cancel" ), buttons );
        connect( ok, SIGNAL( clicked() ), SLOT( OnCommit() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( OnCancel() ) );
    }
    pLayout->addWidget( buttons, 1, 0 );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ClockEditDialog destructor
// Created: SBO 2010-05-31
// -----------------------------------------------------------------------------
ClockEditDialog::~ClockEditDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ClockEditDialog::OnCommit
// Created: SBO 2010-05-31
// -----------------------------------------------------------------------------
void ClockEditDialog::OnCommit()
{
    scheduler_.SetDate( editor_->dateTime() );
    accept();
}

// -----------------------------------------------------------------------------
// Name: ClockEditDialog::OnCancel
// Created: SBO 2010-05-31
// -----------------------------------------------------------------------------
void ClockEditDialog::OnCancel()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: ClockEditDialog::NotifyUpdated
// Created: SBO 2010-05-31
// -----------------------------------------------------------------------------
void ClockEditDialog::NotifyUpdated( const Simulation& simulation )
{
    if( !isShown() )
        editor_->setDateTime( simulation.GetDateTime() );
}
