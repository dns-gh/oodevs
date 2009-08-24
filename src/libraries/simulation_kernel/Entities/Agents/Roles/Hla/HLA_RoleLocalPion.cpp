// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_RoleLocalPion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Hla/HLA_UpdateFunctor.h"
#include <hla/ObjectIdentifier.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion constructor
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
HLA_RoleLocalPion::HLA_RoleLocalPion( MIL_AgentPion& pion )
    : HLA_RoleInterface( pion )
    , pObjectId_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion destructor
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
HLA_RoleLocalPion::~HLA_RoleLocalPion()
{
    delete pObjectId_;
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion::Serialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void HLA_RoleLocalPion::Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const
{
    assert( pAgent_ );
    HLA_UpdateFunctor updateFunctor( functor, bUpdateAll );
    dynamic_cast< MIL_AgentPion* > ( pAgent_ )->Serialize( updateFunctor );
    pAgent_->GetRole< PHY_RoleInterface_Composantes >().Serialize( updateFunctor );
    pAgent_->GetRole< PHY_RoleInterface_Location    >().Serialize( updateFunctor );
    pAgent_->GetRole< PHY_RoleInterface_Posture     >().Serialize( updateFunctor );
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion::Deserialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void HLA_RoleLocalPion::Deserialize( const AttributeIdentifier&, const Deserializer& )
{
    throw std::exception( "local units should not be deserialized" );
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion::SetId
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
void HLA_RoleLocalPion::SetId( const ObjectIdentifier& objectId )
{
    if( pObjectId_ )
        throw std::exception( "object id already set" );
    pObjectId_ = new ObjectIdentifier( objectId );
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion::GetId
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
ObjectIdentifier HLA_RoleLocalPion::GetId() const
{
    if( ! pObjectId_ )
        throw std::exception( "object id not set" );
    return *pObjectId_;
}
