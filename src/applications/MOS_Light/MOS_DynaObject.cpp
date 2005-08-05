//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 14:54 $
// $Revision: 24 $
// $Workfile: MOS_DynaObject.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_DynaObject.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject.inl"
#endif

#include "MOS_Net_Def.h"
#include "MOS_App.h"
#include "MOS_World.h"

#include "MOS_DynaObject_ListViewItem.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject constructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObject::MOS_DynaObject()
    : rConstructionPercentage_           ( 0. )
    , rValorizationPercentage_           ( 0. )
    , rBypassConstructionPercentage_     ( 0. )
    , bPrepared_                         ( false )
    , pItem_                             ( 0 )
    , nSiteFranchissementLargeur_        ( 0 )
    , nSiteFranchissementProfondeur_     ( 0 )
    , nSiteFranchissementVitesseCourant_ ( 0 )
    , nSiteFranchissementBergesAAmenager_( false )
    , bAttrSiteFranchissementPresent_    ( false )
    , bAttrNuageNBCPresent_              ( false )
    , bAttrRotaPresent_                  ( false )
    , nNuageNBCAgentNbcID_               ( 0 )
    , bAttrItineraireLogisticPresent_    ( false )
    , bItineraireLogistiqueEquipped_     ( false )
    , nItineraireLogistiqueFlow_         ( 0 )
    , nItineraireLogistiqueWidth_        ( 0 )
    , nItineraireLogistiqueLength_       ( 0 )
    , nItineraireLogistiqueMaxWeight_    ( 0 )
    , strTypeDotationConstruction_       ()
    , strTypeDotationValorization_       ()
    , nNbrDotationConstruction_          ( 0 )
    , nNbrDotationValorization_          ( 0 )
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject destructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObject::~MOS_DynaObject()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::Initialize
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_DynaObject::Initialize( const ASN1T_MsgObjectCreation& asnMsg )
{
    rConstructionPercentage_       = 0.;
    rValorizationPercentage_       = 0.;
    rBypassConstructionPercentage_ = 0.;
    bPrepared_                     = false;

    nID_         = asnMsg.oid;
    nType_       = asnMsg.type;
    nIDPlanifie_ = asnMsg.oid_objet_planifie;

    center_.Reset();
    pointVector_.clear();
    nTypeLocalisation_ = asnMsg.localisation.type;
    for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vTmp );
        pointVector_.push_back( vTmp );
        center_ += vTmp;
    }
    if( pointVector_.size() > 1 )
        center_ /= pointVector_.size();

    if( asnMsg.m.type_dotation_constructionPresent  )
    {
        strTypeDotationConstruction_ = MOS_App::GetApp().GetRessourceName( asnMsg.type_dotation_construction );
        nNbrDotationConstruction_    = 0;
    }
    
    if( asnMsg.m.type_dotation_valorisationPresent )
    {
        strTypeDotationValorization_ = MOS_App::GetApp().GetRessourceName( asnMsg.type_dotation_valorisation );
        nNbrDotationValorization_    = 0;
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
            nNuageNBCAgentNbcID_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        {
            bAttrNuageNBCPresent_ = true;
            nNuageNBCAgentNbcID_ = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_rota )
        {
            bAttrRotaPresent_ = true;
            nRotaDanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
            rotaNbcAgents_.clear();
            for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
                rotaNbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[i] );
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
        {
            bAttrItineraireLogisticPresent_ = true;
            bItineraireLogistiqueEquipped_  = asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
            nItineraireLogistiqueFlow_      = asnMsg.attributs_specifiques.u.itineraire_logistique->debit;
            nItineraireLogistiqueWidth_     = asnMsg.attributs_specifiques.u.itineraire_logistique->largeur;
            nItineraireLogistiqueLength_    = asnMsg.attributs_specifiques.u.itineraire_logistique->longueur;
            nItineraireLogistiqueMaxWeight_ = asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
        }
    }
    
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ObjectCreation - ID: " << (int)nID_;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::Terminate
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_DynaObject::Terminate()
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ObjectDestruction - ID: " << (int)nID_;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::Draw
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_DynaObject::Draw()
{
    GFX_Color color( 255, 255, 128, 0.5 );

    switch( nTypeLocalisation_ )
    {
        case EnumTypeLocalisation::point  : GFX_Tools::CreateGLCross( *pointVector_.begin(), 500, 1.0, color ); break;
        case EnumTypeLocalisation::line: GFX_Tools::CreateGLLines( pointVector_, 1.0, color ); break;
        case EnumTypeLocalisation::polygon: GFX_Tools::CreateGLLines( pointVector_, 1.0, color ); break;
        default:
            GFX_Tools::CreateGLLines( pointVector_, 1.0, color );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::Update
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_DynaObject::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    bPrepared_ = asnMsg.en_preparation;

    if( asnMsg.m.nb_dotation_constructionPresent )
        nNbrDotationConstruction_ = asnMsg.nb_dotation_construction;

    if( asnMsg.m.nb_dotation_valorisationPresent )
        nNbrDotationValorization_ = asnMsg.nb_dotation_valorisation;

    if( asnMsg.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = asnMsg.pourcentage_construction;
    if( asnMsg.m.pourcentage_valorisationPresent )
        rValorizationPercentage_ = asnMsg.pourcentage_valorisation;
    if( asnMsg.m.pourcentage_creation_contournementPresent )
        rBypassConstructionPercentage_ = asnMsg.pourcentage_creation_contournement;
    if( asnMsg.m.localisationPresent )
    {
        center_.Reset();
        pointVector_.clear();
        nTypeLocalisation_ = asnMsg.localisation.type;
        for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vTmp;
            MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vTmp );
            pointVector_.push_back( vTmp );
            center_ += vTmp;
        }
        if( pointVector_.size() > 1 )
            center_ /= pointVector_.size();
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
            nNuageNBCAgentNbcID_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        {
            bAttrNuageNBCPresent_ = true;
            nNuageNBCAgentNbcID_ = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_rota )
        {
            bAttrRotaPresent_ = true;
            nRotaDanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
            rotaNbcAgents_.clear();
            for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
                rotaNbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[i] );
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
        {
            bAttrItineraireLogisticPresent_ = true;
            bItineraireLogistiqueEquipped_ = asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;

        }
    }

    //$$$ A COMPLETER DANS AFFICHAGE

    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ObjectUpdate - ID: " << (int)nID_ 
                 << " Prepared: " << (int)bPrepared_
                 << " Construction :" << rConstructionPercentage_ << "%"
                 << " Valorisation :" << rValorizationPercentage_ << "%"
                 << " Contournement :" << rBypassConstructionPercentage_ << "%";
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    if( pItem_ != 0 )
        pItem_->Update();
}
