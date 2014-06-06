// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PhysicalAttribute_ABC.h"
#include "Architecture_ABC.h"
#include "Usages_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC constructor
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
PhysicalAttribute_ABC::PhysicalAttribute_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC destructor
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
PhysicalAttribute_ABC::~PhysicalAttribute_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::GetArchitecture
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
Architecture_ABC& PhysicalAttribute_ABC::GetArchitecture() const
{
    assert( architecture_.get() );
    return *architecture_;
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::GetUsages
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
Usages_ABC& PhysicalAttribute_ABC::GetUsages() const
{
    assert( usages_.get() );
    return *usages_;
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::FindUsagesValue
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
unsigned int PhysicalAttribute_ABC::FindUsagesValue( const std::string& motivation ) const
{
    assert( usages_.get() );
    return usages_->Find( motivation );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::Display
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::Display( Displayer_ABC& displayer ) const
{
    assert( architecture_.get() );
    architecture_->Display( displayer );
    assert( usages_.get() );
    usages_->Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::DisplayInList
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::DisplayInList( Displayer_ABC& displayer ) const
{
    assert( architecture_.get() );
    architecture_->DisplayInList( displayer );
    assert( usages_.get() );
    usages_->DisplayInList( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::DisplayInTooltip
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    assert( architecture_.get() );
    architecture_->DisplayInTooltip( displayer );
    assert( usages_.get() );
    usages_->DisplayInTooltip( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::DisplayInSummary
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::DisplayInSummary( Displayer_ABC& displayer ) const
{
    assert( architecture_.get() );
    architecture_->DisplayInSummary( displayer );
    assert( usages_.get() );
    usages_->DisplayInSummary( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::GetInvalidMaterial
// Created: LDC 2013-11-15
// -----------------------------------------------------------------------------
const std::string& PhysicalAttribute_ABC::GetInvalidMaterial() const
{
    return architecture_->GetInvalidMaterial();
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::GetInvalidRoofShape
// Created: LDC 2013-11-15
// -----------------------------------------------------------------------------
const std::string& PhysicalAttribute_ABC::GetInvalidRoofShape() const
{
    return architecture_->GetInvalidRoofShape();
}
