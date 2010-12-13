// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "protocol/protocol.h"
#include "Equipment.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Equipment::Equipment( const sword::EquipmentDotations_EquipmentDotation& message )
   : nEquipmentType_   ( message.type().id() )
   , nNbrAvailable_    ( message.nb_disponibles() )
   , nNbrUnavailable_  ( message.nb_indisponibles() )
   , nNbrRepairable_   ( message.nb_reparables() )
   , nNbrInMaintenance_( message.nb_dans_chaine_maintenance() )
   , nNbrPrisoner_     ( message.nb_prisonniers() )
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
   nNbrAvailable_     = message.nb_disponibles();
   nNbrUnavailable_   = message.nb_indisponibles();
   nNbrRepairable_    = message.nb_reparables();
   nNbrInMaintenance_ = message.nb_dans_chaine_maintenance();
   nNbrPrisoner_      = message.nb_prisonniers();
}

// -----------------------------------------------------------------------------
// Name: Equipment::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Equipment::Send( sword::EquipmentDotations_EquipmentDotation& message ) const
{
    message.mutable_type()->set_id         ( nEquipmentType_ );
    message.set_nb_disponibles             ( nNbrAvailable_ );
    message.set_nb_indisponibles           ( nNbrUnavailable_ );
    message.set_nb_reparables              ( nNbrRepairable_ );
    message.set_nb_dans_chaine_maintenance ( nNbrInMaintenance_ );
    message.set_nb_prisonniers             ( nNbrPrisoner_ );
}
