// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ObjectKnowledge.h"

#include "Network_Def.h"
#include "Model.h"
#include "Side.h"
#include "Object.h"
#include "Automat.h"
#include "NBCObjectAttribute.h"
#include "LogisticRouteObjectAttribute.h"
#include "CrossingSiteObjectAttribute.h"
#include "RotaObjectAttribute.h"
#include "CampObjectAttribute.h"
#include "MineJamObjectAttribute.h"
#include "LinearMinedAreaObjectAttribute.h"
#include "DispersedMinedAreaObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( Model& model, const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
    : model_                        ( model )
    , nID_                          ( asnMsg.oid_connaissance )
    , side_                         ( model.GetSides().Get( asnMsg.oid_camp_possesseur ) )
    , pObject_                      ( model.GetObjects().Find( asnMsg.oid_objet_reel ) )
    , nType_                        ( asnMsg.type )
    , nObstacleType_                ( asnMsg.m.type_obstaclePresent ? asnMsg.type_obstacle : EnumTypeObstacle::preliminaire )
    , nTypeDotationForConstruction_ ( asnMsg.m.type_dotation_constructionPresent ? asnMsg.type_dotation_construction : std::numeric_limits< unsigned int >::max() )
    , nTypeDotationForMining_       ( asnMsg.m.type_dotation_valorisationPresent ? asnMsg.type_dotation_valorisation : std::numeric_limits< unsigned int >::max() )
    , pAttributes_                  ( 0 )
    , nRelevance_                   ( std::numeric_limits< unsigned int >::max() )
    , localisation_                 ()
    , nConstructionPercentage_      ( std::numeric_limits< unsigned int >::max() )
    , nMiningPercentage_            ( std::numeric_limits< unsigned int >::max() )
    , nBypassingPercentage_         ( std::numeric_limits< unsigned int >::max() )
    , bReservedObstacleActivated_   ( asnMsg.m.obstacle_de_manoeuvre_activePresent ? asnMsg.obstacle_de_manoeuvre_active : false )
    , bPerceived_                   ( false )
    , automatPerceptions_           ()
    , nNbrDotationForConstruction_  ( std::numeric_limits< unsigned int >::max() )
    , nNbrDotationForMining_        ( std::numeric_limits< unsigned int >::max() )
{
    //$$ BULLSHIT // $$$$ AGE 2007-05-11: Clair !
    optionals_.pertinencePresent                   = 0;
    optionals_.localisationPresent                 = 0;
    optionals_.pourcentage_constructionPresent     = 0;
    optionals_.pourcentage_valorisationPresent     = 0;
    optionals_.pourcentage_contournementPresent    = 0;
    optionals_.est_percuPresent                    = 0;
    optionals_.attributs_specifiquesPresent        = 0;
    optionals_.perception_par_compagniePresent     = 0;
    optionals_.nb_dotation_constructionPresent     = 0;
    optionals_.nb_dotation_valorisationPresent     = 0;
    optionals_.type_obstaclePresent                = asnMsg.m.type_obstaclePresent;
    optionals_.obstacle_de_manoeuvre_activePresent = asnMsg.m.obstacle_de_manoeuvre_activePresent;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void ObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    bool realObjectChanged = ( message.oid_objet_reel && ! pObject_ )
                          || ( pObject_ && pObject_->GetID() != message.oid_objet_reel );

    FlagUpdate( realObjectChanged );
    if( realObjectChanged )
        pObject_ = model_.GetObjects().Find( message.oid_objet_reel );
}

// =============================================================================
// OPERATIONS
// =============================================================================

#define UPDATE_ASN_ATTRIBUTE( ASN, CPP ) \
    if( asnMsg.m.##ASN##Present )        \
    {                                    \
        CPP = asnMsg.##ASN;              \
        optionals_.##ASN##Present = 1;   \
    }

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.attributs_specifiquesPresent )
    {
        if( !pAttributes_ )
        {
            switch( nType_ )
            {
                case EnumObjectType::camp_prisonniers:
                case EnumObjectType::camp_refugies:
                    pAttributes_ = new CampObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::itineraire_logistique:
                    pAttributes_ = new LogisticRouteObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::nuage_nbc:
                case EnumObjectType::zone_nbc:
                    pAttributes_ = new NBCObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::rota:
                    pAttributes_ = new RotaObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::site_franchissement:
                    pAttributes_ = new CrossingSiteObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::bouchon_mines:
                    pAttributes_ = new MineJamObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::zone_minee_lineaire:
                    pAttributes_ = new LinearMinedAreaObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                case EnumObjectType::zone_minee_par_dispersion:
                    pAttributes_ = new DispersedMinedAreaObjectAttribute( model_, asnMsg.attributs_specifiques );
                    break;
                default:
                    pAttributes_ = 0;
            }
        }
        pAttributes_->Update( asnMsg.attributs_specifiques );
        optionals_.attributs_specifiquesPresent = 1;
    }
   
    if( asnMsg.m.localisationPresent )
    {
        localisation_.Update( asnMsg.localisation );
        optionals_.localisationPresent = 1;
    }

    if( asnMsg.m.perception_par_compagniePresent )
    {
        optionals_.perception_par_compagniePresent = 1;
        automatPerceptions_.Clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
            automatPerceptions_.Register( model_.GetAutomats().Get( asnMsg.perception_par_compagnie.elem[ i ]) );
    }

    if( asnMsg.m.oid_objet_reelPresent )
        pObject_ = model_.GetObjects().Find( asnMsg.oid_objet_reel );

    UPDATE_ASN_ATTRIBUTE( pertinence                  , nRelevance_                   );
    UPDATE_ASN_ATTRIBUTE( pourcentage_construction    , nConstructionPercentage_      );
    UPDATE_ASN_ATTRIBUTE( pourcentage_valorisation    , nMiningPercentage_            );
    UPDATE_ASN_ATTRIBUTE( pourcentage_contournement   , nBypassingPercentage_         );
    UPDATE_ASN_ATTRIBUTE( est_percu                   , bPerceived_                   );
    UPDATE_ASN_ATTRIBUTE( nb_dotation_construction    , nNbrDotationForConstruction_  );
    UPDATE_ASN_ATTRIBUTE( nb_dotation_valorisation    , nNbrDotationForMining_        );
    UPDATE_ASN_ATTRIBUTE( obstacle_de_manoeuvre_active, bReservedObstacleActivated_   );
}

