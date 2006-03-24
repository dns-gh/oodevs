//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/DisconnectDialog.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 15:56 $
// $Revision: 3 $
// $Workfile: DisconnectDialog.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "DisconnectDialog.h"
#include "moc_DisconnectDialog.cpp"

#include "App.h"
#include "MainWindow.h"
#include "Network.h"

//-----------------------------------------------------------------------------
// Name: DisconnectDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
DisconnectDialog::DisconnectDialog( QWidget* pParent )
    : QDialog( pParent ) 
{
    setCaption( tr("Déconnexion") );

    QGridLayout* pMainLayout = new QGridLayout( this, 5, 5 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    // Buttons
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
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
    accept();
    App::GetApp().GetNetwork().Disconnect(); // $$$$ AGE 2006-03-24: 
}


//-----------------------------------------------------------------------------
// Name: DisconnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void DisconnectDialog::Reject()
{
    reject();
}

