// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "protocol/Protocol.h"
#include "Equipment.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Equipment::Equipment( const sword::EquipmentDotations_EquipmentDotation& message )
   : nEquipmentType_   ( message.type().id() )
   , nNbrAvailable_    ( message.available() )
   , nNbrUnavailable_  ( message.unavailable() )
   , nNbrRepairable_   ( message.repairable() )
   , nNbrInMaintenance_( message.repairing() )
   , nNbrPrisoner_     ( message.captured() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Equipment::~Equipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Equipment::Update( const sword::EquipmentDotations_EquipmentDotation& message )
{
   nNbrAvailable_     = message.available();
   nNbrUnavailable_   = message.unavailable();
   nNbrRepairable_    = message.repairable();
   nNbrInMaintenance_ = message.repairing();
   nNbrPrisoner_      = message.captured();
}

// -----------------------------------------------------------------------------
// Name: Equipment::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Equipment::Send( sword::EquipmentDotations_EquipmentDotation& message ) const
{
    message.mutable_type()->set_id( nEquipmentType_ );
    message.set_available( nNbrAvailable_ );
    message.set_unavailable( nNbrUnavailable_ );
    message.set_repairable( nNbrRepairable_ );
    message.set_repairing( nNbrInMaintenance_ );
    message.set_captured( nNbrPrisoner_ );
}
