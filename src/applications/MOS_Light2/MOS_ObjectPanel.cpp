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
#include "MOS_Display.h"
#include "MOS_DisplayGroup.h"
#include "MOS_DisplayItem.h"
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
    display_ = new MOS_Display( this );
    display_->AddGroup( "Informations" )
                .AddItem( "Id:" )
                .AddItem( "Nom:" )
                .AddItem( "Type:" )
                .AddItem( "Position:" );

    MOS_DisplayGroup* group = &display_->Group( "Informations" );
    new QLabel( "Construction:", group );
    pPercentBuiltEdit_  = new QSpinBox( 0, 100, 1, group ); pPercentBuiltEdit_->setSuffix( "%" );
    new QLabel( "Valeur:", group );
    pPercentValueEdit_  = new QSpinBox( 0, 100, 1, group ); pPercentValueEdit_->setSuffix( "%" );
    new QLabel( "Contournement:", group );
    pPercentAroundEdit_ = new QSpinBox( 0, 100, 1, group ); pPercentAroundEdit_->setSuffix( "%" );
    new QLabel( "En préparation:", group );
    pIsUnderPrepCheckBox_ = new QCheckBox( group );

    display_->Group( "Informations" )
                .AddItem( "Dotation construction:" )
                .AddItem( "Dotation valorisation:" );

    display_->AddGroup( "Site de franchissement" )
                .AddItem( "Largeur:" )
                .AddItem( "Profondeur:" )
                .AddItem( "Vitesse courant:" )
                .AddItem( "Berges à aménager:" );

    display_->AddGroup( "Camp" )
                .AddItem( "TC2:" );

    display_->AddGroup( "Nuage/Zone NBC" )
                .AddItem( "Agent NBC:" );

    display_->AddGroup( "ROTA" )
                .AddItem( "Danger:" )
                .AddItem( "Agents NBC:" );

    display_->AddGroup( "Itinéraire Logistique" )
                .AddItem( "Equipé:" )
                .AddItem( "Débit:" )
                .AddItem( "Largeur:" )
                .AddItem( "Longueur:" )
                .AddItem( "Poids supporté:" );

    // $$$$ SBO 2005-09-23: QSpacerItem is inserted as first element in layout...
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
    delete display_;
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
    OnUpdate();
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
    display_->Clear();
    display_->Group( "Site de franchissement" ).hide();
    display_->Group( "Camp" ).hide();
    display_->Group( "Nuage/Zone NBC" ).hide();
    display_->Group( "ROTA" ).hide();
    display_->Group( "Itinéraire Logistique" ).hide();

    pApplyButton_ ->setEnabled( false );
    pCancelButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateGeneric
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateGeneric( MOS_Object_ABC& object )
{
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( object.GetCenter(), strPos );

    display_->Group( "Informations" )
                .Display( "Id:", MOS_Display::Id( object.nID_ ) )
                .Display( "Nom:", object.strName_ )
                .Display( "Type:", ENT_Tr::ConvertFromObjectType( (E_ObjectType)object.nType_ ) )
                .Display( "Position:", strPos )
                .Display( "Dotation construction:", QString::number( object.GetNbrDotationConstruction() ) + " " + object.GetTypeDotationConstruction().c_str() )
                .Display( "Dotation valorisation:", QString::number( object.GetNbrDotationValorization() ) + " " + object.GetTypeDotationValorization().c_str() );

    pPercentBuiltEdit_->setValue( object.rConstructionPercentage_ );
    pPercentValueEdit_->setValue( object.rValorizationPercentage_ );
    pPercentAroundEdit_->setValue( object.rBypassConstructionPercentage_ );
    pIsUnderPrepCheckBox_->setChecked( object.bPrepared_ );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateSiteFranchissement
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateSiteFranchissement( MOS_Object_ABC& object )
{
    MOS_Object_SiteFranchissement& obj = static_cast< MOS_Object_SiteFranchissement& >( object );
    display_->Group( "Site de franchissement" )
                .Display( "Largeur:", QString::number( obj.GetWidth() ) + " m" )
                .Display( "Profondeur:", QString::number( obj.GetDepth() ) + " m" )
                .Display( "Vitesse courant:", QString::number( obj.GetSpeed() ) + " m/s" )
                .Display( "Berges à aménager:", MOS_Display::YesNo( obj.MustConstruct() ) );
}
// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateCamp
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateCamp( MOS_Object_ABC& object )
{
    MOS_Object_Camp& obj = static_cast< MOS_Object_Camp& >( object );
    display_->Group( "Camp" ).Display( "TC2:", MOS_Display::Id( obj.GetTC2ID() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateNBC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateNBC( MOS_Object_ABC& object )
{
    MOS_Object_NBC& obj = static_cast< MOS_Object_NBC& >( object );
    display_->Group( "Nuage/Zone NBC" ).Display( "Agent NBC", MOS_App::GetApp().GetNBCName( obj.GetAgentNbcId() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateROTA
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateROTA( MOS_Object_ABC& object )
{
    MOS_Object_ROTA& obj = static_cast< MOS_Object_ROTA& >( object );
        const std::vector< uint >& agents = obj.GetNBCAgents();
    std::stringstream ss;
    for( uint i = 0; i < agents.size(); ++i ) {
        if( i > 0 )
            ss << ", ";
        ss << MOS_App::GetApp().GetNBCName( agents[ i ] );
    }

    display_->Group( "ROTA" )
                .Display( "Danger:", obj.GetDanger() )
                .Display( "Agents NBC:", ss.str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectPanel::UpdateItineraireLogistique
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_ObjectPanel::UpdateItineraireLogistique( MOS_Object_ABC& object )
{
    MOS_Object_ItineraireLogistique& obj = static_cast< MOS_Object_ItineraireLogistique& >( object );
    display_->Group( "Itinéraire Logistique" )
                .Display( "Equipé:", MOS_Display::YesNo( obj.IsEquipped() ) )
                .Display( "Débit:", QString::number( obj.GetFlow() ) + " veh/h" )
                .Display( "Largeur:", QString::number( obj.GetWidth() ) + " m" )
                .Display( "Longueur:", QString::number( obj.GetLength() ) + " m" )
                .Display( "Poids supporté:", QString::number( obj.GetMaxWeight() ) + " t" );
}
