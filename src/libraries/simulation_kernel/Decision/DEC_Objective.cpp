// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Objective.h"
#include "Network/NET_ASN_Tools.h"
#include "tools/NET_AsnException.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_Objective constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Objective::DEC_Objective( const sword::MissionObjective& asn )
    : localisation_()
    , bFlag_       ( false )
{
    NET_ASN_Tools::ReadTick( asn.time(), nSchedule_);
    if( !NET_ASN_Tools::ReadLocation( asn.location(), localisation_ ) )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective destructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Objective::~DEC_Objective()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::operator=
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Objective::operator=( const DEC_Objective& rhs )
{
    localisation_ = rhs.localisation_;
    nSchedule_ = rhs.nSchedule_;
    bFlag_ = rhs.bFlag_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::Serialize
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Objective::Serialize( sword::MissionObjective& asn ) const
{
    NET_ASN_Tools::WriteTick    ( nSchedule_, *asn.mutable_time() );
    NET_ASN_Tools::WriteLocation( localisation_, *asn.mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::ComputerBarycenter
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
MT_Vector2D DEC_Objective::ComputerBarycenter() const
{
    return localisation_.ComputeBarycenter();
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::GetSchedule
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
unsigned int DEC_Objective::GetSchedule() const
{
    return nSchedule_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::IsFlagged
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
bool DEC_Objective::IsFlagged() const
{
    return bFlag_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::Flag
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Objective::Flag( bool bValue )
{
    bFlag_ = bValue;
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::GetLocalisation
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_Objective::GetLocalisation() const
{
    return localisation_;
}