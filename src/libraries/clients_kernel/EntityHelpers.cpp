// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EntityHelpers.h"
#include "Entity_ABC.h"
#include "Automat_ABC.h"
#include "Formation_ABC.h"
#include "LogisticLevel.h"

using namespace kernel;

namespace kernel
{
namespace EntityHelpers
{

// -----------------------------------------------------------------------------
// Name: IsLogisticBase
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
bool IsLogisticBase( const Entity_ABC& entity )
{
    const Automat_ABC* pAutomat = dynamic_cast< const Automat_ABC* >( &entity );
    if( pAutomat && pAutomat->GetLogisticLevel() == LogisticLevel::logistic_base_ )
        return true;
    const Formation_ABC* pFormation = dynamic_cast< const Formation_ABC* >( &entity );
    if( pFormation && pFormation->GetLogisticLevel() == LogisticLevel::logistic_base_ )
        return true;
    return false;
}

}
}