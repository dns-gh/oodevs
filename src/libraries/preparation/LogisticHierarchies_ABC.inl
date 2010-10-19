// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Dotation.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "DotationsItem.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC constructor
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
LogisticHierarchies_ABC< I >::LogisticHierarchies_ABC( kernel::Controller& controller, kernel::Entity_ABC& entity )
    : kernel::EntityHierarchies< I >( controller, entity, 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC destructor
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
LogisticHierarchies_ABC< I >::~LogisticHierarchies_ABC()
{
    // NOTHING
}

