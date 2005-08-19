//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectKnowledge.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:15 $
// $Revision: 3 $
// $Workfile: MOS_DynaObjectKnowledge.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_DynaObjectKnowledge.h"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ASN_Messages.h"
#include "MOS_AgentManager.h"
#include "MOS_DynaObjectManager.h"

#ifndef MOS_USE_INLINE
#	include "MOS_DynaObjectKnowledge.inl"
#endif

//$$$$ mise a jour de pRealObject_ suite a un delete de l'objet réel non fait.


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_DynaObjectKnowledge::MOS_DynaObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg, MOS_Team& owner )
    : owner_                    ( owner )
    , nAttrUpdated_             ( 0 ) 
    , nID_                      ( asnMsg.oid_connaissance ) 
    , pRealObject_              ( asnMsg.oid_objet_reel == 0 ? 0 : MOS_App::GetApp().GetDynaObjectManager().FindDynaObject( asnMsg.oid_objet_reel ) ) 
    , vCenter_                  ()
    , points_                   ()     
    , nPourcentageConstruction_ ( 0 ) 
    , nPourcentageValorisation_ ( 0 ) 
    , nPourcentageContournement_( 0 ) 
    , bEnPreparation_           ( false ) 
    , bIsPerceived_             ( true ) 
    , automatePerceptionSet_    () 
    , nRelevance_               ( 0 ) 
    , nObjectTypeID_            ( asnMsg.type )
    , nSiteFranchissementLargeur_           ( 0 )
    , nSiteFranchissementProfondeur_        ( 0 )
    , nSiteFranchissementVitesseCourant_    ( 0 )
    , nSiteFranchissementBergesAAmenager_   ( false )
    , nNuageNBCAgentNbcID_                  ( 0 )  
{
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_DynaObjectKnowledge::~MOS_DynaObjectKnowledge()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
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

    if( asnMsg.m.perception_par_compagniePresent )
    {
        automatePerceptionSet_.clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
        {
            MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.perception_par_compagnie.elem[i] );
            assert( pAgent );
            automatePerceptionSet_.insert( pAgent );
        }
        nAttrUpdated_ |= eUpdated_AutomatePerception;
    }

    if( asnMsg.m.attributs_specifiquesPresent )
    {
        if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
        {
            nSiteFranchissementLargeur_         = asnMsg.attributs_specifiques.u.site_franchissement->largeur;
            nSiteFranchissementProfondeur_      = asnMsg.attributs_specifiques.u.site_franchissement->profondeur;
            nSiteFranchissementVitesseCourant_  = asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant;
            nSiteFranchissementBergesAAmenager_ = asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager;
            nAttrUpdated_ |= eUpdated_RiverCrossing;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        {
            nNuageNBCAgentNbcID_  = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
            nAttrUpdated_ |= eUpdated_NBCCloud;
        }
    }
}

