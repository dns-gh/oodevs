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
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/LogisticLevel.h"

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
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsValidSuperior( const LogisticBaseSuperior& /*superior*/ ) const
{
    //const kernel::Formation_ABC* formation = (const Formation_ABC*)superior;
    //return IsSameTeam( *formation ) && formation->GetLogisticLevel()==kernel::LogisticLevel::logistic_base_;
    assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsSameTeam
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsSameTeam( const kernel::Automat_ABC& automat ) const
{
    return &entity_.Get< kernel::TacticalHierarchies >().GetTop() == &automat.Get< kernel::TacticalHierarchies >().GetTop();
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperior::IsSameTeam
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
bool LogisticSuperior::IsSameTeam( const kernel::Formation_ABC& formation ) const
{
    return &entity_.Get< kernel::TacticalHierarchies >().GetTop() == &formation.Get< kernel::TacticalHierarchies >().GetTop();
}

