// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Limit.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( Controller& controller, tools::IdManager& idManager )
    : TacticalLine_ABC( controller, tools::translate( "Limit", "Limit" ), idManager )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller, xml::xistream& xis, tools::IdManager& idManager )
    : TacticalLine_ABC( controller, xis, idManager )
    , controller_( controller )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Limit::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Limit::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Limit* > first;
    std::vector< const kernel::TacticalLine_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Limit* >( elements[ i ] ) );
        second.push_back( static_cast< const kernel::TacticalLine_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second, third );
}

// -----------------------------------------------------------------------------
// Name: Limit::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Limit::IsLimit() const
{
    return true;
}
