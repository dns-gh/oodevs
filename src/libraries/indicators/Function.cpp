// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Function.h"
#include "DataType_ABC.h"
#include "ElementDeclarator_ABC.h"
#include "ElementTypeResolver.h"
#include "clients_kernel/Tools.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Function constructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
Function::Function( const std::string& id, const QString& name, const std::string& category, boost::shared_ptr< DataType_ABC > type )
    : Element_ABC( id )
    , name_( name )
    , category_( category )
    , type_( type )
    , parameterSet_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Function destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
Function::~Function()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Function::DeclareParameter
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void Function::DeclareParameter( const std::string& attribute, boost::shared_ptr< Element_ABC > element )
{
    parameters_.push_back( std::make_pair( attribute, element ) );
}

// -----------------------------------------------------------------------------
// Name: Function::AddParameter
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void Function::AddParameter( boost::shared_ptr< Element_ABC > element )
{
    try
    {
        if( parameterSet_ >= parameters_.size() )
            throw std::exception( tools::translate( "Indicators", "too many parameters." ).ascii() );
        boost::shared_ptr< Element_ABC > definition = parameters_[ parameterSet_ ].second;
        type_->AddParameter( definition->GetType(), element->GetType() );
        parameters_[ parameterSet_++ ].second = element;
    }
    catch( std::exception& e )
    {
        throw std::exception( tools::translate( "Indicators", "Parameter type mismatch in function '%1': %2" ).arg( name_ ).arg( e.what() ).ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: Function::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
const DataType_ABC& Function::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Function::GetValue
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
std::string Function::GetValue() const
{
    return GetInput();
}

// -----------------------------------------------------------------------------
// Name: Function::Serialize
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void Function::Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const
{
    if( parameters_.size() > parameterSet_ )
        throw std::exception( tools::translate( "Indicators", "Parameter type mismatch in function '%1': too few parameters." ).arg( name_ ).ascii() );
    SerializeDeclarations( declarator );
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        parameter.second->Serialize( xos, declarator );
    xos << xml::start( category_ )
            << xml::attribute( "function", name_.ascii() );
    if( category_ != "result" )
        xos << xml::attribute( "id", GetInput() );
    SerializeType( xos );
    SerializeParameters( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Function::SerializeDeclarations
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void Function::SerializeDeclarations( ElementDeclarator_ABC& declarator ) const
{
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        if( parameter.first == "input" )
            declarator.Declare( parameter.second );
}

// -----------------------------------------------------------------------------
// Name: Function::SerializeType
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void Function::SerializeType( xml::xostream& xos ) const
{
    if( category_ != "extract" )
    {
        const std::string simpleType = ElementTypeResolver::ToSimpleType( type_->Resolve() );
        xos << xml::attribute( "type", simpleType );
    }
}

// -----------------------------------------------------------------------------
// Name: Function::SerializeParameters
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void Function::SerializeParameters( xml::xostream& xos ) const
{
    typedef std::map< std::string, std::vector< std::string > > T_Attributes;
    T_Attributes attributes;
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
    {
        const std::string name = parameter.first;
        attributes[ name ].push_back( name == "input" ? parameter.second->GetInput() : parameter.second->GetValue() );
    }
    BOOST_FOREACH( const T_Attributes::value_type& attribute, attributes )
        if( attribute.second.size() )
            xos << xml::attribute( attribute.first, boost::algorithm::join( attribute.second, "," ) );
}
