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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:12 $
// $Revision: 7 $
// $Workfile: MOS_ObjectPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ObjectPanel.h"
#include "moc_MOS_ObjectPanel.cpp"

#include "MOS_Object_ABC.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Tools.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Object_Generic.h"
#include "MOS_Object_SiteFranchissement.h"
#include "MOS_Object_NBC.h"
#include "MOS_Object_Camp.h"
#include "MOS_Object_ROTA.h"
#include "MOS_Object_ItineraireLogistique.h"
#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
MOS_ObjectPanel::MOS_ObjectPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC ( pParent )
{
    InitializeGeneric             ( this );
    InitializeSiteFranchissement  ( this );
    InitializeCamp                ( this );
    InitializeNBC                 ( this );
    InitializeROTA                ( this );
    InitializeItineraireLogistique( this );

    // $$$$ SBO 2005-09-23: QSpacerItem is insert as first element in layout...
    //layout()->addItem( new QSpacerItem( 100, 50, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
    QWidget* pSpacer = new QWidget( this );
    pSpacer->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );

    QHBox* pHBox = new QHBox( this );
    pApplyButton_  = new QPushButton( tr( "Appliquer" ), pHBox );
    pCancelButton_ = new QPushButton( tr( "Annuler" )  , pHBox );

    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel destructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
