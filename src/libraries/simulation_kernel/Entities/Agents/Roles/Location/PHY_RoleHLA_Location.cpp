// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Location.h"
#include "Tools/MIL_Tools.h"
#include "Hla/HLA_Serialization.h"
#include "Hla/AttributeIdentifier.h"
#include "Entities/Agents/MIL_AgentHLA.h"
#include "Entities/Objects/MIL_Object_ABC.h"

using namespace hla;


// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Location::PHY_RoleHLA_Location( MIL_AgentHLA& pion )
    : pion_         ( pion )
    , vDirection_   ()
    , pvPosition_   ( new MT_Vector2D() )
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
void PHY_RoleHLA_Location::NotifyTerrainObjectCollision( MIL_Object_ABC& /*object*/ )
{
    // NOTHING (used for knowledge)
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyTerrainPutInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyTerrainPutInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutInside( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::NotifyTerrainPutOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Location::NotifyTerrainPutOutsideObject( MIL_Object_ABC& object )
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
        MIL_Tools::ConvertCoordMosToSim( strPos, *pvPosition_ );
        TER_World::GetWorld().ClipPointInsideWorld( *pvPosition_ );
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
    return MIL_Tools::GetAltitude( *pvPosition_ ) + rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetPosition
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleHLA_Location::GetPosition() const
{
    return *pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MT_Vector2D> PHY_RoleHLA_Location::GetSharedPosition
// Created: LDC 2009-11-17
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> PHY_RoleHLA_Location::GetSharedPosition() const
{
    return pvPosition_;
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
    return ! pvPosition_->IsZero();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Location::GetCost
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Location::GetCost( const MT_Vector2D&, const MT_Vector2D&, const TerrainData&, const TerrainData&, void* ) const
{
    throw std::exception( __FUNCTION__ " should not be called on distant agents" );
}
