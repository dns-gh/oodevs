// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Variables.h"
#include "Variable.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Variables constructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
Variables::Variables()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variables constructor
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
Variables::Variables( const Variables& variables )
    : variables_( variables.variables_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variables constructor
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
Variables::Variables( xml::xistream& xis )
{
    xis >> xml::start( "constants" )
            >> xml::list( "constant", *this, &Variables::ReadVariable )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Variables destructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
Variables::~Variables()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variables::Register
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
void Variables::Register( const std::string& name, boost::shared_ptr< Element_ABC > element )
{
    variables_[ name ] = element;
}

// -----------------------------------------------------------------------------
// Name: Variables::Find
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > Variables::Find( const std::string& name ) const
{
    T_Variables::const_iterator it = variables_.find( name );
    if( it != variables_.end() )
        return it->second;
    return boost::shared_ptr< Element_ABC >();
}

// -----------------------------------------------------------------------------
// Name: Variables::ReadVariable
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void Variables::ReadVariable( xml::xistream& xis )
{
    boost::shared_ptr< Element_ABC > element( new Variable( xis ) );
    Register( boost::erase_first_copy( element->GetInput(), "$" ), element );
}

// -----------------------------------------------------------------------------
// Name: Variables::Serialize
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void Variables::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "constants" );
    BOOST_FOREACH( const T_Variables::value_type& variable, variables_ )
        variable.second->SerializeDeclaration( xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Variables::Accept
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void Variables::Accept( VariablesVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_Variables::value_type& variable, variables_ )
        visitor.Visit( *variable.second );
}

// -----------------------------------------------------------------------------
// Name: Variables::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Variables& Variables::Clone() const
{
    Variables* clone = new Variables();
    BOOST_FOREACH( const T_Variables::value_type& variable, variables_ )
        clone->Register( variable.first, boost::shared_ptr< Element_ABC >( & variable.second->Clone() ) );
    return *clone;
}

// -----------------------------------------------------------------------------
// Name: Variables::operator=
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Variables& Variables::operator=( const Variables& rhs )
{
    variables_.clear();
    BOOST_FOREACH( const T_Variables::value_type& variable, rhs.variables_ )
        Register( variable.first, variable.second );
    return *this;
}
