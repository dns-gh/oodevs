// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Lima.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, tools::IdManager& idManager )
    : TacticalLine_ABC( controller, tools::translate( "Lima", "Phase line" ), idManager )
    , controller_     ( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
Lima::Lima( kernel::Controller& controller, xml::xistream& xis, tools::IdManager& idManager )
    : TacticalLine_ABC( controller, xis, idManager )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Lima::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Lima::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Lima* > first;
    std::vector< const kernel::TacticalLine_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Lima* >( elements[ i ] ) );
        second.push_back( static_cast< const kernel::TacticalLine_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second, third );
}


// -----------------------------------------------------------------------------
// Name: Lima::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Lima::IsLimit() const
{
    return false;
}
