// *****************************************************************************
//
// $Created: NLD 2003-08-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_Editor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_DynaObject_Editor.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_DynaObject_Editor.h"
#include "moc_MOS_DynaObject_Editor.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject_Editor.inl"
#endif

#include "MOS_ASN_Messages.h"
#include "MOS_DynaObject.h"
#include "MOS_MainWindow.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <qspinbox.h>

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Editor constructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MOS_DynaObject_Editor::MOS_DynaObject_Editor( QWidget* pParent )
    : QFrame  ( pParent )
    , pObject_( 0 )
{
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );


    QGridLayout* pFirstLayout = new QGridLayout( pMainLayout );
    pFirstLayout->setMargin( 20 );
    pFirstLayout->setAlignment( Qt::AlignTop );

    int nCurRow = 0;

    // Construction pourcentage 
    pFirstLayout->addWidget( new QLabel( "Construction %", this ) , nCurRow, 0 );
    pPrctConstructionSpinBox_ = new QSpinBox( 0, 100, 1, this );
    pFirstLayout->addWidget( pPrctConstructionSpinBox_, nCurRow++, 1 );

    // Valorization pourcentage 
    pFirstLayout->addWidget( new QLabel( "Valorization %", this ) , nCurRow, 0 );
    pPrctValorizationSpinBox_ = new QSpinBox( 0, 100, 1, this );
    pFirstLayout->addWidget( pPrctValorizationSpinBox_, nCurRow++, 1 );

    // Bypass creation pourcentage 
    pFirstLayout->addWidget( new QLabel( "Bypass creation %", this ) , nCurRow, 0 );
    pPrctBypassCreationSpinBox_ = new QSpinBox( 0, 100, 1, this );
    pFirstLayout->addWidget( pPrctBypassCreationSpinBox_, nCurRow++, 1 );

    // Prepared
    pFirstLayout->addWidget( new QLabel( "Prepared", this ) , nCurRow, 0 );
    pPreparedCheckBox_ = new QCheckBox( this );
    pFirstLayout->addWidget( pPreparedCheckBox_, nCurRow++, 1 );


    //--------------------
    // OK / Cancel

    // Second layout
    QHBoxLayout* pSecondLayout = new QHBoxLayout( pMainLayout );
    
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pSecondLayout->addWidget( pCancelButton );
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pSecondLayout->addWidget( pOKButton );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Editor destructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MOS_DynaObject_Editor::~MOS_DynaObject_Editor()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_Editor::SlotValidate
// Created: NLD 2003-08-05
//-----------------------------------------------------------------------------
void MOS_DynaObject_Editor::SlotValidate()
{
    assert( pObject_ );

    ASN1T_MagicActionUpdateObject asnAction;

    asnAction.m.pourcentage_constructionPresent           = 1;
    asnAction.m.pourcentage_valorisationPresent           = 1;
    asnAction.m.pourcentage_creation_contournementPresent = 1;
    asnAction.m.en_preparationPresent                     = 1;

    asnAction.pourcentage_construction           = pPrctConstructionSpinBox_  ->value();
    asnAction.pourcentage_valorisation           = pPrctValorizationSpinBox_  ->value();
    asnAction.pourcentage_creation_contournement = pPrctBypassCreationSpinBox_->value();
    asnAction.en_preparation                     = pPreparedCheckBox_         ->isChecked();

    MOS_ASN_MsgObjectMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid_objet                = pObject_->GetID();
    asnMsg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_update_object;
    asnMsg.GetAsnMsg().action.u.update_object   = &asnAction;

    asnMsg.Send( 912 );
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_Editor::SlotCancel
// Created: NLD 2003-08-05
//-----------------------------------------------------------------------------
void MOS_DynaObject_Editor::SlotCancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Editor::SetObject
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObject_Editor::SetObject( const MOS_DynaObject* pObject )
{
    pObject_ = pObject;
    if( !pObject_ )
        return;

    pPrctConstructionSpinBox_  ->setValue( pObject_->GetConstructionPercentage      () );
    pPrctValorizationSpinBox_  ->setValue( pObject_->GetValorizationPercentage      () );
    pPrctBypassCreationSpinBox_->setValue( pObject_->GetBypassConstructionPercentage() );
    pPreparedCheckBox_         ->setChecked( pObject_->IsPrepared() );
}
