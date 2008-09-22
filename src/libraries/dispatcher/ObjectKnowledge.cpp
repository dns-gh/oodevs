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
#include "ClientPublisher_ABC.h"
#include "Model.h"
#include "NBCObjectAttribute.h"
#include "LogisticRouteObjectAttribute.h"
#include "CrossingSiteObjectAttribute.h"
#include "RotaObjectAttribute.h"
#include "CampObjectAttribute.h"
#include "MineJamObjectAttribute.h"
#include "LinearMinedAreaObjectAttribute.h"
#include "DispersedMinedAreaObjectAttribute.h"
#include "ModelVisitor_ABC.h"
#include "Side.h"
#include "Automat.h"
#include "Object.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Model& model, const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
    : SimpleEntity< kernel::ObjectKnowledge_ABC >( asnMsg.oid )
    , model_                        ( model )
    , team_                         ( model.sides_.Get( asnMsg.team ) )
    , pObject_                      ( model.objects_.Find( asnMsg.real_object ) )
    , nType_                        ( asnMsg.type )
    , nObstacleType_                ( asnMsg.m.obstacle_typePresent ? asnMsg.obstacle_type : EnumObstacleType::initial )
    , nTypeDotationForConstruction_ ( asnMsg.m.construction_dotation_typePresent ? asnMsg.construction_dotation_type : std::numeric_limits< unsigned int >::max() )
    , nTypeDotationForMining_       ( asnMsg.m.mining_dotation_typePresent ? asnMsg.mining_dotation_type : std::numeric_limits< unsigned int >::max() )
    , pAttributes_                  ( 0 )
    , nRelevance_                   ( std::numeric_limits< unsigned int >::max() )
    , nConstructionPercentage_      ( std::numeric_limits< unsigned int >::max() )
    , nMiningPercentage_            ( std::numeric_limits< unsigned int >::max() )
    , nBypassingPercentage_         ( std::numeric_limits< unsigned int >::max() )
    , bReservedObstacleActivated_   ( asnMsg.m.reserved_obstacle_activatedPresent ? asnMsg.reserved_obstacle_activated : false )
    , bPerceived_                   ( false )
    , nNbrDotationForConstruction_  ( std::numeric_limits< unsigned int >::max() )
    , nNbrDotationForMining_        ( std::numeric_limits< unsigned int >::max() )
{
    //$$ BULLSHIT // $$$$ AGE 2007-05-11: Clair !
    optionals_.relevancePresent                         = 0;
    optionals_.locationPresent                          = 0;
    optionals_.construction_percentagePresent           = 0;
    optionals_.mining_percentagePresent                 = 0;
    optionals_.bypass_construction_percentagePresent    = 0;
    optionals_.reserved_obstacle_activatedPresent       = asnMsg.m.reserved_obstacle_activatedPresent;
    optionals_.perceivedPresent                         = 0;
    optionals_.specific_attributesPresent               = 0;
    optionals_.automat_perceptionPresent                = 0;
    optionals_.construction_dotation_nbrPresent         = 0;
    optionals_.mining_dotation_nbrPresent               = 0;
    optionals_.obstacle_typePresent                     = asnMsg.m.obstacle_typePresent;
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
    bool realObjectChanged = ( message.real_object && ! pObject_ )
                          || ( pObject_ && pObject_->GetId() != message.real_object );
    if( realObjectChanged )
        pObject_ = model_.objects_.Find( message.real_object );

    ApplyUpdate( message );
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
    if( asnMsg.m.specific_attributesPresent)
    {
        if( !pAttributes_ )
        {
            switch( nType_ )
            {
                case EnumObjectType::camp_prisonniers:
                case EnumObjectType::camp_refugies:
                    pAttributes_ = new CampObjectAttribute( model_, asnMsg.specific_attributes );
                    break;
                case EnumObjectType::itineraire_logistique:
                    pAttributes_ = new LogisticRouteObjectAttribute( asnMsg.specific_attributes );
                    break;
                case EnumObjectType::nuage_nbc:
                case EnumObjectType::zone_nbc:
                    pAttributes_ = new NBCObjectAttribute( asnMsg.specific_attributes );
                    break;
                case EnumObjectType::rota:
                    pAttributes_ = new RotaObjectAttribute( asnMsg.specific_attributes );
                    break;
                case EnumObjectType::site_franchissement:
                    pAttributes_ = new CrossingSiteObjectAttribute( asnMsg.specific_attributes );
                    break;
                case EnumObjectType::bouchon_mines:
                    pAttributes_ = new MineJamObjectAttribute( asnMsg.specific_attributes );
                    break;
                case EnumObjectType::zone_minee_lineaire:
                    pAttributes_ = new LinearMinedAreaObjectAttribute( asnMsg.specific_attributes );
                    break;
                case EnumObjectType::zone_minee_par_dispersion:
                    pAttributes_ = new DispersedMinedAreaObjectAttribute( asnMsg.specific_attributes );
                    break;
                default:
                    pAttributes_ = 0;
            }
        }
        pAttributes_->Update( asnMsg.specific_attributes );
        optionals_.specific_attributesPresent = 1;
    }

    if( asnMsg.m.locationPresent )
    {
        localisation_.Update( asnMsg.location );
        optionals_.locationPresent = 1;
    }

    if( asnMsg.m.automat_perceptionPresent )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( unsigned int i = 0; i < asnMsg.automat_perception.n; ++i )
            automatPerceptions_.push_back( &model_.automats_.Get( asnMsg.automat_perception.elem[ i ]) );
    }

    if( asnMsg.m.real_objectPresent )
        pObject_ = model_.objects_.Find( asnMsg.real_object );

    UPDATE_ASN_ATTRIBUTE( relevance                     , nRelevance_                   );
    UPDATE_ASN_ATTRIBUTE( construction_percentage       , nConstructionPercentage_      );
    UPDATE_ASN_ATTRIBUTE( mining_percentage             , nMiningPercentage_            );
    UPDATE_ASN_ATTRIBUTE( bypass_construction_percentage, nBypassingPercentage_         );
    UPDATE_ASN_ATTRIBUTE( perceived                     , bPerceived_                   );
    UPDATE_ASN_ATTRIBUTE( construction_dotation_nbr     , nNbrDotationForConstruction_  );
    UPDATE_ASN_ATTRIBUTE( mining_dotation_nbr           , nNbrDotationForMining_        );
    UPDATE_ASN_ATTRIBUTE( reserved_obstacle_activated   , bReservedObstacleActivated_   );
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
void ObjectKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeCreation asn;

    asn().oid         = GetId();
    asn().team        = team_.GetId();
    asn().real_object = pObject_ ? pObject_->GetId() : 0;
    asn().type        = nType_;

    if( nTypeDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.construction_dotation_typePresent = 1;
        asn().construction_dotation_type = nTypeDotationForConstruction_;
    }

    if( nTypeDotationForMining_ != std::numeric_limits< unsigned int >::max() )
    {
        asn().m.mining_dotation_typePresent = 1;
        asn().mining_dotation_type = nTypeDotationForMining_;
    }

    SEND_ASN_ATTRIBUTE( obstacle_type              , nObstacleType_              );
    SEND_ASN_ATTRIBUTE( reserved_obstacle_activated, bReservedObstacleActivated_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeUpdate asn;

    asn().oid  = GetId();
    asn().team = team_.GetId();

    asn().m.real_objectPresent = 1;
    asn().real_object          = pObject_ ? pObject_->GetId() : 0;

    if( optionals_.specific_attributesPresent && pAttributes_ )
    {
        asn().m.specific_attributesPresent = 1;
        pAttributes_->Send( asn().specific_attributes );
    }

    if( optionals_.locationPresent )
    {
        asn().m.locationPresent = 1;
        localisation_.Send( asn().location );
    }

    if( optionals_.automat_perceptionPresent )
    {
        asn().m.automat_perceptionPresent = 1;
        asn().automat_perception.n = automatPerceptions_.size();
        asn().automat_perception.elem = asn().automat_perception.n > 0 ? new ASN1T_OID[ asn().automat_perception.n ] : 0;
        unsigned int i = 0;
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it, ++i )
            asn().automat_perception.elem[i] = (*it)->GetId();
    }

    SEND_ASN_ATTRIBUTE( relevance                     , nRelevance_                   );
    SEND_ASN_ATTRIBUTE( construction_percentage       , nConstructionPercentage_      );
    SEND_ASN_ATTRIBUTE( mining_percentage             , nMiningPercentage_            );
    SEND_ASN_ATTRIBUTE( bypass_construction_percentage, nBypassingPercentage_         );
    SEND_ASN_ATTRIBUTE( perceived                     , bPerceived_                   );
    SEND_ASN_ATTRIBUTE( construction_dotation_nbr     , nNbrDotationForConstruction_  );
    SEND_ASN_ATTRIBUTE( mining_dotation_nbr           , nNbrDotationForMining_        );
    SEND_ASN_ATTRIBUTE( reserved_obstacle_activated   , bReservedObstacleActivated_   );

    asn.Send( publisher );

    if( asn().m.specific_attributesPresent && pAttributes_ )
        pAttributes_->AsnDelete( asn().specific_attributes );

    if( asn().m.automat_perceptionPresent && asn().automat_perception.n > 0 )
        delete [] asn().automat_perception.elem;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeDestruction asn;
    asn().oid  = GetId();
    asn().team = team_.GetId();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetRecognizedEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ObjectKnowledge::GetRecognizedEntity() const
{
    return pObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Object_ABC* ObjectKnowledge::GetEntity() const
{
    return pObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetOwner
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Team_ABC& ObjectKnowledge::GetOwner() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Display
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( kernel::Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( kernel::Displayer_ABC& ) const
{
    // NOTHING
}
