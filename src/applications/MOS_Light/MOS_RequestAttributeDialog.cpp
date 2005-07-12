//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RequestAttributeDialog.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_RequestAttributeDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_RequestAttributeDialog.h"
#include "moc_MOS_RequestAttributeDialog.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_RequestAttributeDialog.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_Agent.h"

#include "DIN/DIN_OutputHelper.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_RequestAttributeDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_RequestAttributeDialog::MOS_RequestAttributeDialog( QWidget* pParent )
    : MOS_AgentDialog_ABC( pParent )
{
/*    setCaption( tr("Unit attribute request") );

    //--------------------
    // Checkboxes
    QGridLayout* pMainLayout = new QGridLayout( this, 5, 5 );
    uint nCurRow = 0;
    pMainLayout->addRowSpacing( nCurRow++, 20 );

    for( uint i = 0; i < eNbrAttributes; ++i )
    {
        std::string strLabel = MOS_Agent::StaticGetAttributeAsString( (E_Attribute)i );
        pMainLayout->addWidget( new QLabel( tr( strLabel.c_str() ), this  ), nCurRow, 0 );
   
        QCheckBox* pCheckbox = new QCheckBox( this, "testzer" );
        pMainLayout->addWidget( pCheckbox, nCurRow, 1, Qt::AlignHCenter );
        pMainLayout->addRowSpacing( nCurRow++, 20 );
        attributeCheckBoxVector_.push_back( pCheckbox );
    }

    //--------------------
    // OK / Cancel
    ++ nCurRow;
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pMainLayout->addWidget( pCancelButton, nCurRow, 0 );
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pMainLayout->addWidget( pOKButton, nCurRow, 1 );
    pMainLayout->addRowSpacing( ++nCurRow, 20 );
    pOKButton->setDefault( TRUE );
    
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
*/

}

//-----------------------------------------------------------------------------
// Name: MOS_RequestAttributeDialog destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_RequestAttributeDialog::~MOS_RequestAttributeDialog()
{
    
}


//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_RequestAttributeDialog::SlotValidate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_RequestAttributeDialog::SlotValidate()
{
    assert( pAgent_ );

    DIN_BufferedMessage msg = pAgent_->PrebuildAttributeRequestMsg();

    DIN_OutputHelper< uint8 > valueWriterHelper( msg.GetOutput() );

    uint8 nNbrAttrs = 0;
    MIL_AttributeID nAttrID = 0;
    for( IT_CheckBoxVector itCheckBox = attributeCheckBoxVector_.begin(); itCheckBox != attributeCheckBoxVector_.end(); ++itCheckBox )
    {
        QCheckBox* pCheckBox = *itCheckBox;
        if( pCheckBox->isChecked() )
        {
            msg << nAttrID;
            ++nNbrAttrs;
        }
        ++nAttrID;
    }

    valueWriterHelper << nNbrAttrs;

    pAgent_->SendAttributeRequestMsg( msg );

    accept();
  
    Cleanup();
}


//-----------------------------------------------------------------------------
// Name: MOS_RequestAttributeDialog::SlotCancel
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_RequestAttributeDialog::SlotCancel()
{
    Cleanup();
    reject();
}


