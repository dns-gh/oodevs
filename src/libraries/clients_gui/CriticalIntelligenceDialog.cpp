// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CriticalIntelligenceDialog.h"
#include "moc_CriticalIntelligenceDialog.cpp"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "RichPushButton.h"
#include "SubObjectName.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog constructor
// Created: ABR 2012-07-06
// -----------------------------------------------------------------------------
CriticalIntelligenceDialog::CriticalIntelligenceDialog( QWidget* parent, kernel::Controllers& controllers )
    : gui::PropertyDialog( parent )
    , controllers_         ( controllers )
    , criticalIntelligence_( 0 )
{
    SubObjectName subObject( "CriticalIntelligenceDialog" );
    setCaption( tr( "Change critical intelligence" ) );
    resize( 300, 150 );

    textEdit_ = new QTextEdit();
    RichPushButton* okButton = new RichPushButton( "ok", tr( "Ok" ) );
    RichPushButton* cancelButton = new RichPushButton( "cancel", tr( "Cancel" ) );
    okButton->setDefault( true );
    connect( okButton, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( OnReject() ) );

    QGridLayout* pLayout = new QGridLayout( this, 2, 2, 4 );
    pLayout->addMultiCellWidget( textEdit_, 0, 0, 0, 1 );
    pLayout->addWidget( okButton, 1, 0, Qt::AlignCenter );
    pLayout->addWidget( cancelButton, 1, 1, Qt::AlignCenter );
    pLayout->setRowStretch( 0, 1 );
    pLayout->setRowStretch( 1, 0 );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog destructor
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
CriticalIntelligenceDialog::~CriticalIntelligenceDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::SetValue
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::SetValue( kernel::CriticalIntelligenceType& value )
{
    criticalIntelligence_ = &value;
    textEdit_->setText( value() );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::GetValue
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
kernel::CriticalIntelligenceType CriticalIntelligenceDialog::GetValue()
{
    return textEdit_->text();
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::OnAccept
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::OnAccept()
{
    assert( criticalIntelligence_ != 0 );
    *criticalIntelligence_ = textEdit_->text();
    controllers_.controller_.Update( 0 );
    accept();
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligenceDialog::OnReject
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
void CriticalIntelligenceDialog::OnReject()
{
    criticalIntelligence_ = 0;
    reject();
}
