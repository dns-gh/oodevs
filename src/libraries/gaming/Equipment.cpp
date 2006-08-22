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
std::string Equipment::GetName() const
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
}

// -----------------------------------------------------------------------------
// Name: Equipment::Total
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
unsigned Equipment::Total() const
{
    return available_ + unavailable_ + repairable_ + inMaintenance_;
}
