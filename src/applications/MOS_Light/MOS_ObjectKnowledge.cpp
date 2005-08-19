//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 14:57 $
// $Revision: 17 $
// $Workfile: MOS_ObjectKnowledge.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_ObjectKnowledge.h"

#include "GFX/GFX_Color.h"
#include "MOS_App.h"
#include "MOS_WorldWidget.h"
#include "MOS_World.h"
#include "MOS_MainWindow.h"
#include "MOS_ASN_Messages.h"
#include "MOS_EnemyInfo_Localisation.h"
#include "MOS_EnemyInfo_Direction.h"
#include "MOS_PointListViewItem.h"
#include "MOS_Value.h"
#include "MOS_Tools.h"
#include "MOS_AgentManager.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_ObjectKnowledge_ListView_Item.h"

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_ObjectKnowledge::MOS_ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
    : pListViewItem_                        ( 0 ) 
    , nAttrUpdated_                         ( 0 ) 
    , nID_                                  ( asnMsg.oid_connaissance ) 
    , pRealObject_                          ( asnMsg.oid_objet_reel == 0 ? 0 : MOS_App::GetApp().GetDynaObjectManager().FindDynaObject( asnMsg.oid_objet_reel ) ) 
    , vCenter_                              ()
    , points_                               ()     
    , nPourcentageConstruction_             ( 0 ) 
    , nPourcentageValorisation_             ( 0 ) 
    , nPourcentageContournement_            ( 0 ) 
    , bEnPreparation_                       ( false ) 
    , bIsPerceived_                         ( true ) 
    , automatePerceptionSet_                () 
    , nRelevance_                           ( 0 ) 
    , nObjectTypeID_                        ( asnMsg.type )
    , bAttrSiteFranchissementPresent_       ( false )
    , nSiteFranchissementLargeur_           ( 0 )
    , nSiteFranchissementProfondeur_        ( 0 )
    , nSiteFranchissementVitesseCourant_    ( 0 )
    , nSiteFranchissementBergesAAmenager_   ( false )
    , bAttrNuageNBCPresent_                 ( false )
    , bAttrRotaPresent_                     ( false )
    , nNuageNBCAgentNbcID_                  ( 0 )    
    , bAttrItiLogPresent_                   ( false )
    , bItiLogEquipped_                      ( false )
    , nItiLogistiqueFlow_                   ( 0 )
    , nItiLogistiqueWidth_                  ( 0 )
    , nItiLogistiqueLength_                 ( 0 )
    , nItiLogistiqueMaxWeight_              ( 0 )
{
}



// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_ObjectKnowledge::~MOS_ObjectKnowledge()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.oid_objet_reelPresent )
    {
        if( asnMsg.oid_objet_reel )
            pRealObject_ = MOS_App::GetApp().GetDynaObjectManager().FindDynaObject( asnMsg.oid_objet_reel );
        else
            pRealObject_ = 0;
        nAttrUpdated_ |= eUpdated_RealObject;
    }
    
    if( asnMsg.m.pertinencePresent )
    {
        nRelevance_ = asnMsg.pertinence;
        nAttrUpdated_ |= eUpdated_Relevance;
    }
    
    if( asnMsg.m.localisationPresent )
    {
        points_.clear();

        vCenter_.Reset();
        for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vPos;
            MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vPos );    
            points_.push_back( vPos );
            vCenter_ += vPos;
        }
        if( points_.size() > 1 )
            vCenter_ /= points_.size();

        nAttrUpdated_ |= eUpdated_Localisation;
    }
    
    if( asnMsg.m.pourcentage_constructionPresent )
    {
        nPourcentageConstruction_ = asnMsg.pourcentage_construction;
        nAttrUpdated_ |= eUpdated_PourcentageConstruction;
    }
    
    if( asnMsg.m.pourcentage_valorisationPresent )
    {
        nPourcentageValorisation_ = asnMsg.pourcentage_valorisation;
        nAttrUpdated_ |= eUpdated_PourcentageValorisation;
    }
    
    if( asnMsg.m.pourcentage_contournementPresent )
    {
        nPourcentageContournement_ = asnMsg.pourcentage_contournement;
        nAttrUpdated_ |= eUpdated_PourcentageContournement;
    }
    
    if( asnMsg.m.en_preparationPresent )
    {
        bEnPreparation_ = asnMsg.en_preparation;
        nAttrUpdated_ |= eUpdated_EnPreparation;
    }
    
    if( asnMsg.m.est_percuPresent )
    {
        bIsPerceived_ = asnMsg.est_percu;
        nAttrUpdated_ |= eUpdated_IsPerceived;
    }

    if( asnMsg.m.attributs_specifiquesPresent )
    {
        //$$$ TODO
    }

    if( asnMsg.m.perception_par_compagniePresent )
    {
        nAttrUpdated_ |= eUpdated_AutomatePerception;
        automatePerceptionSet_.clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
        {
            MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.perception_par_compagnie.elem[i] );
            if( pAgent )
                automatePerceptionSet_.insert( pAgent );
        }
    }        


    if( asnMsg.m.attributs_specifiquesPresent )
    {
        if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
        {
            bAttrSiteFranchissementPresent_     = true;
            nSiteFranchissementLargeur_         = asnMsg.attributs_specifiques.u.site_franchissement->largeur;
            nSiteFranchissementProfondeur_      = asnMsg.attributs_specifiques.u.site_franchissement->profondeur;
            nSiteFranchissementVitesseCourant_  = asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant;
            nSiteFranchissementBergesAAmenager_ = asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        {
            bAttrNuageNBCPresent_ = true;
            nNuageNBCAgentNbcID_  = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        {
            bAttrNuageNBCPresent_ = true;
            nNuageNBCAgentNbcID_  = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_rota )
        {
            bAttrRotaPresent_ = true;
            nRotaDanger_      = asnMsg.attributs_specifiques.u.rota->niveau_danger;
            rotaNbcAgents_.clear();
            for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
                rotaNbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[i] );
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
        {
            bAttrItiLogPresent_      = true;
            bItiLogEquipped_         = asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
            nItiLogistiqueFlow_      = asnMsg.attributs_specifiques.u.itineraire_logistique->debit;
            nItiLogistiqueWidth_     = asnMsg.attributs_specifiques.u.itineraire_logistique->largeur;
            nItiLogistiqueLength_    = asnMsg.attributs_specifiques.u.itineraire_logistique->longueur;
            nItiLogistiqueMaxWeight_ = asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
        }
    }

    if( pListViewItem_ )
        pListViewItem_->Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::SetListViewItem
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledge::SetListViewItem( MOS_ObjectKnowledge_ListView_Item* pListViewItem )
{
    pListViewItem_ = pListViewItem;
    if( pListViewItem_ )
        pListViewItem_->Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::Draw
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledge::Draw()
{
    static const MT_Float rCrossSize = 100;
    if( points_.size() == 1 ) 
        GFX_Tools::CreateGLCross( points_[0], rCrossSize, 4.0, GFX_Color( 255, 255, 255 ) );
    else
        GFX_Tools::CreateGLLines( points_, 4.0, GFX_Color( 255, 255, 255 ) );

    /*if( !rotaSecurityZone_.empty() )
    {
        if( rotaSecurityZone_.size() == 1 ) 
            GFX_Tools::CreateGLCross( rotaSecurityZone_[0], rCrossSize, 4.0, GFX_Color( 255, 255, 255, 0.5 ) );
        else
            GFX_Tools::CreateGLLines( rotaSecurityZone_, 4.0, GFX_Color( 255, 255, 255, 0.5 ) );
    }*/
}

