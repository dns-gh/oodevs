//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DisconnectDialog.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_DisconnectDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_DisconnectDialog.h"
#include "moc_MOS_DisconnectDialog.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_DisconnectDialog.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_Config.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_SIMControl.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>

//-----------------------------------------------------------------------------
// Name: MOS_DisconnectDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_DisconnectDialog::MOS_DisconnectDialog( QWidget* pParent )
    : QDialog( pParent ) 
{
    setCaption( tr("Disconnect") );

    QGridLayout* pMainLayout = new QGridLayout( this, 5, 5 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    uint nCurRow = 0;

    // Buttons
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pMainLayout->addWidget( pCancelButton, nCurRow, 0 );
    pMainLayout->addWidget( pOKButton, nCurRow, 1     );
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

    MOS_App::GetApp().GetMOSServer().Disconnect();
}

//-----------------------------------------------------------------------------
// Name: MOS_DisconnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_DisconnectDialog::Reject()
{
    reject();
}

