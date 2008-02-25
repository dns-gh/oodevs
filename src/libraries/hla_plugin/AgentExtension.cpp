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
#include "dispatcher/Agent.h"
#include "SerializationTools.h"
#include "hla/UpdateFunctor_ABC.h"
#include "hla/AttributeIdentifier.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder )
    : holder_            ( holder )
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
    formation_.Serialize( functor, bUpdateAll );

    if( bUpdateAll || spatialChanged_ )
        UpdateSpatialData( functor );
    if( bUpdateAll || compositionChanged_ )
        UpdateComposition( functor );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateSpatialData
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateSpatialData( UpdateFunctor_ABC& functor ) const
{
    Spatial spatial( holder_.position_, (float)holder_.nHeight_, (float)holder_.nSpeed_, (unsigned short)holder_.nDirection_ );
    Serializer archive;
    spatial.Serialize( archive );
    functor.Visit( AttributeIdentifier( "Spatial" ), archive );
    spatialChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateComposition
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateComposition( UpdateFunctor_ABC& functor ) const
{
    // $$$$ AGE 2008-02-25: 
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
