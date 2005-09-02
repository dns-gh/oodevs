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

#include "MOS_DynaObject_ABC.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Tools.h"
#include "MOS_ASN_Messages.h"
#include "MOS_DynaObject_Generic.h"
#include "MOS_DynaObject_SiteFranchissement.h"
#include "MOS_DynaObject_NBC.h"
#include "MOS_DynaObject_Camp.h"
#include "MOS_DynaObject_ROTA.h"
#include "MOS_DynaObject_ItineraireLogistique.h"

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
    : MOS_InfoPanel_ABC ( pParent )
{
    delete layout(); // $$$$ AGE 2005-04-05: mouarf

    QGridLayout* pLayout = new QGridLayout( this, 1, 2, 5 );
    pLayout->setAutoAdd( true );

    InitializeGeneric             ();
    InitializeSiteFranchissement  ();
    InitializeCamp                ();
    InitializeNBC                 ();
    InitializeROTA                ();
    InitializeItineraireLogistique();
 
    new QWidget( this );
    new QWidget( this );

    pLayout->setRowStretch( 20, 100 );
    pLayout->setRowSpacing( 20, 26 );

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
void MOS_DynaObjectPanel::OnDynaObjectUpdated( MOS_DynaObject_ABC& object )
{
    MOS_DynaObject_ABC* pObject = selectedItem_.pDynaObject_;
    if( &object != pObject )
        return;

    pIdLabel_            ->setText( "-" );
    pObjectTypeLabel_    ->setText( "-" );
    pPositionLabel_      ->setText( "-" );
    pPercentBuiltEdit_   ->setValue( 0 );
    pPercentValueEdit_   ->setValue( 0 );
    pPercentAroundEdit_  ->setValue( 0 );
    pIsUnderPrepCheckBox_->setChecked( false );

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

    pROTADangerTitle_->hide();
    pROTADangerLabel_->hide();
    pROTAAgentsNbcTitle_->hide();
    pROTAAgentsNbcLabel_->hide();

    pItLogEquippedTitle_->hide();
    pItLogEquippedLabel_->hide();
    pItLogFlowTitle_->hide();
    pItLogFlowLabel_->hide();
    pItLogWidthTitle_->hide();
    pItLogWidthLabel_->hide();
    pItLogLengthTitle_->hide();
    pItLogLengthLabel_->hide();
    pItLogMaxWeightTitle_->hide();
    pItLogMaxWeightLabel_->hide();


    pApplyButton_->setEnabled( pObject != 0 );
    pCancelButton_->setEnabled( pObject != 0 );

    if( ! ShouldDisplay( object ) )
        return;

    UpdateGeneric( object );

    switch( pObject->GetType() )
    {
    case EnumObjectType::site_franchissement:
        UpdateSiteFranchissement( object );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        UpdateNBC( object );
        break;
    case EnumObjectType::rota:
        UpdateROTA( object );
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        UpdateCamp( object );
        break;
    case EnumObjectType::itineraire_logistique:
        UpdateItineraireLogistique( object );
        break;
    default:
        break;
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

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::InitializeGeneric
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::InitializeGeneric()
{
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
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::InitializeSiteFranchissement
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::InitializeSiteFranchissement()
{
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
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::InitializeCamp
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::InitializeCamp()
{
    pTC2Title_ = new QLabel( tr( "TC2:" ), this );
	pTC2Label_ = new QLabel( this );
	pTC2Title_->hide();
	pTC2Label_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::InitializeNBC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::InitializeNBC()
{
    pNBCAgentTitle_ = new QLabel( tr( "Nuage/Zone Agent NBC:" ), this );
    pNBCAgentLabel_ = new QLabel( this );
    pNBCAgentTitle_->hide();
    pNBCAgentLabel_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::InitializeROTA
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::InitializeROTA()
{
    pROTADangerTitle_ = new QLabel( tr( "Danger:" ), this );
    pROTADangerLabel_ = new QLabel( this );
    pROTADangerTitle_->hide();
    pROTADangerLabel_->hide();

    pROTAAgentsNbcTitle_ = new QLabel( tr( "Agents NBC:" ), this );
    pROTAAgentsNbcLabel_ = new QLabel( this );
    pROTAAgentsNbcTitle_->hide();
    pROTAAgentsNbcLabel_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::InitializeItineraireLogistique
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::InitializeItineraireLogistique()
{
    pItLogEquippedTitle_ = new QLabel( tr( "Equipé:" ), this );
    pItLogEquippedLabel_ = new QLabel( this );
    pItLogEquippedTitle_->hide();
    pItLogEquippedLabel_->hide();

    pItLogFlowTitle_ = new QLabel( tr( "Débit:" ), this );
    pItLogFlowLabel_ = new QLabel( this );
    pItLogFlowTitle_->hide();
    pItLogFlowLabel_->hide();

    pItLogWidthTitle_ = new QLabel( tr( "Largeur:" ), this );
    pItLogWidthLabel_ = new QLabel( this );
    pItLogWidthTitle_->hide();
    pItLogWidthLabel_->hide();

    pItLogLengthTitle_ = new QLabel( tr( "Longueur:" ), this );
    pItLogLengthLabel_ = new QLabel( this );
    pItLogLengthTitle_->hide();
    pItLogLengthLabel_->hide();

    pItLogMaxWeightTitle_ = new QLabel( tr( "Poids supporté:" ), this );
    pItLogMaxWeightLabel_ = new QLabel( this );
    pItLogMaxWeightTitle_->hide();
    pItLogMaxWeightLabel_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::UpdateGeneric
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::UpdateGeneric( MOS_DynaObject_ABC& object )
{
    pIdLabel_->setText( QString::number( object.GetID() ) );

    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( object.GetCenter(), strPos );
    pPositionLabel_->setText( strPos.c_str() );

    pObjectTypeLabel_->setText( ENT_Tr::ConvertFromObjectType( (E_ObjectType)object.GetType() ).c_str() );

    pPercentBuiltEdit_->setValue( object.GetConstructionPercentage() );
    pPercentValueEdit_->setValue( object.GetValorizationPercentage() );
    pPercentAroundEdit_->setValue( object.GetBypassConstructionPercentage() );
    pIsUnderPrepCheckBox_->setChecked( object.IsPrepared() );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::UpdateSiteFranchissement
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::UpdateSiteFranchissement( MOS_DynaObject_ABC& object )
{
    MOS_DynaObject_SiteFranchissement& obj = static_cast< MOS_DynaObject_SiteFranchissement& >( object );

    pCrossingWidthTitle_->show();
    pCrossingWidthLabel_->show();
    pCrossingWidthLabel_->setText( QString::number( obj.GetLargeur() ) + " m" );

    pCrossingDepthTitle_->show();
    pCrossingDepthLabel_->show();
    pCrossingDepthLabel_->setText( QString::number( obj.GetProfondeur() ) + " m" );

    pCrossingRiverSpeedTitle_->show();
    pCrossingRiverSpeedLabel_->show();
    pCrossingRiverSpeedLabel_->setText( QString::number( obj.GetVitesseCourant() ) + " m/s" );

    pCrossingBanksNeedWorkTitle_->show();
    pCrossingBanksNeedWorkLabel_->show();
    pCrossingBanksNeedWorkLabel_->setText( obj.AreBergesAAmenager() ? tr( "Oui" ) : tr( "Non" ) );
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::UpdateCamp
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::UpdateCamp( MOS_DynaObject_ABC& object )
{
    MOS_DynaObject_Camp& obj = static_cast< MOS_DynaObject_Camp& >( object );

	pTC2Title_->show();
	pTC2Label_->show();
	pTC2Label_->setText( QString::number( obj.GetTC2ID() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::UpdateNBC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::UpdateNBC( MOS_DynaObject_ABC& object )
{
    MOS_DynaObject_NBC& obj = static_cast< MOS_DynaObject_NBC& >( object );

    pNBCAgentTitle_->show();
    pNBCAgentLabel_->show();
    pNBCAgentLabel_->setText( (MOS_App::GetApp().GetNBCName( obj.GetAgentNbcId() )).c_str() );
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::UpdateROTA
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::UpdateROTA( MOS_DynaObject_ABC& object )
{
    MOS_DynaObject_ROTA& obj = static_cast< MOS_DynaObject_ROTA& >( object );

    pROTADangerTitle_->show();
    pROTADangerLabel_->show();
    pROTADangerLabel_->setText( QString::number( obj.GetDanger() ) );

    pROTAAgentsNbcTitle_->show();
    pROTAAgentsNbcLabel_->show();
    const std::vector< uint >& agents = obj.GetNbcAgents();
    std::stringstream ss;
    for( uint i = 0; i < agents.size(); ++i )
    {
        if( i > 0 )
            ss << ", ";
        ss << MOS_App::GetApp().GetNBCName( agents[ i ] );
    }

    pROTAAgentsNbcLabel_->setText( ss.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectPanel::UpdateItineraireLogistique
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObjectPanel::UpdateItineraireLogistique( MOS_DynaObject_ABC& object )
{
    MOS_DynaObject_ItineraireLogistique& obj = static_cast< MOS_DynaObject_ItineraireLogistique& >( object );

    pItLogEquippedTitle_->show();
    pItLogEquippedLabel_->show();
    pItLogEquippedLabel_->setText( obj.IsEquipped() ? tr( "oui" ) : tr( "non" ) );

    pItLogFlowTitle_->show();
    pItLogFlowLabel_->show();
    pItLogFlowLabel_->setText( QString::number( obj.GetFlow() ) + " veh/h" );

    pItLogWidthTitle_->show();
    pItLogWidthLabel_->show();
    pItLogWidthLabel_->setText( QString::number( obj.GetWidth() ) + " m" );

    pItLogLengthTitle_->show();
    pItLogLengthLabel_->show();
    pItLogLengthLabel_->setText( QString::number( obj.GetLength() ) + " m" );

    pItLogMaxWeightTitle_->show();
    pItLogMaxWeightLabel_->show();
    pItLogMaxWeightLabel_->setText( QString::number( obj.GetMaxWeight() ) + " t" );
}
