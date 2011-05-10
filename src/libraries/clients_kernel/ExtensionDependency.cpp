// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ExtensionDependency.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ExtensionDependency constructor
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
ExtensionDependency::ExtensionDependency( xml::xistream& xis )
    : name_  ( xis.attribute< std::string>( "name" ) )
{
    std::string values = xis.attribute< std::string>( "values" );
    boost::split( values_, values, boost::algorithm::is_any_of( ";" ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionDependency destructor
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
ExtensionDependency::~ExtensionDependency()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionDependency::GetName
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
const std::string& ExtensionDependency::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ExtensionDependency::Allow
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
bool ExtensionDependency::Allow( const std::string& value ) const
{
    for( std::vector< std::string >::const_iterator it = values_.begin(); it != values_.end(); ++it )
        if( *it == value )
            return true;
    return false;
}
