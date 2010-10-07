// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DictionaryExtensions.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions()
    : enabled_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions( xml::xistream& xis )
    : enabled_( true )
{
    xis >> xml::list( "entry", *this, &DictionaryExtensions::ReadEntry );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions destructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
DictionaryExtensions::~DictionaryExtensions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SerializeAttributes
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void DictionaryExtensions::SerializeAttributes( xml::xostream& xos ) const
{
    if( enabled_ && !entries_.empty() )
    {
        xos << xml::start( "extensions" );
        for( CIT_Entries it = entries_.begin(); it != entries_.end(); ++it )
            xos << xml::start( "entry" )
                << xml::attribute( "key", it->first )
                << xml::attribute( "value", it->second )
                << xml::end;
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SetEnabled
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void DictionaryExtensions::SetEnabled( bool enabled )
{
    enabled_ = enabled;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::IsEnabled
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
bool DictionaryExtensions::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::SetValue
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void DictionaryExtensions::SetValue( const std::string& name, const std::string& value )
{
    entries_[ name ] = value;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::GetValue
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
const std::string& DictionaryExtensions::GetValue( const std::string& name ) const
{
    static const std::string defaultString;
    CIT_Entries it = entries_.find( name );
    if( it != entries_.end() )
        return it->second;
    return defaultString;
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::ReadEntry
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void DictionaryExtensions::ReadEntry( xml::xistream& xis )
{
    entries_[ xis.attribute< std::string >( "key" ) ] = xis.attribute< std::string >( "value" );
}