#define SEND_ASN_ATTRIBUTE( ASN, CPP )  \
    if( optionals_.##ASN##Present )     \
    {                                   \
        asn().m.##ASN##Present = 1;     \
        asn().##ASN = CPP;              \
    }

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectKnowledgeCreation asn;

    asn().oid_connaissance    = nID_;
    asn().oid_camp_possesseur = side_.GetID();
    asn().oid_objet_reel      = pObject_ ? pObject_->GetID() : 0;
    asn().type                = nType_;

    if( nTypeDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.type_dotation_constructionPresent = 1;
        asn().type_dotation_construction = nTypeDotationForConstruction_;
    }

    if( nTypeDotationForMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.type_dotation_valorisationPresent = 1;
        asn().type_dotation_valorisation = nTypeDotationForMining_;
    }

    SEND_ASN_ATTRIBUTE( type_obstacle               , nObstacleType_              );
    SEND_ASN_ATTRIBUTE( obstacle_de_manoeuvre_active, bReservedObstacleActivated_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectKnowledgeUpdate asn;

    asn().oid_connaissance      = nID_;
    asn().oid_camp_possesseur   = side_.GetID();

    asn().m.oid_objet_reelPresent = 1;
    asn().oid_objet_reel          = pObject_ ? pObject_->GetID() : 0;

    if( optionals_.attributs_specifiquesPresent && pAttributes_ )
    {
        asn().m.attributs_specifiquesPresent = 1;
        pAttributes_->Send( asn().attributs_specifiques );
    }

    if( optionals_.localisationPresent )
    {
        asn().m.localisationPresent = 1;
        localisation_.Send( asn().localisation );
    }

    if( optionals_.perception_par_compagniePresent )
    {
        asn().m.perception_par_compagniePresent = 1;
        automatPerceptions_.Send< ASN1T_ListOID, ASN1T_OID >( asn().perception_par_compagnie );
    }

    SEND_ASN_ATTRIBUTE( pertinence                  , nRelevance_                   );
    SEND_ASN_ATTRIBUTE( pourcentage_construction    , nConstructionPercentage_      );
    SEND_ASN_ATTRIBUTE( pourcentage_valorisation    , nMiningPercentage_            );
    SEND_ASN_ATTRIBUTE( pourcentage_contournement   , nBypassingPercentage_         );   
    SEND_ASN_ATTRIBUTE( obstacle_de_manoeuvre_active, bReservedObstacleActivated_   );    
    SEND_ASN_ATTRIBUTE( est_percu                   , bPerceived_                   );
    SEND_ASN_ATTRIBUTE( nb_dotation_construction    , nNbrDotationForConstruction_  );
    SEND_ASN_ATTRIBUTE( nb_dotation_valorisation    , nNbrDotationForMining_        );

    asn.Send( publisher );

    if( asn().m.attributs_specifiquesPresent && pAttributes_ )
        pAttributes_->AsnDelete( asn().attributs_specifiques );

    if( asn().m.localisationPresent )
        Localisation::AsnDelete( asn().localisation );

    if( asn().m.perception_par_compagniePresent && asn().perception_par_compagnie.n > 0 )
        delete [] asn().perception_par_compagnie.elem;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendDestruction( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectKnowledgeDestruction asn;
    asn().oid_connaissance    = nID_;
    asn().oid_camp_possesseur = side_.GetID();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendSpecialUpdate
// Created: AGE 2007-05-11
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendSpecialUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectKnowledgeUpdate asn;

    asn().oid_connaissance      = nID_;
    asn().oid_camp_possesseur   = side_.GetID();
    asn().m.oid_objet_reelPresent = 1;
    asn().oid_objet_reel          = pObject_ ? pObject_->GetID() : 0;
    asn.Send( publisher );
}
