// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentExtension.h"
#include "Spatial.h"
#include "EntityType.h"
#include "AggregateMarking.h"
#include "SerializationTools.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include "hla/UpdateFunctor_ABC.h"
#include "hla/AttributeIdentifier.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder, const EntityIdentifier& id )
    : holder_            ( holder )
    , id_                ( id )
    , spatialChanged_    ( true )
    , compositionChanged_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AgentExtension::Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const
{
    if( bUpdateAll )
        UpdateEntityType( functor );
    if( bUpdateAll )
        UpdateEntityIdentifier( functor );
    if( bUpdateAll || spatialChanged_ )
        UpdateSpatial( functor );
    if( bUpdateAll )
        UpdateAggregateMarking( functor );
    if( bUpdateAll )
        UpdateAggregateState( functor );
    if( bUpdateAll )
        UpdateForceIdentifier( functor );
    formation_.Serialize( functor, bUpdateAll );
    if( bUpdateAll || compositionChanged_ )
        UpdateComposition( functor );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitAttributes& attributes )
{
    spatialChanged_     = spatialChanged_     || attributes.m.positionPresent
                                              || attributes.m.hauteurPresent
                                              || attributes.m.vitessePresent
                                              || attributes.m.directionPresent;
    compositionChanged_ = compositionChanged_ || attributes.m.dotation_eff_materielPresent;
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitEnvironmentType& attributes )
{
    formation_.Update( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateEntityType
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateEntityType( UpdateFunctor_ABC& functor ) const
{
    EntityType type;
    type.SetKind( EntityType::platform );
    type.SetDomain( EntityType::land );
    type.SetCountry( EntityType::france );
    type.SetCategory( EntityType::tank ); // $$$$ AGE 2008-02-25: 

    Serializer serializer;
    type.Serialize( serializer );
    functor.Visit( AttributeIdentifier( "EntityType" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateEntityIdentifier
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateEntityIdentifier( UpdateFunctor_ABC& functor ) const
{
    Serializer serializer;
    id_.Serialize( serializer );
    functor.Visit( AttributeIdentifier( "EntityIdentifier" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateSpatial
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateSpatial( UpdateFunctor_ABC& functor ) const
{
    Spatial spatial( holder_.position_, (float)holder_.nHeight_, (float)holder_.nSpeed_, (unsigned short)holder_.nDirection_ );
    Serializer archive;
    spatial.Serialize( archive );
    functor.Visit( AttributeIdentifier( "Spatial" ), archive );
    spatialChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateAggregateMarking
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateAggregateMarking( UpdateFunctor_ABC& functor ) const
{
    AggregateMarking marking( holder_.strName_ );
    Serializer archive;
    marking.Serialize( archive );
    functor.Visit( AttributeIdentifier( "AggregateMarking" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateAggregateState
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateAggregateState( UpdateFunctor_ABC& functor ) const
{
    unsigned char state = 1; // fully aggregated
    Serializer archive;
    archive << state;
    functor.Visit( AttributeIdentifier( "AggregateState" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateForceIdentifier
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateForceIdentifier( UpdateFunctor_ABC& functor ) const
{
    unsigned char force = 0; // Other
    const dispatcher::Side& side = holder_.pAutomat_->side_;
    switch( side.nType_ )
    {
    case EnumDiplomacy::ami:    force = 1; break;
    case EnumDiplomacy::ennemi: force = 2; break;
    case EnumDiplomacy::neutre: force = 3; break;
    }

    Serializer archive;
    archive << force;
    functor.Visit( AttributeIdentifier( "ForceIdentifier" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateComposition
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateComposition( UpdateFunctor_ABC& functor ) const
{

}
