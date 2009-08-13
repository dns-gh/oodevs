// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_NBC.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_NBC::PHY_RoleHLA_NBC()
    : bContaminated_( false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_NBC::~PHY_RoleHLA_NBC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC::IsContaminated
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_NBC::IsContaminated() const
{
    return bContaminated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC::Poison
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RoleHLA_NBC::Poison( const MIL_ToxicEffectManipulator& /*nbcAgent*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC::Contaminate
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_NBC::Contaminate( const MIL_ToxicEffectManipulator& /*nbcAgent*/ )
{
     // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC::Decontaminate
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_NBC::Decontaminate()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC::Decontaminate
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_NBC::Decontaminate( MT_Float /*rRatioAgentsWorking*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC::ChangeStatus
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void PHY_RoleHLA_NBC::ChangeStatus( const std::vector< std::string >& statuses )
{
    bContaminated_ = std::find( statuses.begin(), statuses.end(), "contamine" ) != statuses.end();
}
