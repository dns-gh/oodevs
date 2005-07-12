//*****************************************************************************
//
// $Created: AGN 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_ListViewItem.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 14:55 $
// $Revision: 10 $
// $Workfile: MOS_DynaObject_ListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_DynaObject_ListViewItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_DynaObject_ListViewItem.inl"
#endif

#include "MOS_DynaObject.h"
#include "MOS_App.h"

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListViewItem constructor
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_DynaObject_ListViewItem::MOS_DynaObject_ListViewItem( QListViewItem* pParent, MOS_DynaObject& refObject )
    : QListViewItem                          ( pParent, "Objet" )
    , refObject_                             ( refObject )
    , pConstructionPercentageItem_           ( 0 )
    , pValorizationPercentageItem_           ( 0 )
    , pBypassConstructionPercentageItem_     ( 0 )
    , pPreparationItem_                      ( 0 )
    , pSiteFranchissementLargeurItem_        ( 0 )
    , pSiteFranchissementProfondeurItem_     ( 0 )
    , pSiteFranchissementVitesseCourantItem_ ( 0 )
    , pSiteFranchissementBergesAAmenagerItem_( 0 )
    , pNuageNBCAgentNBCItem_                 ( 0 )
    , pRotaNbcAgentsItem_                    ( 0 )
    , pRotaDangerItem_                       ( 0 )
    , pItineraireLogistiqueEquippedItem_     ( 0 )
    , pItineraireLogistiqueFlowItem_         ( 0 )
    , pItineraireLogistiqueWidthItem_        ( 0 )
    , pItineraireLogistiqueLengthItem_       ( 0 )
    , pItineraireLogistiqueMaxWeightItem_    ( 0 )
{
    std::string strLabel( "Objet " );
    strLabel += itostring( refObject.GetID() );
    strLabel += " [" + itostring( refObject.GetIDPlanfie() ) + "]";
    setText( 0, strLabel.c_str() );

    pConstructionPercentageItem_ = new QListViewItem( this, "Construit à " );
    pValorizationPercentageItem_ = new QListViewItem( this, "Valorisé à " );
    pBypassConstructionPercentageItem_ = new QListViewItem( this, "Contourné à " );
    pPreparationItem_ = new QListViewItem( this, "Préparé" );

    refObject.SetViewItem( this );
    Update();
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListViewItem destructor
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_DynaObject_ListViewItem::~MOS_DynaObject_ListViewItem()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListViewItem::Update
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListViewItem::Update()
{
    std::string strLabel0( "Construit à " );
    strLabel0 += itostring( refObject_.GetConstructionPercentage() ) + "%";
    pConstructionPercentageItem_->setText( 0, strLabel0.c_str() );

    std::string strLabel1( "Valorisé à " );
    strLabel1 += itostring( refObject_.GetValorizationPercentage() ) + "%";
    pValorizationPercentageItem_->setText( 0, strLabel1.c_str() );

    std::string strLabel2( "Contourné à " );
    strLabel2 += itostring( refObject_.GetBypassConstructionPercentage() ) + "%";
    pBypassConstructionPercentageItem_->setText( 0, strLabel2.c_str() );

    if( refObject_.IsPrepared() )
        pPreparationItem_->setText( 0, "Préparé" );
    else
        pPreparationItem_->setText( 0, "Non préparé" );

    if( refObject_.AreAttrSiteFranchissementPresent() )
    {
        if( !pSiteFranchissementLargeurItem_ )
            pSiteFranchissementLargeurItem_ = new QListViewItem( this, "Largeur " );
        if( !pSiteFranchissementProfondeurItem_ )
            pSiteFranchissementProfondeurItem_ = new QListViewItem( this, "Profondeur " );
        if( !pSiteFranchissementVitesseCourantItem_ )
            pSiteFranchissementVitesseCourantItem_ = new QListViewItem( this, "Vitesse du courant " );
        if( !pSiteFranchissementBergesAAmenagerItem_ )
            pSiteFranchissementBergesAAmenagerItem_ = new QListViewItem( this, "Berges à aménager " );

        std::string strTmp = "Largeur ";
        strTmp += itostring( refObject_.GetSiteFranchissementLargeur() ) + " m";
        pSiteFranchissementLargeurItem_->setText( 0, strTmp.c_str() );
        
        strTmp = "Profondeur ";
        strTmp += itostring( refObject_.GetSiteFranchissementProfondeur() ) + " m";
        pSiteFranchissementProfondeurItem_->setText( 0, strTmp.c_str() );

        strTmp = "Vitesse du courant ";
        strTmp += itostring( refObject_.GetSiteFranchissementVitesseCourant() ) + " m/s";
        pSiteFranchissementVitesseCourantItem_->setText( 0, strTmp.c_str() );

        if( refObject_.GetSiteFranchissementBergesAAmenager() )
            strTmp = "Berges à aménager";
        else
            strTmp = "Berges aménagées";

        pSiteFranchissementBergesAAmenagerItem_->setText( 0, strTmp.c_str() );
    }

    if( refObject_.AreAttrNuageNBCPresent() )
    {   
        if( !pNuageNBCAgentNBCItem_ )
            pNuageNBCAgentNBCItem_ = new QListViewItem( this, "Agent NBC" );

        std::string strTmp = "Agent NBC ";
    
        strTmp += "'";
        strTmp += MOS_App::GetApp().GetAgentNBCName( refObject_.GetNuageNBCAgentNbcId() );
        strTmp += "'";
        pNuageNBCAgentNBCItem_->setText( 0, strTmp.c_str() );
    }

    if( refObject_.AreAttrItineraireLogistiquePresent() )
    {
        if( !pItineraireLogistiqueEquippedItem_ )
            pItineraireLogistiqueEquippedItem_ = new QListViewItem( this, "Itineraire log equipé" );
        if( !pItineraireLogistiqueFlowItem_ )
            pItineraireLogistiqueFlowItem_     = new QListViewItem( this, "Débit max" );
        if( !pItineraireLogistiqueWidthItem_ )
            pItineraireLogistiqueWidthItem_    = new QListViewItem( this, "Largeur" );
        if( !pItineraireLogistiqueLengthItem_ )
            pItineraireLogistiqueLengthItem_   = new QListViewItem( this, "Longueur" );
        if( !pItineraireLogistiqueMaxWeightItem_ )
            pItineraireLogistiqueMaxWeightItem_ = new QListViewItem( this, "Poids max supporté" );
    
        if( refObject_.IsItineraireLogistiqueEquipped() )
            pItineraireLogistiqueEquippedItem_->setText( 0, "Iti log equipé" );
        else
            pItineraireLogistiqueEquippedItem_->setText( 0, "Iti log non equipé" );        

        std::string strTmp = "Largeur : ";
        strTmp += itostring( refObject_.GetItineraireLogistiqueWidth() ) + " m";
        pItineraireLogistiqueWidthItem_->setText( 0, strTmp.c_str() );

        strTmp = "Longueur : ";
        strTmp += itostring( refObject_.GetItineraireLogistiqueLength() ) + " m";
        pItineraireLogistiqueLengthItem_->setText( 0, strTmp.c_str() );

        strTmp = "Debit : ";
        strTmp += itostring( refObject_.GetItineraireLogistiqueFlow() ) + " ";
        pItineraireLogistiqueFlowItem_->setText( 0, strTmp.c_str() );

        strTmp = "Poids max supporté : ";
        strTmp += itostring( refObject_.GetItineraireLogistiqueMaxWeight() ) + " t";
        pItineraireLogistiqueMaxWeightItem_->setText( 0, strTmp.c_str() );
    }

    if( refObject_.AreAttrRotaPresent() )
    {
        if( !pRotaNbcAgentsItem_ )
            pRotaNbcAgentsItem_ = new QListViewItem( this, "Agents NBC" );
        if( !pRotaDangerItem_ )
            pRotaDangerItem_ = new QListViewItem( this, "Danger" );

        std::string strTmp = "Danger ";
        strTmp += itostring( refObject_.GetRotaDanger() );
        pRotaDangerItem_->setText( 0, strTmp.c_str() );

        strTmp = "Agents NBC ";
        const std::vector< uint >& nbcAgents = refObject_.GetRotaNbcAgents();
        for( std::vector< uint >::const_iterator it = nbcAgents.begin(); it != nbcAgents.end(); ++it )
        {
            uint nID = *it;
            strTmp += "'";
            strTmp += MOS_App::GetApp().GetAgentNBCName( nID );
            strTmp += "' ";
        }
        pRotaNbcAgentsItem_->setText( 0, strTmp.c_str() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListViewItem::rtti
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
int MOS_DynaObject_ListViewItem::rtti() const
{
    return e_rtti;
}

