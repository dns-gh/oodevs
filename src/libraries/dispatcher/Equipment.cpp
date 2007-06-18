// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Equipment.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Equipment::Equipment( const Model& /*model*/, const ASN1T_EquipmentDotations& asnMsg )
   : nEquipmentType_   ( asnMsg.type_equipement )
   , nNbrAvailable_    ( asnMsg.nb_disponibles )
   , nNbrUnavailable_  ( asnMsg.nb_indisponibles )
   , nNbrRepairable_   ( asnMsg.nb_reparables )
   , nNbrInMaintenance_( asnMsg.nb_dans_chaine_maintenance )
   , nNbrPrisoner_     ( asnMsg.nb_prisonniers )
{
}

// -----------------------------------------------------------------------------
// Name: Equipment destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Equipment::~Equipment()
{

}
      
// -----------------------------------------------------------------------------
// Name: Equipment::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Equipment::Update( const ASN1T_EquipmentDotations& asnMsg )
{
   nNbrAvailable_     = asnMsg.nb_disponibles;
   nNbrUnavailable_   = asnMsg.nb_indisponibles;
   nNbrRepairable_    = asnMsg.nb_reparables;
   nNbrInMaintenance_ = asnMsg.nb_dans_chaine_maintenance;
   nNbrPrisoner_      = asnMsg.nb_prisonniers;
}

// -----------------------------------------------------------------------------
// Name: Equipment::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Equipment::Send( ASN1T_EquipmentDotations& asnMsg ) const
{
    asnMsg.type_equipement            = nEquipmentType_;
    asnMsg.nb_disponibles             = nNbrAvailable_;
    asnMsg.nb_indisponibles           = nNbrUnavailable_;
    asnMsg.nb_reparables              = nNbrRepairable_;
    asnMsg.nb_dans_chaine_maintenance = nNbrInMaintenance_;
    asnMsg.nb_prisonniers             = nNbrPrisoner_;
}
