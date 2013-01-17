// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertiesComparator.h"

// -----------------------------------------------------------------------------
// Name: PropertiesComparator constructor
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
PropertiesComparator::PropertiesComparator( const kernel::CoordinateConverter_ABC& converter )
    : PropertyDisplayer( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesComparator destructor
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
PropertiesComparator::~PropertiesComparator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesComparator::StartDisplay
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
void PropertiesComparator::StartDisplay()
{
    texts_.push_back( QString() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesComparator::DisplayFormatted
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
void PropertiesComparator::DisplayFormatted( const QString& formatted )
{
    if( !texts_.empty() )
        texts_.back() += formatted;
}

// -----------------------------------------------------------------------------
// Name: PropertiesComparator::EndDisplay
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
void PropertiesComparator::EndDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesComparator::Clear
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
void PropertiesComparator::Clear()
{
    texts_.clear();
}

// -----------------------------------------------------------------------------
// Name: PropertiesComparator::IsValid
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
bool PropertiesComparator::IsValid() const
{
    if( texts_.empty() )
        return true;
    return std::adjacent_find( texts_.begin(), texts_.end(), std::not_equal_to< QString >() ) == texts_.end();
}
