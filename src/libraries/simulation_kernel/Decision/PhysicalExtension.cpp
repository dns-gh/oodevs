// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PhysicalExtension.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    std::map< std::string, std::string > extensions;
}

void PhysicalExtension::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "physical-extensions" )
            >> xml::list( "extension", [&]( xml::xistream& xis )
                {
                    const std::string key = xis.attribute< std::string >( "key" );
                    if( extensions.find( key ) != extensions.end() )
                        throw MASA_EXCEPTION( xis.context() + "Extension already defined" );
                    extensions[ key ] = xis.attribute< std::string >( "value" );
                } )
        >> xml::end;
}

std::string PhysicalExtension::Find( const std::string& key )
{
    auto it = extensions.find( key );
    if( it == extensions.end() )
        return "";
    return it->second;
}
