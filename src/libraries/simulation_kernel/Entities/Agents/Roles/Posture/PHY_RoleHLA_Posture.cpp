// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Posture.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"

#include <hla/AttributeIdentifier.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Posture::PHY_RoleHLA_Posture( MT_RoleContainer& role, const MIL_Agent_ABC& pion )
    : PHY_RoleInterface_Posture( role )
    , pPosture_( & PHY_Posture::mouvement_ )
    , pion_( pion )
    , bIsStealth_( false )
    , bTransported_( false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Posture::~PHY_RoleHLA_Posture()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::GetLastPosture
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& PHY_RoleHLA_Posture::GetLastPosture() const
{
    assert( pPosture_ );
    return *pPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::GetCurrentPosture
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& PHY_RoleHLA_Posture::GetCurrentPosture() const
{
    assert( pPosture_ );
    return *pPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::GetPostureCompletionPercentage
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Posture::GetPostureCompletionPercentage() const
{
    return 1;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::CanBePerceived
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Posture::CanBePerceived( const MIL_AgentPion& perceiver ) const
{
    return ( !bIsStealth_ || perceiver.GetRole< PHY_RolePion_Perceiver >().WasPerceived( pion_ ) )
        && ! bTransported_; 
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::SetPosturePostePrepareGenie
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Posture::SetPosturePostePrepareGenie()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::UnsetPosturePostePrepareGenie
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Posture::UnsetPosturePostePrepareGenie()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::SetTimingFactor
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Posture::SetTimingFactor( MT_Float )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::Deserialize
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Posture::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "posture" )
    {
        std::string strPosture;
        deserializer >> strPosture;
        PHY_Posture const * const pPosture = PHY_Posture::FindPosture( strPosture );
        if( pPosture )
            pPosture_ = pPosture;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Posture::ChangeStatus
// Created: AGE 2004-12-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Posture::ChangeStatus( const std::vector< std::string >& statuses )
{
    bIsStealth_   = std::find( statuses.begin(), statuses.end(), "furtif" ) != statuses.end();
    bTransported_ = std::find( statuses.begin(), statuses.end(), "transporte" ) != statuses.end();
}
