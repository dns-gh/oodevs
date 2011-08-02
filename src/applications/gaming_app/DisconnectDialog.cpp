// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DisconnectDialog.h"
#include "moc_DisconnectDialog.cpp"
#include "gaming/Network.h"

//-----------------------------------------------------------------------------
// Name: DisconnectDialog constructor
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
DisconnectDialog::DisconnectDialog( QWidget* pParent, Network& network )
    : QDialog( pParent, 0, true, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title )
    , network_( network )
{
    setCaption( tr("Disconnect") );

    Q3GridLayout* pMainLayout = new Q3GridLayout( this, 5, 5 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    // Buttons
    QPushButton* pOKButton     = new QPushButton( tr("Ok")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pMainLayout->addWidget( pOKButton,     0, 0 );
    pMainLayout->addWidget( pCancelButton, 0, 1 );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
}

//-----------------------------------------------------------------------------
// Name: DisconnectDialog destructor
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
DisconnectDialog::~DisconnectDialog()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DisconnectDialog::Validate
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
void DisconnectDialog::Validate()
{
    network_.Disconnect();
    accept();
}

//-----------------------------------------------------------------------------
// Name: DisconnectDialog::Validate
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
void DisconnectDialog::Reject()
{
    reject();
}

