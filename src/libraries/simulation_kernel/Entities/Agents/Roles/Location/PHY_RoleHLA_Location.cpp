// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RoleHLA_Location.cpp $
// $Author: Mco $
// $Modtime: 11/02/05 16:52 $
// $Revision: 12 $
// $Workfile: PHY_RoleHLA_Location.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Location.h"
#include "Tools/MIL_Tools.h"
#include "Hla/HLA_Serialization.h"
#include "Hla/AttributeIdentifier.h"
#include "Entities/Agents/MIL_AgentHLA.h"
#include "Entities/Objects/MIL_Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Location::PHY_RoleHLA_Location( MT_RoleContainer& role, MIL_AgentHLA& pion )
    : PHY_RoleInterface_Location( role )
    , pion_         ( pion )
    , vDirection_   ()
    , vPosition_    ()
    , rHeight_      ( 0 )
    , rCurrentSpeed_( 0 )
{
    UpdatePatch();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Location::~PHY_RoleHLA_Location()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyObjectCollision
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyObjectCollision( MIL_RealObject_ABC& /*object*/ )
{
    // NOTHING (used for knowledge)
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyPopulationCollision( MIL_PopulationFlow& population )
{
    // NOTHING ?
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyPopulationCollision( MIL_PopulationConcentration& population )
{
    // NOTHING ?
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyMovingInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingInside( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyMovingOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingOutside( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyPutInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyPutInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutInside( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyPutOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyPutOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutOutside( pion_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::Deserialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "position" )
    {
        static std::string strPos;
        deserializer >> strPos;
        MIL_Tools::ConvertCoordMosToSim( strPos, vPosition_ );
        TER_World::GetWorld().ClipPointInsideWorld( vPosition_ );
        if( IsValid() && pion_.IsValid() )
            UpdatePatch();
    }
    else if( attributeID == "hauteur" )
        deserializer >> rHeight_;
    else if( attributeID == "direction" )
        deserializer >> vDirection_;
    else if( attributeID == "vitesse" )
        deserializer >> rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetHeight
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Location::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetAltitude
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Location::GetAltitude() const
{
    return MIL_Tools::GetAltitude( vPosition_ ) + rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetPosition
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleHLA_Location::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetDirection
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleHLA_Location::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetAgent
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MIL_Agent_ABC& PHY_RoleHLA_Location::GetAgent() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetCurrentSpeed
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Location::GetCurrentSpeed() const
{
    return rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::HasDoneMagicMove
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Location::HasDoneMagicMove() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::IsValid
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Location::IsValid() const
{
    return ! vPosition_.IsZero();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetCost
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Location::GetCost( const MT_Vector2D&, const MT_Vector2D&, const TerrainData&, const TerrainData&, void* ) const
{
    throw std::exception( __FUNCTION__ " should not be called on distant agents" );
}
