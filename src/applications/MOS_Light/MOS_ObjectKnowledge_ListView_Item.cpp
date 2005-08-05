//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge_ListView_Item.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:02 $
// $Revision: 24 $
// $Workfile: MOS_ObjectKnowledge_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_ObjectKnowledge_ListView_Item.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ObjectKnowledge_ListView_Item.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"

#include "MOS_ObjectKnowledge.h"
#include "MOS_DynaObject.h"
#include "MOS_Agent.h"

//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView_Item constructor
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
MOS_ObjectKnowledge_ListView_Item::MOS_ObjectKnowledge_ListView_Item( QListView* pParent, MOS_ObjectKnowledge& represented_ )
    : QListViewItem                                  ( pParent, "Object" )
    , represented_                                   ( represented_ )
    , pListViewItemSiteFranchissementLargeur_        ( 0 )
    , pListViewItemSiteFranchissementProfondeur_     ( 0 )
    , pListViewItemSiteFranchissementVitesseCourant_ ( 0 )
    , pListViewItemSiteFranchissementBergesAAmenager_( 0 )
    , pListViewItemNuageNBCAgentNbcID_               ( 0 )
    , pListViewItemRotaDanger_                       ( 0 )
    , pListViewItemRotaNbcAgents_                    ( 0 )
    , pListViewItemItiLogEquipped_                   ( 0 )
    , pListViewItemItiLogFlow_                       ( 0 )
    , pListViewItemItiLogWidth_                      ( 0 )
    , pListViewItemItiLogLength_                     ( 0 )
    , pListViewItemItiLogMaxWeight_                  ( 0 )
{
    // Nom de l'objet
    std::string strLabel( "Objet " );
    strLabel += itostring( represented_.GetID() );
    setText( 0, strLabel.c_str() );

    // Type de l'objet
    std::string strTmp( "Type ");
    strTmp += ENT_Tr::ConvertFromObjectType( (E_ObjectType)represented_.nObjectTypeID_ );
    QListViewItem* pType = new QListViewItem( this, strTmp.c_str() );

    pListViewItemRealObject_                = new QListViewItem( this, pType, "ID objet réel : -" );
    pListViewItemRelevance_                 = new QListViewItem( this, pListViewItemRealObject_, "Pertinence : -" );
    pListViewItemPourcentageConstruction_   = new QListViewItem( this, pListViewItemRelevance_, "Construit à : -" );
    pListViewItemPourcentageValorisation_   = new QListViewItem( this, pListViewItemPourcentageConstruction_, "Valorisé à : -" );
    pListViewItemPourcentageContournement_  = new QListViewItem( this, pListViewItemPourcentageValorisation_, "Contourné à : -" );
    pListViewItemEnPreparation_             = new QListViewItem( this, pListViewItemPourcentageContournement_, " - " );
    pListViewItemIsPerceived_               = new QListViewItem( this, pListViewItemEnPreparation_, " - ");
    pListViewItemPerceivedBy_               = new QListViewItem( this, pListViewItemIsPerceived_, " - " );
   
    represented_.SetListViewItem( this );
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView_Item destructor
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
MOS_ObjectKnowledge_ListView_Item::~MOS_ObjectKnowledge_ListView_Item()
{
    represented_.SetListViewItem( 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView_Item::rtti
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
int MOS_ObjectKnowledge_ListView_Item::rtti() const
{
    return e_rtti;
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_ObjectKnowledge_ListView_Item::Update()
{
    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_RealObject ) )
        pListViewItemRealObject_->setText( 0, MT_FormatString( "ID objet réel : %d", represented_.pRealObject_ ? represented_.pRealObject_->GetID() : 0 ).c_str() );

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_Relevance ) )
        pListViewItemRelevance_->setText( 0, MT_FormatString( "Pertinence : %d%%", represented_.nRelevance_ ).c_str() );

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_PourcentageConstruction ) )
        pListViewItemPourcentageConstruction_->setText( 0, MT_FormatString( "Construit à : %d%%", represented_.nPourcentageConstruction_ ).c_str() );

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_PourcentageValorisation ) )
        pListViewItemPourcentageValorisation_->setText( 0, MT_FormatString( "Valorisé à : %d%%", represented_.nPourcentageValorisation_ ).c_str() );

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_PourcentageContournement ) )
        pListViewItemPourcentageContournement_->setText( 0, MT_FormatString( "Contourné à : %d%%", represented_.nPourcentageContournement_ ).c_str() );

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_EnPreparation ) )
        pListViewItemEnPreparation_->setText( 0, represented_.bEnPreparation_ ? "Préparé" : "Non préparé" );

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_IsPerceived ) )
        pListViewItemIsPerceived_->setText( 0, represented_.bIsPerceived_ ? "Est perçu" : "Non perçu" );

    if( represented_.bAttrSiteFranchissementPresent_ )
    {
        if( !pListViewItemSiteFranchissementLargeur_ )
        {
            pListViewItemSiteFranchissementLargeur_         = new QListViewItem( this, pListViewItemPerceivedBy_, "" );
            pListViewItemSiteFranchissementProfondeur_      = new QListViewItem( this, pListViewItemSiteFranchissementLargeur_, "" );
            pListViewItemSiteFranchissementVitesseCourant_  = new QListViewItem( this, pListViewItemSiteFranchissementProfondeur_, "" );
            pListViewItemSiteFranchissementBergesAAmenager_ = new QListViewItem( this, pListViewItemSiteFranchissementVitesseCourant_, "" );
        }
        pListViewItemSiteFranchissementLargeur_        ->setText( 0, MT_FormatString( "Largeur : %dm", represented_.nSiteFranchissementLargeur_ ).c_str() );
        pListViewItemSiteFranchissementProfondeur_     ->setText( 0, MT_FormatString( "Profondeur : %dm", represented_.nSiteFranchissementProfondeur_ ).c_str() );
        pListViewItemSiteFranchissementVitesseCourant_ ->setText( 0, MT_FormatString( "Vitesse courant : %dkm/h", represented_.nSiteFranchissementVitesseCourant_ ).c_str() );
        pListViewItemSiteFranchissementBergesAAmenager_->setText( 0, represented_.nSiteFranchissementBergesAAmenager_ ? "Berges à aménager" : "Berges aménagées" );
    }

    if( represented_.bAttrItiLogPresent_ )
    {
        if( !pListViewItemItiLogEquipped_ )
            pListViewItemItiLogEquipped_ = new QListViewItem( this, pListViewItemPerceivedBy_, "" );
        if( !pListViewItemItiLogFlow_ )
            pListViewItemItiLogFlow_     = new QListViewItem( this, pListViewItemItiLogFlow_, "" );
        if( !pListViewItemItiLogWidth_ )
            pListViewItemItiLogWidth_    = new QListViewItem( this, pListViewItemItiLogWidth_, "" );
        if( !pListViewItemItiLogLength_ )
            pListViewItemItiLogLength_   = new QListViewItem( this, pListViewItemItiLogWidth_, "" );
        if( !pListViewItemItiLogMaxWeight_ )
            pListViewItemItiLogMaxWeight_ = new QListViewItem( this, pListViewItemItiLogLength_, "" );

        if( represented_.bItiLogEquipped_ )
            pListViewItemItiLogEquipped_->setText( 0, "Iti log equipé" );
        else
            pListViewItemItiLogEquipped_->setText( 0, "Iti log non equipé" );

        pListViewItemItiLogFlow_     ->setText( 0, MT_FormatString( "Débit    : %d", represented_.nItiLogistiqueFlow_   ).c_str() );
        pListViewItemItiLogWidth_    ->setText( 0, MT_FormatString( "Largeur  : %d", represented_.nItiLogistiqueWidth_  ).c_str() );
        pListViewItemItiLogLength_   ->setText( 0, MT_FormatString( "Longueur : %d", represented_.nItiLogistiqueLength_ ).c_str() );
        pListViewItemItiLogMaxWeight_->setText( 0, MT_FormatString( "Poids max supporté : %d", represented_.nItiLogistiqueMaxWeight_ ).c_str() );
    }

    if( represented_.bAttrNuageNBCPresent_ )
    {
        if( !pListViewItemNuageNBCAgentNbcID_ )
            pListViewItemNuageNBCAgentNbcID_ = new QListViewItem( this, pListViewItemPerceivedBy_, "" );
        pListViewItemNuageNBCAgentNbcID_->setText( 0, MT_FormatString( "Agent NBC : '%s'", MOS_App::GetApp().GetAgentNBCName( represented_.nNuageNBCAgentNbcID_ ).c_str() ).c_str() );
    }

    if( represented_.bAttrRotaPresent_ )
    {
        if( !pListViewItemRotaDanger_ )
            pListViewItemRotaDanger_ = new QListViewItem( this, pListViewItemPerceivedBy_, "" );
        if( !pListViewItemRotaNbcAgents_ )
            pListViewItemRotaNbcAgents_ = new QListViewItem( this, pListViewItemRotaDanger_, "" );

        std::string strTmp = "Danger ";
        strTmp += itostring( represented_.nRotaDanger_ );
        pListViewItemRotaDanger_->setText( 0, strTmp.c_str() );

        strTmp = "Agents NBC ";
        const std::vector< uint >& nbcAgents = represented_.rotaNbcAgents_;
        for( std::vector< uint >::const_iterator it = nbcAgents.begin(); it != nbcAgents.end(); ++it )
        {
            uint nID = *it;
            strTmp += "'";
            strTmp += MOS_App::GetApp().GetAgentNBCName( nID );
            strTmp += "' ";
        }
        pListViewItemRotaNbcAgents_->setText( 0, strTmp.c_str() );
    }

    if( represented_.IsValid( MOS_ObjectKnowledge::eUpdated_AutomatePerception ) )
    {
        std::string strTmp = "Perçu par automates ";
        for( MOS_ObjectKnowledge::CIT_AutomatePerceptionSet it = represented_.automatePerceptionSet_.begin(); it != represented_.automatePerceptionSet_.end(); ++it )
            strTmp += MT_FormatString( "%d ", (*it)->GetAgentID() );

        pListViewItemPerceivedBy_->setText( 0, strTmp.c_str() );
    }

}