MOS_ObjectPanel::~MOS_ObjectPanel()
{
    
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::OnObjectUpdated
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::OnObjectUpdated( MOS_Object_ABC& object )
{
    OnClearSelection();

    if( ! ShouldDisplay( object ) )
        return;

    pApplyButton_ ->setEnabled( true );
    pCancelButton_->setEnabled( true );

    UpdateGeneric( object );

    switch( object.GetType() )
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
// Name: MOS_ObjectPanel::OnApply
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::OnApply()
{
    assert( selectedItem_.pObject_ != 0 );

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
    asnMsg.GetAsnMsg().oid_objet                = selectedItem_.pObject_->GetID();
    asnMsg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_update_object;
    asnMsg.GetAsnMsg().action.u.update_object   = &asnAction;

    asnMsg.Send( 912 );

    std::stringstream strMsgTitle;
    strMsgTitle << "Demande modification objet " << selectedItem_.pObject_->GetID();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::OnCancel
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::OnCancel()
{
    if( selectedItem_.pObject_ != 0 )
        OnObjectUpdated( *selectedItem_.pObject_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::OnUpdate()
{
    if( selectedItem_.pObject_ != 0 )
        OnObjectUpdated( *selectedItem_.pObject_ );
    else
        OnClearSelection();
    
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::OnClearSelection()
{
    pIdLabel_                   ->setText( "-" );
    pNameLabel_                 ->setText( "-" );
    pObjectTypeLabel_           ->setText( "-" );
    pPositionLabel_             ->setText( "-" );
    pPercentBuiltEdit_          ->setValue( 0 );
    pPercentValueEdit_          ->setValue( 0 );
    pPercentAroundEdit_         ->setValue( 0 );
    pIsUnderPrepCheckBox_       ->setChecked( false );
    pDotationConstructionLabel_ ->setText( "-" );
    pDotationValorisationLabel_ ->setText( "-" );

    pCrossingGroup_->hide();
    pNBCGroup_     ->hide();
    pCampGroup_    ->hide();
    pROTAGroup_    ->hide();
    pItLogGroup_   ->hide();

    pApplyButton_ ->setEnabled( false );
    pCancelButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::InitializeGeneric
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::InitializeGeneric( QWidget* pParent )
{
    pGenericGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Informations" ), pParent );

    AddRow( tr( "Id:" )                   , pIdLabel_                  , pGenericGroup_ );
    AddRow( tr( "Nom:" )                  , pNameLabel_                , pGenericGroup_ );
    AddRow( tr( "Type:" )                 , pObjectTypeLabel_          , pGenericGroup_ );
    AddRow( tr( "Position:" )             , pPositionLabel_            , pGenericGroup_ );
    AddRow( tr( "Construction:" )         , pPercentBuiltEdit_         , pGenericGroup_ );
    AddRow( tr( "Valeur:" )               , pPercentValueEdit_         , pGenericGroup_ );
    AddRow( tr( "Contournement:" )        , pPercentAroundEdit_        , pGenericGroup_ );
    AddRow( tr( "En préparation:" )       , pIsUnderPrepCheckBox_      , pGenericGroup_ );
    AddRow( tr( "Dotation construction:" ), pDotationConstructionLabel_, pGenericGroup_ );
    AddRow( tr( "Dotation valorisation:" ), pDotationValorisationLabel_, pGenericGroup_ );
    pGenericGroup_->show();
}
// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::InitializeSiteFranchissement
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::InitializeSiteFranchissement( QWidget* pParent )
{
    pCrossingGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres spécifiques" ), pParent );

    AddRow( tr( "Largeur:" ), pCrossingWidthLabel_, pCrossingGroup_ );
    AddRow( tr( "Profondeur:" ), pCrossingDepthLabel_, pCrossingGroup_ );
    AddRow( tr( "Vitesse courant:" ), pCrossingRiverSpeedLabel_, pCrossingGroup_ );
    AddRow( tr( "Berges à aménager:" ), pCrossingBanksNeedWorkLabel_, pCrossingGroup_ );
    pCrossingGroup_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::InitializeCamp
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::InitializeCamp( QWidget* pParent )
{
    pCampGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres spécifiques" ), pParent );

    AddRow( tr( "TC2:" ), pTC2Label_, pCampGroup_ );
    pCampGroup_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::InitializeNBC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::InitializeNBC( QWidget* pParent )
{
    pNBCGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres spécifiques" ), pParent );

    AddRow( tr( "Nuage/Zone Agent NBC:" ), pNBCAgentLabel_, pNBCGroup_ );
    pNBCGroup_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::InitializeROTA
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::InitializeROTA( QWidget* pParent )
{
    pROTAGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres spécifiques" ), pParent );

    AddRow( tr( "Danger:" ), pROTADangerLabel_, pROTAGroup_ );
    AddRow( tr( "Agents NBC:" ), pROTAAgentsNbcLabel_, pROTAGroup_ );
    pROTAGroup_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::InitializeItineraireLogistique
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::InitializeItineraireLogistique( QWidget* pParent )
{
    pItLogGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Paramètres spécifiques" ), pParent );

    AddRow( tr( "Equipé:" ), pItLogEquippedLabel_, pItLogGroup_ );
    AddRow( tr( "Débit:" ), pItLogFlowLabel_, pItLogGroup_ );
    AddRow( tr( "Largeur:" ), pItLogWidthLabel_, pItLogGroup_ );
    AddRow( tr( "Longueur:" ), pItLogLengthLabel_, pItLogGroup_ );
    AddRow( tr( "Poids supporté:" ), pItLogMaxWeightLabel_, pItLogGroup_ );
    pItLogGroup_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateGeneric
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateGeneric( MOS_Object_ABC& object )
{
    pIdLabel_  ->setText( QString::number( object.GetID() ) );
    pNameLabel_->setText( object.GetName().c_str() );

    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( object.GetCenter(), strPos );
    pPositionLabel_->setText( strPos.c_str() );

    pObjectTypeLabel_->setText( ENT_Tr::ConvertFromObjectType( (E_ObjectType)object.GetType() ).c_str() );

    pPercentBuiltEdit_->setValue( object.GetConstructionPercentage() );
    pPercentValueEdit_->setValue( object.GetValorizationPercentage() );
    pPercentAroundEdit_->setValue( object.GetBypassConstructionPercentage() );
    pIsUnderPrepCheckBox_->setChecked( object.IsPrepared() );

    pDotationConstructionLabel_->setText( QString::number( object.GetNbrDotationConstruction() ) + " " + object.GetTypeDotationConstruction().c_str() );
    pDotationValorisationLabel_->setText( QString::number( object.GetNbrDotationValorization() ) + " " + object.GetTypeDotationValorization().c_str() );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateSiteFranchissement
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateSiteFranchissement( MOS_Object_ABC& object )
{
    MOS_Object_SiteFranchissement& obj = static_cast< MOS_Object_SiteFranchissement& >( object );

    pCrossingGroup_->show();

    pCrossingWidthLabel_        ->setText( QString::number( obj.GetWidth() ) + " m" );
    pCrossingDepthLabel_        ->setText( QString::number( obj.GetDepth() ) + " m" );
    pCrossingRiverSpeedLabel_   ->setText( QString::number( obj.GetSpeed() ) + " m/s" );
    pCrossingBanksNeedWorkLabel_->setText( obj.MustConstruct() ? tr( "Oui" ) : tr( "Non" ) );
}
// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateCamp
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateCamp( MOS_Object_ABC& object )
{
    MOS_Object_Camp& obj = static_cast< MOS_Object_Camp& >( object );

    pCampGroup_->show();
	pTC2Label_->setText( QString::number( obj.GetTC2ID() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateNBC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateNBC( MOS_Object_ABC& object )
{
    MOS_Object_NBC& obj = static_cast< MOS_Object_NBC& >( object );

    pNBCGroup_->show();
    pNBCAgentLabel_->setText( (MOS_App::GetApp().GetNBCName( obj.GetAgentNbcId() )).c_str() );
}
// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateROTA
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateROTA( MOS_Object_ABC& object )
{
    MOS_Object_ROTA& obj = static_cast< MOS_Object_ROTA& >( object );

    pROTAGroup_->show();
    pROTADangerLabel_->setText( QString::number( obj.GetDanger() ) );
    const std::vector< uint >& agents = obj.GetNBCAgents();
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
// Name: MOS_ObjectPanel::UpdateItineraireLogistique
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateItineraireLogistique( MOS_Object_ABC& object )
{
    MOS_Object_ItineraireLogistique& obj = static_cast< MOS_Object_ItineraireLogistique& >( object );

    pItLogGroup_->show();
    pItLogEquippedLabel_ ->setText( obj.IsEquipped() ? tr( "oui" ) : tr( "non" ) );
    pItLogFlowLabel_     ->setText( QString::number( obj.GetFlow() ) + " veh/h" );
    pItLogWidthLabel_    ->setText( QString::number( obj.GetWidth() ) + " m" );
    pItLogLengthLabel_   ->setText( QString::number( obj.GetLength() ) + " m" );
    pItLogMaxWeightLabel_->setText( QString::number( obj.GetMaxWeight() ) + " t" );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::AddRow
// Created: SBO 2005-09-23
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::AddRow( const QString& strLabel, QLabel*& pValueLabel, QWidget* pParent )
{
    new QLabel( strLabel, pParent );
    pValueLabel = new QLabel( pParent );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::AddRow
// Created: SBO 2005-09-23
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::AddRow( const QString& strLabel, QSpinBox*& pValueSpinBox, QWidget* pParent )
{
    new QLabel( strLabel, pParent );
    pValueSpinBox = new QSpinBox( 0, 100, 1, pParent );
    pValueSpinBox->setSuffix( "%" );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::AddRow
// Created: SBO 2005-09-23
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::AddRow( const QString& strLabel, QCheckBox*& pValueCheckBox, QWidget* pParent )
{
    new QLabel( strLabel, pParent );
    pValueCheckBox = new QCheckBox( pParent );
}
