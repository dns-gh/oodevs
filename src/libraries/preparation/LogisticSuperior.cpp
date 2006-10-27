// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticSuperior.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticSuperior constructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
LogisticSuperior::LogisticSuperior( const kernel::Entity_ABC& entity )
    : entity_( entity )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LogisticSuperior destructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
LogisticSuperior::~LogisticSuperior()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsValidSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsValidSuperior( const TC2& superior ) const
{
    const kernel::Automat_ABC* automat = (const Automat_ABC*)superior;
    return IsSameTeam( *automat ) && automat->GetType().IsTC2();
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsValidSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsValidSuperior( const MaintenanceSuperior& superior ) const
{
    const kernel::Automat_ABC* automat = (const Automat_ABC*)superior;
    return IsSameTeam( *automat ) && automat->GetType().IsLogisticMaintenance();
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsValidSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsValidSuperior( const MedicalSuperior& superior ) const
{
    const kernel::Automat_ABC* automat = (const Automat_ABC*)superior;
    return IsSameTeam( *automat ) && automat->GetType().IsLogisticMedical();
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsValidSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsValidSuperior( const SupplySuperior& superior ) const
{
    const kernel::Automat_ABC* automat = (const Automat_ABC*)superior;
    return IsSameTeam( *automat ) && automat->GetType().IsLogisticSupply();
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsSameTeam
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsSameTeam( const kernel::Automat_ABC& automat ) const
{
    return &entity_.Get< kernel::TacticalHierarchies >().GetTop() == &automat.Get< kernel::TacticalHierarchies >().GetTop();
}
