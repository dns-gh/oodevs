// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_EquipmentType.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_EquipmentType::MissionParameter_EquipmentType( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , equipmentType_      ( asn.value().equipmenttype().equipment() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_EquipmentType::~MissionParameter_EquipmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_EquipmentType::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_equipmenttype()->set_equipment( equipmentType_ );
}
