// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderParameterTypeResolver.h"

using namespace plugins::crossbow::tools;

// -----------------------------------------------------------------------------
// Name: OrderParameterTypeResolver::Register
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
OrderParameterTypeResolver& OrderParameterTypeResolver::Register( const std::string& type, int value )
{
    map_[ type ] = value;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterTypeResolver::Resolve
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
int OrderParameterTypeResolver::Resolve( const std::string& type ) const
{
    std::string lowerType = type;
    std::transform( type.begin(), type.end(), lowerType.begin(), &tolower );
    T_TypeMap::const_iterator it = map_.find( lowerType );
    if ( it != map_.end() )
        return it->second;
    throw std::runtime_error( "Unsupported parameter type" );
}
