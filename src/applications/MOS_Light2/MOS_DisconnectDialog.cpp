//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DisconnectDialog.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 15:56 $
// $Revision: 3 $
// $Workfile: MOS_DisconnectDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_DisconnectDialog.h"
#include "moc_MOS_DisconnectDialog.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Network.h"

//-----------------------------------------------------------------------------
// Name: MOS_DisconnectDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_DisconnectDialog::MOS_DisconnectDialog( QWidget* pParent )
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
// Name: MOS_DisconnectDialog destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_DisconnectDialog::~MOS_DisconnectDialog()
{
}


//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DisconnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_DisconnectDialog::Validate()
{
    accept();
    MOS_App::GetApp().GetNetwork().Disconnect();
}


//-----------------------------------------------------------------------------
// Name: MOS_DisconnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_DisconnectDialog::Reject()
{
    reject();
}

