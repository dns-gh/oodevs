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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleHLA_NBC.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 14:47 $
// $Revision: 6 $
// $Workfile: PHY_RoleHLA_NBC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_RoleHLA_NBC.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_NBC constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_NBC::PHY_RoleHLA_NBC( MT_RoleContainer& role )
    : PHY_RoleInterface_NBC( role )
    , bContaminated_( false )
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
// Name: PHY_RoleHLA_NBC::Contaminate
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_NBC::Contaminate( const MIL_NbcAgentType& /*nbcAgentType*/ )
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
