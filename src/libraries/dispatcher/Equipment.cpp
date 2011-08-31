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
   , nNbrAvailable_    ( 0 )
   , nNbrUnavailable_  ( 0 )
   , nNbrRepairable_   ( 0 )
   , nNbrOnSiteFixable_( 0 )
   , nNbrInMaintenance_( 0 )
   , nNbrPrisoner_     ( 0 )
{
    Update( message );
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
    nNbrOnSiteFixable_ = message.on_site_fixable();
    nNbrInMaintenance_ = message.repairing();
    nNbrPrisoner_      = message.captured();
    vBreakdowns_.clear();
    for( int i = 0; i < message.breakdowns_size(); ++i )
        vBreakdowns_.push_back( message.breakdowns( i ) );
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
    message.set_on_site_fixable( nNbrOnSiteFixable_ );
    message.set_repairing( nNbrInMaintenance_ );
    message.set_captured( nNbrPrisoner_ );
    for( unsigned int i = 0; i < vBreakdowns_.size(); ++i )
        message.add_breakdowns( vBreakdowns_[ i ] );
}
