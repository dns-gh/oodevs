//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledge.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:15 $
// $Revision: 3 $
// $Workfile: ObjectKnowledge.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledge.h"

#include "App.h"
#include "World.h"
#include "ASN_Messages.h"
#include "AgentManager.h"
#include "ObjectManager.h"
#include "Object_ABC.h"
#include "Model.h"

//$$$$ mise a jour de pRealObject_ suite a un delete de l'objet réel non fait.

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg, Team& owner )
    : owner_                    ( owner )
    , nID_                      ( asnMsg.oid_connaissance ) 
    , nObjectTypeID_            ( asnMsg.type )
    , pRealObject_              ( asnMsg.oid_objet_reel == 0 ? 0 : App::GetApp().GetObjectManager().FindObject( asnMsg.oid_objet_reel ) ) 
{
    Object_ABC::GetIDManagerForObjectType( asnMsg.type ).LockIdentifier( asnMsg.oid_connaissance ) ;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void ObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.oid_objet_reelPresent )
    {
        if( asnMsg.oid_objet_reel )
            pRealObject_ = App::GetApp().GetObjectManager().FindObject( asnMsg.oid_objet_reel );
        else
            pRealObject_ = 0;
    }

    if( asnMsg.m.pertinencePresent )
        nRelevance_ = asnMsg.pertinence;

    if( asnMsg.m.localisationPresent )
    {
        points_.Set();
        points_.Data().clear();
        vCenter_.Reset();
        for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vPos;
            App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vPos );    
            points_.Data().push_back( vPos );
            vCenter_ += vPos;
        }
        if( points_.Data().size() > 1 )
            vCenter_ /= points_.Data().size();
        std::string pos;
        App::GetApp().GetWorld().SimToMosMgrsCoord( vCenter_, pos );
        strPos_ = pos;
    }

    if( asnMsg.m.pourcentage_constructionPresent )
        nPourcentageConstruction_ = asnMsg.pourcentage_construction;
    
    if( asnMsg.m.pourcentage_valorisationPresent )
        nPourcentageValorisation_ = asnMsg.pourcentage_valorisation;
    
    if( asnMsg.m.pourcentage_contournementPresent )
        nPourcentageContournement_ = asnMsg.pourcentage_contournement;
    
    if( asnMsg.m.en_preparationPresent )
        bEnPreparation_ = asnMsg.en_preparation;
    
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = asnMsg.est_percu;

    if( asnMsg.m.perception_par_compagniePresent )
    {
        automatePerceptionSet_.Set();
        automatePerceptionSet_.Data().clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
        {
            Agent* pAgent = & App::GetApp().GetModel().GetAgent( asnMsg.perception_par_compagnie.elem[i] );
            automatePerceptionSet_.Data().insert( pAgent );
        }
    }

    if( asnMsg.m.attributs_specifiquesPresent )
    {
        if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
        {
            nSiteFranchissementLargeur_         = asnMsg.attributs_specifiques.u.site_franchissement->largeur;
            nSiteFranchissementProfondeur_      = asnMsg.attributs_specifiques.u.site_franchissement->profondeur;
            nSiteFranchissementVitesseCourant_  = asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant;
            nSiteFranchissementBergesAAmenager_ = asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
            nNuageNBCAgentNbcID_  = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
            nNuageNBCAgentNbcID_  = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_camp_prisonniers )
            nCampTC2ID_ = asnMsg.attributs_specifiques.u.camp_prisonniers->tc2;
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_camp_refugies )
            nCampTC2ID_ = asnMsg.attributs_specifiques.u.camp_refugies->tc2;
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_rota )
        {
            nROTADanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
            for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
                rotaNBCAgents_.Data().push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[ i ] );
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
        {
            nLogRouteFlow_      = asnMsg.attributs_specifiques.u.itineraire_logistique->debit;
            nLogRouteLength_    = asnMsg.attributs_specifiques.u.itineraire_logistique->longueur;
            nLogRouteWidth_     = asnMsg.attributs_specifiques.u.itineraire_logistique->largeur;
            nLogRouteMaxWeight_ = asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
            bLogRouteEquipped_  = asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
        }
    }
}

