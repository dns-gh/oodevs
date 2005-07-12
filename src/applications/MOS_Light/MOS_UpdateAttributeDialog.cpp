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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_UpdateAttributeDialog.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 12 $
// $Workfile: MOS_UpdateAttributeDialog.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#	pragma implementation
#endif

#include "MOS_Light_Pch.h"

#include "MOS_UpdateAttributeDialog.h"
#include "moc_MOS_UpdateAttributeDialog.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_UpdateAttributeDialog.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_Agent.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qwidgetstack.h>

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_UpdateAttributeDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_UpdateAttributeDialog::MOS_UpdateAttributeDialog( QWidget* pParent )
    : MOS_AgentDialog_ABC( pParent )
{
    setCaption( tr("Unit attribute update") );

    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );

    // First layout
    QGroupBox* pFirstGroupBox_ = new QGroupBox( this );
    pMainLayout->addWidget( pFirstGroupBox_ );

    QGridLayout* pFirstLayout = new QGridLayout( pFirstGroupBox_, 5, 5 );
    pFirstLayout->setMargin( 20 );

    int nCurRow = 0;

    //--------------------
    // Attribute
    pFirstLayout->addWidget( new QLabel( tr("Attribute" ), pFirstGroupBox_ ), nCurRow, 0 );
    pAttributesComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pAttributesComboBox_->insertItem( "Position" );
    pAttributesComboBox_->insertItem( "Carburant" );
    pAttributesComboBox_->insertItem( "Munitions" );
    pAttributesComboBox_->insertItem( "Capacite communication" );
    pAttributesComboBox_->insertItem( "Capacite feu" );
    pAttributesComboBox_->insertItem( "Capacite detection" );
    pAttributesComboBox_->insertItem( "Capacite protection" );
    pFirstLayout->addWidget( pAttributesComboBox_, nCurRow, 1 );
    pFirstLayout->addRowSpacing( (++nCurRow)++, 20 );
    connect( pAttributesComboBox_, SIGNAL( activated( int ) ), SLOT( SlotAttributeChange( int ) ) );

    //--------------------
    // Attribute value
    pFirstLayout->addWidget( new QLabel( tr("Attribute value" ), pFirstGroupBox_ ), nCurRow, 0 );
    pAttributeValueWidgetStack_ = new QWidgetStack( pFirstGroupBox_ );
    pFirstLayout->addWidget( pAttributeValueWidgetStack_, nCurRow, 1 );

    pAttributeValuePosition_  = new QComboBox( TRUE, pAttributeValueWidgetStack_ );
    InitPositionComboBox( *pAttributeValuePosition_ );
    pAttributeValueCarburant_               = new QSpinBox( 1, 100000, 1, pAttributeValueWidgetStack_ );
    pAttributeValueMunition_                = new QSpinBox( 1, 100000, 1, pAttributeValueWidgetStack_ );
    pAttributeValueCapacityCommunication_   = new QSpinBox( 0, 100, 1, pAttributeValueWidgetStack_ );
    pAttributeValueCapacityFire_            = new QSpinBox( 0, 100, 1, pAttributeValueWidgetStack_ );
    pAttributeValueCapacityDetection_       = new QSpinBox( 0, 100, 1, pAttributeValueWidgetStack_ );
    pAttributeValueCapacityProtection_      = new QSpinBox( 0, 100, 1, pAttributeValueWidgetStack_ );
    
    pAttributeValueWidgetStack_->addWidget( pAttributeValuePosition_ );
    pAttributeValueWidgetStack_->addWidget( pAttributeValueCarburant_ );
    pAttributeValueWidgetStack_->addWidget( pAttributeValueCarburant_ );
    pAttributeValueWidgetStack_->addWidget( pAttributeValueCapacityCommunication_ );
    pAttributeValueWidgetStack_->addWidget( pAttributeValueCapacityFire_ );
    pAttributeValueWidgetStack_->addWidget( pAttributeValueCapacityDetection_ );
    pAttributeValueWidgetStack_->addWidget( pAttributeValueCapacityProtection_ );
    pAttributeValueWidgetStack_->raiseWidget( pAttributeValuePosition_ );

    // Second layout
    QHBoxLayout* pSecondLayout = new QHBoxLayout( pMainLayout );

    //--------------------
    // OK / Cancel
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pSecondLayout->addWidget( pCancelButton );
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pSecondLayout->addWidget( pOKButton );
    pOKButton->setDefault( TRUE );
    
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_UpdateAttributeDialog destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_UpdateAttributeDialog::~MOS_UpdateAttributeDialog()
{
    
}

//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_UpdateAttributeDialog::SlotAttributeChange
// Created: NLD 2002-09-17
//-----------------------------------------------------------------------------
void MOS_UpdateAttributeDialog::SlotAttributeChange( int nIndex )
{
    if( nIndex == 0 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValuePosition_ );
    else if( nIndex == 1 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValueCarburant_ );
    else if( nIndex == 2 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValueMunition_ );   
    else if( nIndex == 3 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValueCapacityCommunication_ );
    else if( nIndex == 4 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValueCapacityFire_ );
    else if( nIndex == 5 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValueCapacityDetection_ );
    else if( nIndex == 6 )
        pAttributeValueWidgetStack_->raiseWidget( pAttributeValueCapacityProtection_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_UpdateAttributeDialog::SlotValidate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_UpdateAttributeDialog::SlotValidate()
{
    assert( pAgent_ );

    DIN_BufferedMessage msg = pAgent_->PrebuildAttributeUpdateMsg();

    int nIdx = pAttributesComboBox_->currentItem();
    // Position
    if( nIdx == 0 )
    {
        msg << (MIL_AttributeID)eAttrPosition;
        std::string strValue( pAttributeValuePosition_->currentText() );
        msg << ConvertPositionValue( strValue );
    }
    else if( nIdx == 1 )
    {
        msg << (MIL_AttributeID)eAttrDotationCarburant;
        msg << (uint32)pAttributeValueCarburant_->value();
    }
    else if( nIdx == 2 )
    {
        msg << (MIL_AttributeID)eAttrDotationMunition;
        msg << (uint8)0; //$$$ dummy 
        msg << (uint32)pAttributeValueMunition_->value();
    }
    else if( nIdx == 3 )
    {
        msg << (MIL_AttributeID)eAttrCapacityCommunication;
        msg << (uint8)pAttributeValueCapacityCommunication_->value();
    }
    else if( nIdx == 4 )
    {
        msg << (MIL_AttributeID)eAttrCapacityFire;
        msg << (uint8)pAttributeValueCapacityFire_->value();
    }
    else if( nIdx == 5 )
    {
        msg << (MIL_AttributeID)eAttrCapacityDetection;
        msg << (uint8)pAttributeValueCapacityDetection_->value();
    }
    else if( nIdx == 6 )
    {
        msg << (MIL_AttributeID)eAttrCapacityProtection;
        msg << (uint8)pAttributeValueCapacityProtection_->value();
    }
    else
    {
        assert( false );
    }

    pAgent_->SendAttributeUpdateMsg( msg );

    accept();
  
    Cleanup();
}


//-----------------------------------------------------------------------------
// Name: MOS_UpdateAttributeDialog::SlotCancel
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_UpdateAttributeDialog::SlotCancel()
{
    Cleanup();
    reject();
}


