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
#include "Side.h"
#include "NBCObjectAttribute.h"
#include "LogisticRouteObjectAttribute.h"
#include "CrossingSiteObjectAttribute.h"
#include "RotaObjectAttribute.h"
#include "CampObjectAttribute.h"
#include "MineJamObjectAttribute.h"
#include "DispersedMinedAreaObjectAttribute.h"
#include "LinearMinedAreaObjectAttribute.h"
#include "Network_Def.h"

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
    if( msg.m.attributs_specifiquesPresent )
    {
        switch( nType_ )
        {
            case EnumObjectType::camp_prisonniers:
            case EnumObjectType::camp_refugies:
                pAttributes_ = new CampObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::itineraire_logistique:
                pAttributes_ = new LogisticRouteObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::nuage_nbc:
            case EnumObjectType::zone_nbc:
                pAttributes_ = new NBCObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::rota:
                pAttributes_ = new RotaObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::site_franchissement:
                pAttributes_ = new CrossingSiteObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::bouchon_mines:
                pAttributes_ = new MineJamObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::zone_minee_lineaire:
                pAttributes_ = new LinearMinedAreaObjectAttribute( model, msg.attributs_specifiques );
                break;
            case EnumObjectType::zone_minee_par_dispersion:
                pAttributes_ = new DispersedMinedAreaObjectAttribute( model, msg.attributs_specifiques );
                break;
            default:
                pAttributes_ = 0;
        }
    }
	side_.GetObjects().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::~Object()
{
	side_.GetObjects().Unregister( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Object::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Object::Update( const ASN1T_MsgObjectCreation& )
{
    FlagUpdate();
}

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
    if( msg.m.pourcentage_creation_contournementPresent )
        nPercentageBypassing_ = msg.pourcentage_creation_contournement;

    bPrepared_ = msg.en_preparation;

    if( msg.m.nb_dotation_constructionPresent )
        nNbrDotationForConstruction_ = msg.nb_dotation_construction;
    if( msg.m.nb_dotation_valorisationPresent )
        nNbrDotationForMining_ = msg.nb_dotation_valorisation;
    
    if( msg.m.localisationPresent )
        localisation_.Update( msg.localisation );

    if( pAttributes_ && msg.m.attributs_specifiquesPresent )
        pAttributes_->Update( msg.attributs_specifiques );
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Object::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectCreation asn;

    asn().oid  = nID_;
    asn().type = nType_;
    asn().nom  = strName_.c_str(); // !! pointeur sur const char*
    asn().camp = side_.GetID();

    localisation_.Send( asn().localisation );

    if( nTypeDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.type_dotation_constructionPresent = 1;
        asn().type_dotation_construction          = nTypeDotationForConstruction_;
    }

    if( nTypeDotationForMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.type_dotation_valorisationPresent = 1;
        asn().type_dotation_valorisation          = nTypeDotationForMining_;
    }

    if( pAttributes_ )
    {
        asn().m.attributs_specifiquesPresent = 1;
        pAttributes_->Send( asn().attributs_specifiques );
    }

    asn.Send( publisher );

    Localisation::AsnDelete( asn().localisation );
    if( pAttributes_ )
        pAttributes_->AsnDelete( asn().attributs_specifiques );
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Object::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectUpdate asn;

    asn().oid = nID_;

    if( nPercentageConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.pourcentage_constructionPresent = 1;
        asn().pourcentage_construction          = nPercentageConstruction_;
    }

    if( nPercentageMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.pourcentage_valorisationPresent = 1;
        asn().pourcentage_valorisation          = nPercentageMining_;
    }

    if( nPercentageBypassing_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.pourcentage_creation_contournementPresent = 1;
        asn().pourcentage_creation_contournement          = nPercentageBypassing_;
    }

    asn().en_preparation = bPrepared_;

    if( nNbrDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.nb_dotation_constructionPresent = 1;
        asn().nb_dotation_construction          = nNbrDotationForConstruction_;
    }

    if( nNbrDotationForMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.nb_dotation_valorisationPresent = 1;
        asn().nb_dotation_valorisation          = nNbrDotationForMining_;
    }

    asn().m.localisationPresent = 1;
    localisation_.Send( asn().localisation );

    if( pAttributes_ )
    {
        asn().m.attributs_specifiquesPresent = 1;
        pAttributes_->Send( asn().attributs_specifiques );
    }

    asn.Send( publisher );

    if( pAttributes_ )
        pAttributes_->AsnDelete( asn().attributs_specifiques );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Object::SendDestruction( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientObjectDestruction destruction;
    destruction() = nID_;
    destruction.Send( publisher );
}
