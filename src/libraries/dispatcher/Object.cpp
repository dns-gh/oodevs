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
    , nObstacleType_               ( msg.m.obstacle_typePresent ? msg.obstacle_type : EnumObstacleType::initial )
    , strName_                     ( msg.name  )
    , localisation_                ( msg.location )
    , side_                        ( model.GetSides().Get( msg.team ) )
    , nTypeDotationForConstruction_( msg.m.construction_dotation_typePresent ? msg.construction_dotation_type : std::numeric_limits< unsigned int >::max() )
    , nTypeDotationForMining_      ( msg.m.mining_dotation_typePresent       ? msg.mining_dotation_type       : std::numeric_limits< unsigned int >::max() )
    , nPercentageConstruction_     ( std::numeric_limits< unsigned int >::max() )
    , nPercentageMining_           ( std::numeric_limits< unsigned int >::max() )
    , nPercentageBypassing_        ( std::numeric_limits< unsigned int >::max() )
    , bReservedObstacleActivated_  ( msg.m.reserved_obstacle_activatedPresent ? msg.reserved_obstacle_activated : false )
    , nNbrDotationForConstruction_ ( std::numeric_limits< unsigned int >::max() )
    , nNbrDotationForMining_       ( std::numeric_limits< unsigned int >::max() )
    , pAttributes_                 ( 0 )
{
    optionals_.obstacle_de_manoeuvre_activePresent  = msg.m.reserved_obstacle_activatedPresent;
    optionals_.type_obstaclePresent                 = msg.m.obstacle_typePresent;

    if( msg.m.specific_attributesPresent )
    {
        switch( nType_ )
        {
            case EnumObjectType::camp_prisonniers:
            case EnumObjectType::camp_refugies:
                pAttributes_ = new CampObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::itineraire_logistique:
                pAttributes_ = new LogisticRouteObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::nuage_nbc:
            case EnumObjectType::zone_nbc:
                pAttributes_ = new NBCObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::rota:
                pAttributes_ = new RotaObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::site_franchissement:
                pAttributes_ = new CrossingSiteObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::bouchon_mines:
                pAttributes_ = new MineJamObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::zone_minee_lineaire:
                pAttributes_ = new LinearMinedAreaObjectAttribute( model, msg.specific_attributes );
                break;
            case EnumObjectType::zone_minee_par_dispersion:
                pAttributes_ = new DispersedMinedAreaObjectAttribute( model, msg.specific_attributes );
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
    if( msg.m.construction_percentagePresent )
        nPercentageConstruction_ = msg.construction_percentage;
    if( msg.m.mining_percentagePresent )
        nPercentageMining_ = msg.mining_percentage;
    if( msg.m.bypass_construction_percentagePresent )
        nPercentageBypassing_ = msg.bypass_construction_percentage;
    if( msg.m.reserved_obstacle_activatedPresent )
    {
        optionals_.obstacle_de_manoeuvre_activePresent = 1;
        bReservedObstacleActivated_ = msg.reserved_obstacle_activated;
    }
    if( msg.m.construction_dotation_nbrPresent )
        nNbrDotationForConstruction_ = msg.construction_dotation_nbr;
    if( msg.m.mining_dotation_nbrPresent )
        nNbrDotationForMining_ = msg.mining_dotation_nbr;
    
    if( msg.m.locationPresent )
        localisation_.Update( msg.location );

    if( pAttributes_ && msg.m.specific_attributesPresent )
        pAttributes_->Update( msg.specific_attributes );
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
    asn().name = strName_.c_str();
    asn().team = side_.GetID();

    if( optionals_.obstacle_de_manoeuvre_activePresent )
    {
        asn().m.reserved_obstacle_activatedPresent = 1;
        asn().reserved_obstacle_activated = bReservedObstacleActivated_;
    }
    if( optionals_.type_obstaclePresent )
    {
        asn().m.obstacle_typePresent = 1;
        asn().obstacle_type = nObstacleType_;
    }

    localisation_.Send( asn().location );

    if( nTypeDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.construction_dotation_typePresent = 1;
        asn().construction_dotation_type          = nTypeDotationForConstruction_;
    }

    if( nTypeDotationForMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.mining_dotation_typePresent = 1;
        asn().mining_dotation_type          = nTypeDotationForMining_;
    }

    if( pAttributes_ )
    {
        asn().m.specific_attributesPresent = 1;
        pAttributes_->Send( asn().specific_attributes );
    }

    asn.Send( publisher );

    Localisation::AsnDelete( asn().location );
    if( pAttributes_ )
        pAttributes_->AsnDelete( asn().specific_attributes );
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
        asn().m.construction_percentagePresent = 1;
        asn().construction_percentage          = nPercentageConstruction_;
    }

    if( nPercentageMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.mining_percentagePresent = 1;
        asn().mining_percentage          = nPercentageMining_;
    }

    if( nPercentageBypassing_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.bypass_construction_percentagePresent = 1;
        asn().bypass_construction_percentage          = nPercentageBypassing_;
    }

    if( optionals_.obstacle_de_manoeuvre_activePresent )
    {
        asn().m.reserved_obstacle_activatedPresent = 1;
        asn().reserved_obstacle_activated = bReservedObstacleActivated_;
    }

    if( nNbrDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.construction_dotation_nbrPresent = 1;
        asn().construction_dotation_nbr          = nNbrDotationForConstruction_;
    }

    if( nNbrDotationForMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.mining_dotation_nbrPresent = 1;
        asn().mining_dotation_nbr          = nNbrDotationForMining_;
    }

    asn().m.locationPresent = 1;
    localisation_.Send( asn().location );

    if( pAttributes_ )
    {
        asn().m.specific_attributesPresent = 1;
        pAttributes_->Send( asn().specific_attributes );
    }

    asn.Send( publisher );

    if( pAttributes_ )
        pAttributes_->AsnDelete( asn().specific_attributes );
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
