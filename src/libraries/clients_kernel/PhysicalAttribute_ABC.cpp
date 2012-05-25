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
    : architecture_( 0 )
    , usages_      ( 0 )
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
Architecture_ABC* PhysicalAttribute_ABC::GetArchitecture() const
{
    return architecture_.get();
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::GetUsages
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
Usages_ABC* PhysicalAttribute_ABC::GetUsages() const
{
    return usages_.get();
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::FindUsagesValue
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
unsigned int PhysicalAttribute_ABC::FindUsagesValue( const std::string& motivation ) const
{
    if( usages_.get() )
        return usages_->Find( motivation );
    return 0u;
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::Display
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::Display( Displayer_ABC& displayer ) const
{
    if( architecture_.get() )
        architecture_->Display( displayer );
    if( usages_.get() )
        usages_->Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::DisplayInList
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::DisplayInList( Displayer_ABC& displayer ) const
{
    if( architecture_.get() )
        architecture_->DisplayInList( displayer );
    if( usages_.get() )
        usages_->DisplayInList( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::DisplayInTooltip
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    if( architecture_.get() )
        architecture_->DisplayInTooltip( displayer );
    if( usages_.get() )
        usages_->DisplayInTooltip( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::DisplayInSummary
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::DisplayInSummary( Displayer_ABC& displayer ) const
{
    if( architecture_.get() )
        architecture_->DisplayInSummary( displayer );
    if( usages_.get() )
        usages_->DisplayInSummary( displayer );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute_ABC::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void PhysicalAttribute_ABC::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "physical" );
    if( architecture_.get() )
        architecture_->SerializeAttributes( xos );
    if( usages_.get() )
        usages_->SerializeAttributes( xos );
    xos << xml::end;
}
