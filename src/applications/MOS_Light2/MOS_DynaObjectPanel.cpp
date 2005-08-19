// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-11 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:12 $
// $Revision: 7 $
// $Workfile: MOS_DynaObjectPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_DynaObjectPanel.h"
#include "moc_MOS_DynaObjectPanel.cpp"

#include "MOS_DynaObject.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Tools.h"
#include "MOS_ASN_Messages.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObjectPanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
MOS_DynaObjectPanel::MOS_DynaObjectPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC( pParent )
{
    delete layout(); // $$$$ AGE 2005-04-05: mouarf
    QGridLayout* pLayout = new QGridLayout( this, 1, 2, 5 );
    pLayout->setAutoAdd( true );

    new QLabel( tr( "Id:" ), this );
    pIdLabel_ = new QLabel( this );

    new QLabel( tr( "Type:" ), this );
    pObjectTypeLabel_ = new QLabel( this );

    new QLabel( tr( "Position:" ), this );
    pPositionLabel_ = new QLabel( this );

    new QLabel( tr( "Construction:" ), this );
    pPercentBuiltEdit_ = new QSpinBox( 0, 100, 1, this );
    pPercentBuiltEdit_->setSuffix( "%" );

    new QLabel( tr( "Valeur:" ), this );
    pPercentValueEdit_ = new QSpinBox( 0, 100, 1, this );
    pPercentValueEdit_->setSuffix( "%" );

    new QLabel( tr( "Contournement:" ), this );
    pPercentAroundEdit_ = new QSpinBox( 0, 100, 1, this );
    pPercentAroundEdit_->setSuffix( "%" );

    new QLabel( tr( "En préparation:" ), this );
    pIsUnderPrepCheckBox_ = new QCheckBox( this );

    pCrossingWidthTitle_ = new QLabel( tr( "Largeur:" ), this );
    pCrossingWidthLabel_ = new QLabel( this );
    pCrossingWidthTitle_->hide();
    pCrossingWidthLabel_->hide();

    pCrossingDepthTitle_ = new QLabel( tr( "Profondeur:" ), this );
    pCrossingDepthLabel_ = new QLabel( this );
    pCrossingDepthTitle_->hide();
    pCrossingDepthLabel_->hide();

    pCrossingRiverSpeedTitle_ = new QLabel( tr( "Vitesse courant:" ), this );
    pCrossingRiverSpeedLabel_ = new QLabel( this );
    pCrossingRiverSpeedTitle_->hide();
    pCrossingRiverSpeedLabel_->hide();

    pCrossingBanksNeedWorkTitle_ = new QLabel( tr( "Berges à aménager:" ), this );
    pCrossingBanksNeedWorkLabel_ = new QLabel( this );
    pCrossingBanksNeedWorkTitle_->hide();
    pCrossingBanksNeedWorkLabel_->hide();
    
    pNBCAgentTitle_ = new QLabel( tr( "Nuage Agent NBC:" ), this );
    pNBCAgentLabel_ = new QLabel( this );
    pNBCAgentTitle_->hide();
    pNBCAgentLabel_->hide();

	pTC2Title_ = new QLabel( tr( "TC2:" ), this );
	pTC2Label_ = new QLabel( this );
	pTC2Title_->hide();
	pTC2Label_->hide();

    new QWidget( this );
    new QWidget( this );

    pLayout->setRowStretch( 13, 100 );
    pLayout->setRowSpacing( 13, 26 );

    pApplyButton_ = new QPushButton( tr( "Appliquer" ), this );
    pCancelButton_ = new QPushButton( tr( "Annuler" ), this );

    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel destructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
MOS_DynaObjectPanel::~MOS_DynaObjectPanel()
{
    
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::OnDynaObjectUpdated
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::OnDynaObjectUpdated( MOS_DynaObject& object )
{
    MOS_DynaObject* pObject = selectedItem_.pDynaObject_;
    if( &object != pObject )
        return;

    pIdLabel_               ->setText( "-" );
    pObjectTypeLabel_       ->setText( "-" );
    pPositionLabel_         ->setText( "-" );
    pPercentBuiltEdit_      ->setValue( 0 );
    pPercentValueEdit_      ->setValue( 0 );
    pPercentAroundEdit_     ->setValue( 0 );
    pIsUnderPrepCheckBox_   ->setChecked( false );

    pCrossingWidthTitle_->hide();
    pCrossingWidthLabel_->hide();

    pCrossingDepthTitle_->hide();
    pCrossingDepthLabel_->hide();

    pCrossingRiverSpeedTitle_->hide();
    pCrossingRiverSpeedLabel_->hide();

    pCrossingBanksNeedWorkTitle_->hide();
    pCrossingBanksNeedWorkLabel_->hide();

    pNBCAgentTitle_->hide();
    pNBCAgentLabel_->hide();

	pTC2Title_->hide();
	pTC2Label_->hide();

    pApplyButton_->setEnabled( pObject != 0 );
    pCancelButton_->setEnabled( pObject != 0 );

    if( ! ShouldDisplay( object ) )
        return;

    pIdLabel_->setText( QString::number( pObject->GetID() ) );

    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pObject->GetCenter(), strPos );
    pPositionLabel_->setText( strPos.c_str() );

    pObjectTypeLabel_->setText( ENT_Tr::ConvertFromObjectType( (E_ObjectType)pObject->GetType() ).c_str() );

    pPercentBuiltEdit_->setValue( pObject->GetConstructionPercentage() );
    pPercentValueEdit_->setValue( pObject->GetValorizationPercentage() );
    pPercentAroundEdit_->setValue( pObject->GetBypassConstructionPercentage() );
    pIsUnderPrepCheckBox_->setChecked( pObject->IsPrepared() );

    if( pObject->AreAttrSiteFranchissementPresent() )
    {
        pCrossingWidthTitle_->show();
        pCrossingWidthLabel_->show();
        pCrossingWidthLabel_->setText( QString::number( pObject->GetSiteFranchissementLargeur() ) );

        pCrossingDepthTitle_->show();
        pCrossingDepthLabel_->show();
        pCrossingDepthLabel_->setText( QString::number( pObject->GetSiteFranchissementProfondeur() ) );

        pCrossingRiverSpeedTitle_->show();
        pCrossingRiverSpeedLabel_->show();
        pCrossingRiverSpeedLabel_->setText( QString::number( pObject->GetSiteFranchissementVitesseCourant() ) );

        pCrossingBanksNeedWorkTitle_->show();
        pCrossingBanksNeedWorkLabel_->show();
        pCrossingBanksNeedWorkLabel_->setText( pObject->GetSiteFranchissementBergesAAmenager() ? tr( "Oui" ) : tr( "Non" ) );
    }

    if( pObject->AreAttrNuageNBCPresent() )
    {
        pNBCAgentTitle_->show();
        pNBCAgentLabel_->show();

        pNBCAgentLabel_->setText( (MOS_App::GetApp().GetNBCName( pObject->GetNuageNBCAgentNbcId() )).c_str() );
    }

	if( pObject->AreAttrTC2Present() )
    {
		pTC2Title_->show();
		pTC2Label_->show();

		pTC2Label_->setText( QString::number(pObject->GetTC2ID()) );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::OnApply
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::OnApply()
{
    assert( selectedItem_.pDynaObject_ != 0 );

    ASN1T_MagicActionUpdateObject asnAction;

    asnAction.m.pourcentage_constructionPresent           = 1;
    asnAction.m.pourcentage_valorisationPresent           = 1;
    asnAction.m.pourcentage_creation_contournementPresent = 1;
    asnAction.m.en_preparationPresent                     = 1;

    asnAction.pourcentage_construction           = pPercentBuiltEdit_   ->value();
    asnAction.pourcentage_valorisation           = pPercentValueEdit_   ->value();
    asnAction.pourcentage_creation_contournement = pPercentAroundEdit_  ->value();
    asnAction.en_preparation                     = pIsUnderPrepCheckBox_->isChecked();

    MOS_ASN_MsgObjectMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid_objet                = selectedItem_.pDynaObject_->GetID();
    asnMsg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_update_object;
    asnMsg.GetAsnMsg().action.u.update_object   = &asnAction;

    asnMsg.Send( 912 );

    std::stringstream strMsgTitle;
    strMsgTitle << "Demande modification objet " << selectedItem_.pDynaObject_->GetID();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::OnCancel
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::OnCancel()
{
    OnDynaObjectUpdated( *selectedItem_.pDynaObject_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::OnUpdate()
{
    OnDynaObjectUpdated( *selectedItem_.pDynaObject_ );
}
