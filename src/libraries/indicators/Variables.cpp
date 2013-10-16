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
#include "DataTypeFactory.h"
#include "ElementTypeResolver.h"
#include "Variable.h"
#include <tools/Iterator.h>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace indicators;

namespace
{
    std::string StripPrefix( const std::string& name )
    {
        return boost::erase_first_copy( name, "$" );
    }
}

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
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
Variables::Variables( xml::xistream& xis )
{
    DataTypeFactory types;
    xis >> xml::start( "constants" )
            >> xml::list( "constant", *this, &Variables::ReadVariable, types )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Variables constructor
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
Variables::Variables( const Variables& variables )
{
    DataTypeFactory types;
    BOOST_FOREACH( const T_Elements::value_type& variable, variables.elements_ )
    {
        boost::shared_ptr< Element_ABC > element( new Variable( *variable.second, types ) );
        Register( variable.first, element );
    }
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
// Name: Variables::ReadVariable
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void Variables::ReadVariable( xml::xistream& xis, const DataTypeFactory& types )
{
    boost::shared_ptr< Element_ABC > element( new Variable( xis, types ) );
    Register( StripPrefix( element->GetInput() ), element );
}

// -----------------------------------------------------------------------------
// Name: Variables::Register
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
void Variables::Register( const std::string& name, boost::shared_ptr< Element_ABC > element )
{
    elements_[ name ] = element;
}

// -----------------------------------------------------------------------------
// Name: Variables::Find
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > Variables::Find( const std::string& name ) const
{
    T_Elements::const_iterator it = elements_.find( name );
    if( it != elements_.end() )
        return it->second;
    return boost::shared_ptr< Element_ABC >();
}

// -----------------------------------------------------------------------------
// Name: Variables::Serialize
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void Variables::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "constants" );
    BOOST_FOREACH( const T_Elements::value_type& variable, elements_ )
        variable.second->SerializeDeclaration( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Variables::CreateIterator
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
tools::Iterator< const Element_ABC& > Variables::CreateIterator() const
{
    return new tools::AssociativeIterator< const Element_ABC&, T_Elements >( elements_ );
}

// -----------------------------------------------------------------------------
// Name: Variables::operator=
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
Variables& Variables::operator=( const Variables& variables )
{
    elements_.clear();
    DataTypeFactory types;
    BOOST_FOREACH( const T_Elements::value_type& variable, variables.elements_ )
    {
        boost::shared_ptr< Element_ABC > element( new Variable( *variable.second, types ) );
        Register( variable.first, element );
    }
    return *this;
}
