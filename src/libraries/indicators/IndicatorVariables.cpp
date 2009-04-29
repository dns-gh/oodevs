// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "IndicatorVariables.h"
#include "IndicatorVariable.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorVariables constructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
IndicatorVariables::IndicatorVariables()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables constructor
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
IndicatorVariables::IndicatorVariables( xml::xistream& xis )
{
    xis >> xml::start( "constants" )
            >> xml::list( "constant", *this, &IndicatorVariables::ReadVariable )
        >> xml::end();
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

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::ReadVariable
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void IndicatorVariables::ReadVariable( xml::xistream& xis )
{
    boost::shared_ptr< IndicatorElement_ABC > element( new IndicatorVariable( xis ) );
    Register( element->GetInput(), element );
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::Serialize
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void IndicatorVariables::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "constants" );
    BOOST_FOREACH( const T_Variables::value_type& variable, variables_ )
        variable.second->SerializeDeclaration( xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::Accept
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void IndicatorVariables::Accept( IndicatorVariablesVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_Variables::value_type& variable, variables_ )
        visitor.Visit( *variable.second );
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
IndicatorVariables& IndicatorVariables::Clone() const
{
    IndicatorVariables* clone = new IndicatorVariables();
    BOOST_FOREACH( const T_Variables::value_type& variable, variables_ )
        clone->Register( variable.first, boost::shared_ptr< IndicatorElement_ABC >( & variable.second->Clone() ) );
    return *clone;
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariables::operator=
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
IndicatorVariables& IndicatorVariables::operator=( const IndicatorVariables& rhs )
{
    variables_.clear();
    BOOST_FOREACH( const T_Variables::value_type& variable, rhs.variables_ )
        Register( variable.first, variable.second );
    return *this;
}
