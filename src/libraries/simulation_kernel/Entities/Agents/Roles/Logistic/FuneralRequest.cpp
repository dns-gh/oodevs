// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FuneralRequest.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "protocol/ClientSenders.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: FuneralRequest constructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralRequest::FuneralRequest( Human_ABC& human )
    : human_( human )
{
}

// -----------------------------------------------------------------------------
// Name: FuneralRequest destructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralRequest::~FuneralRequest()
{
}

// =============================================================================
// Main
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralRequest::GetLogisticHierarchy
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
const LogisticHierarchy_ABC& FuneralRequest::GetLogisticHierarchy() const
{
    return human_.GetPion().GetLogisticHierarchy();
}

// -----------------------------------------------------------------------------
// Name: FuneralRequest::GetLogisticHierarchy
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
const MT_Vector2D& FuneralRequest::GetPosition() const
{
    return human_.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// =============================================================================
// Event
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralRequest::GetRank
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralRequest::OnHandledByFuneral()
{
    human_.NotifyHandledByFuneral();
}

// -----------------------------------------------------------------------------
// Name: FuneralRequest::GetRank
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralRequest::OnBackFromFuneral()
{
    human_.NotifyBackFromFuneral();
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralRequest::GetRank
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
sword::EnumHumanRank FuneralRequest::GetRank() const
{
    return human_.GetRank().GetAsnID();
}

// -----------------------------------------------------------------------------
// Name: FuneralRequest::Serialize
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralRequest::Serialize( sword::UnitId& unitId ) const
{
    unitId.set_id( human_.GetPion().GetID() );
}