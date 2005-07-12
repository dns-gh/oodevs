//****************************************************************************
//
// This file is part of MASA DIN library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBNumberSelector.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_ODBNumberSelector.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#	pragma implementation
#endif

#include "MOS_Light_Pch.h"

#include "MOS_ODBNumberSelector.h"
#include "moc_MOS_ODBNumberSelector.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBNumberSelector.inl"
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
// Name: MOS_ODBNumberSelector constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_ODBNumberSelector::MOS_ODBNumberSelector( uint* pNumber, QWidget* pParent )
:   QDialog( pParent ) 
,   pNumber_( pNumber )
{
    setCaption( tr("Select Pawns Number") );

    QGridLayout* pMainLayout = new QGridLayout( this, 5, 5 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    uint nCurRow = 0;
    
    // Port
    pPortSpinBox_ = new QSpinBox( 1, 65535, 1, this );
    pMainLayout->addWidget( new QLabel( tr("Pions"), this ), nCurRow, 0 );
    pMainLayout->addWidget( pPortSpinBox_, nCurRow, 1 );

    // Buttons
    ++ nCurRow;
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pMainLayout->addWidget( pCancelButton, nCurRow, 0 );
    pMainLayout->addWidget( pOKButton, nCurRow, 1     );
    pOKButton->setDefault( TRUE );
    
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBNumberSelector destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_ODBNumberSelector::~MOS_ODBNumberSelector()
{
    
}

//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_ODBNumberSelector::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ODBNumberSelector::Validate()
{
    accept();
    *pNumber_ = pPortSpinBox_->value();
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBNumberSelector::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ODBNumberSelector::Reject()
{
    reject();
}

