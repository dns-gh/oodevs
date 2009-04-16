// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorVariables.h"

// -----------------------------------------------------------------------------
// Name: IndicatorVariables constructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
IndicatorVariables::IndicatorVariables()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables destructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
IndicatorVariables::~IndicatorVariables()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::Register
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
void IndicatorVariables::Register( const std::string& name, boost::shared_ptr< IndicatorElement_ABC > element )
{
    variables_[ name ] = element;
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::Find
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
boost::shared_ptr< IndicatorElement_ABC > IndicatorVariables::Find( const std::string& name ) const
{
    T_Variables::const_iterator it = variables_.find( name );
    if( it != variables_.end() )
        return it->second;
    return boost::shared_ptr< IndicatorElement_ABC >();
}
