// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Hla/HLA_RoleLocalPion.cpp $
// $Author: Age $
// $Modtime: 29/11/04 17:59 $
// $Revision: 8 $
// $Workfile: HLA_RoleLocalPion.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_RoleLocalPion.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Hla/HLA_UpdateFunctor.h"
#include "hla/ObjectIdentifier.h"

// -----------------------------------------------------------------------------
// Name: HLA_RoleLocalPion constructor
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
HLA_RoleLocalPion::HLA_RoleLocalPion( MT_RoleContainer& roleContainer, MIL_AgentPion& pion )
    : HLA_RoleInterface( roleContainer )
    , pion_( pion )
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
    HLA_UpdateFunctor updateFunctor( functor, bUpdateAll );
    pion_.Serialize( updateFunctor );
    GetRole< PHY_RolePion_Composantes >().Serialize( updateFunctor );
    GetRole< PHY_RolePion_Location    >().Serialize( updateFunctor );
    GetRole< PHY_RolePion_Posture     >().Serialize( updateFunctor );
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
