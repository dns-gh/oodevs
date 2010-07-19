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
#include "Network/NET_AsnException.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_Objective constructor
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
DEC_Objective::DEC_Objective( const Common::MsgMissionObjective& asn )
    : bFlag_( false )
{
    NET_ASN_Tools::ReadTick( asn.horaire(), nSchedule_);
    if( !NET_ASN_Tools::ReadLocation( asn.localisation(), localisation_ ) )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
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
}

// -----------------------------------------------------------------------------
// Name: DEC_Objective::Serialize
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_Objective::Serialize( Common::MsgMissionObjective& asn ) const
{
    NET_ASN_Tools::WriteTick( nSchedule_, *asn.mutable_horaire() );
    NET_ASN_Tools::WriteLocation( localisation_, *asn.mutable_localisation() );
}
