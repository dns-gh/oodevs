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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Hla/HLA_RoleDistantPion.cpp $
// $Author: Age $
// $Modtime: 29/11/04 10:57 $
// $Revision: 7 $
// $Workfile: HLA_RoleDistantPion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_RoleDistantPion.h"

#include "Entities/Agents/MIL_AgentHLA.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleHLA_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleHLA_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleHLA_Posture.h"

// -----------------------------------------------------------------------------
// Name: HLA_RoleDistantPion constructor
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
HLA_RoleDistantPion::HLA_RoleDistantPion( MT_RoleContainer& roleContainer, MIL_AgentHLA& pion, const ObjectIdentifier& objectId )
    : HLA_RoleInterface( roleContainer )
    , pion_( pion )
    , objectId_( objectId )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleDistantPion destructor
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
HLA_RoleDistantPion::~HLA_RoleDistantPion()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleDistantPion::Serialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void HLA_RoleDistantPion::Serialize( UpdateFunctor_ABC&, bool ) const
{
    throw std::exception( "distant units should not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleDistantPion::Deserialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void HLA_RoleDistantPion::Deserialize( const AttributeIdentifier& attributeID, const Deserializer& deserializer )
{
    pion_.Deserialize( attributeID, deserializer );
    GetRole< PHY_RoleHLA_Location    >().Deserialize( attributeID, deserializer );
    GetRole< PHY_RoleHLA_Composantes >().Deserialize( attributeID, deserializer );
    GetRole< PHY_RoleHLA_Posture     >().Deserialize( attributeID, deserializer );
}

// -----------------------------------------------------------------------------
// Name: HLA_RoleDistantPion::GetId
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
ObjectIdentifier HLA_RoleDistantPion::GetId() const
{
    return objectId_;
}
