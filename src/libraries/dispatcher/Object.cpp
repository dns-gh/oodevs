// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Object.h"

#include "Model.h"
#include "NBCObjectAttribute.h"
#include "LogisticRouteObjectAttribute.h"
#include "CrossingSiteObjectAttribute.h"
#include "RotaObjectAttribute.h"
#include "CampObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::Object( Model& model, const ASN1T_MsgObjectCreation& msg )
    : nID_                         ( msg.oid  )
    , nType_                       ( msg.type )
    , strName_                     ( msg.nom  )
    , localisation_                ( msg.localisation )
    , side_                        ( model.GetSides().Get( msg.camp ) )
    , nPlannedID_                  ( msg.m.oid_objet_planifiePresent         ? msg.oid_objet_planifie         : std::numeric_limits< unsigned int >::max() )
    , nTypeDotationForConstruction_( msg.m.type_dotation_constructionPresent ? msg.type_dotation_construction : std::numeric_limits< unsigned int >::max() )
    , nTypeDotationForMining_      ( msg.m.type_dotation_valorisationPresent ? msg.type_dotation_valorisation : std::numeric_limits< unsigned int >::max() )
    , nPercentageConstruction_     ( std::numeric_limits< unsigned int >::max() )
    , nPercentageMining_           ( std::numeric_limits< unsigned int >::max() )
    , nPercentageBypassing_        ( std::numeric_limits< unsigned int >::max() )
    , bPrepared_                   ( false )
    , nNbrDotationForConstruction_ ( std::numeric_limits< unsigned int >::max() )
    , nNbrDotationForMining_       ( std::numeric_limits< unsigned int >::max() )
    , pAttributes_                 ( 0 )
{
    switch( nType_ )
    {
        case EnumObjectType::camp_prisonniers:
        case EnumObjectType::camp_refugies:
            pAttributes_ = new CampObjectAttribute( model, msg );           
            break;
        case EnumObjectType::itineraire_logistique:
            pAttributes_ = new LogisticRouteObjectAttribute( model, msg );           
            break;
        case EnumObjectType::nuage_nbc:
        case EnumObjectType::zone_nbc:
            pAttributes_ = new NBCObjectAttribute( model, msg );           
            break;
        case EnumObjectType::rota:
            pAttributes_ = new RotaObjectAttribute( model, msg );
            break;
        case EnumObjectType::site_franchissement:
            pAttributes_ = new CrossingSiteObjectAttribute( model, msg );
            break;
        default:
            pAttributes_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::~Object()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Object::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Object::Update( const ASN1T_MsgObjectUpdate& msg )
{
    if( msg.m.pourcentage_constructionPresent )
        nPercentageConstruction_ = msg.pourcentage_construction;
    if( msg.m.pourcentage_valorisationPresent )
        nPercentageMining_ = msg.pourcentage_valorisation;
    if( msg.m.pourcentage_constructionPresent )
        nPercentageBypassing_ = msg.pourcentage_creation_contournement;

    bPrepared_ = msg.en_preparation;

    if( msg.m.nb_dotation_constructionPresent )
        nNbrDotationForConstruction_ = msg.nb_dotation_construction;
    if( msg.m.nb_dotation_valorisationPresent )
        nNbrDotationForMining_ = msg.nb_dotation_valorisation;
    
    if( msg.m.localisationPresent )
        localisation_.Update( msg.localisation );

    if( pAttributes_ )
        pAttributes_->Update( msg );
}
