// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Equipment.h"
#include "clients_kernel/EquipmentType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Equipment::Equipment( const EquipmentType& type )
    : type_( type )
    , available_( 0 )
    , unavailable_( 0 )
    , repairable_( 0 )
    , inMaintenance_( 0 )
    , prisonners_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment::Equipment( const Equipment& rhs )
    : type_( rhs.type_ )
    , available_( rhs.available_ )
    , unavailable_( rhs.unavailable_ )
    , repairable_( rhs.repairable_ )
    , inMaintenance_( rhs.inMaintenance_ )
    , prisonners_( rhs.prisonners_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Equipment::~Equipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment::GetName
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
QString Equipment::GetName() const
{
    return type_.GetName();
}

// -----------------------------------------------------------------------------
// Name: Equipment::Update
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void Equipment::Update( const ASN1T_DotationEquipement& message )
{
    available_     = message.nb_disponibles;
    unavailable_   = message.nb_indisponibles;
    repairable_    = message.nb_reparables;
    inMaintenance_ = message.nb_dans_chaine_maintenance;
    prisonners_    = message.nb_prisonniers;
}

// -----------------------------------------------------------------------------
// Name: Equipment::Total
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
unsigned Equipment::Total() const
{
    return available_ + unavailable_ + repairable_ + inMaintenance_ + prisonners_;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator+
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment Equipment::operator+( const Equipment& rhs )
{
    Equipment equipment( *this );
    equipment.available_     += rhs.available_;
    equipment.unavailable_   += rhs.unavailable_;
    equipment.repairable_    += rhs.repairable_;
    equipment.inMaintenance_ += rhs.inMaintenance_;
    equipment.prisonners_    += rhs.prisonners_;
    return equipment;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator-
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment Equipment::operator-( const Equipment& rhs )
{
    Equipment equipment( *this );
    equipment.available_     -= rhs.available_;
    equipment.unavailable_   -= rhs.unavailable_;
    equipment.repairable_    -= rhs.repairable_;
    equipment.inMaintenance_ -= rhs.inMaintenance_;
    equipment.prisonners_    -= rhs.prisonners_;
    return equipment;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator=
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment& Equipment::operator=( const Equipment& rhs )
{
    available_     = rhs.available_;
    unavailable_   = rhs.unavailable_;
    repairable_    = rhs.repairable_;
    inMaintenance_ = rhs.inMaintenance_;
    prisonners_    = rhs.prisonners_;
    return *this;
}
