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
#include "ElementDeclarator_ABC.h"
#include "ElementType.h"
#include "ElementTypeResolver.h"
#include "Primitive.h"
#include "PrimitiveParameter.h"
#include "clients_kernel/Tools.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Function constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
Function::Function( const std::string& id, const Primitive& primitive )
    : Element_ABC( id )
    , primitive_( primitive )
    , typeResolver_( boost::shared_ptr< ElementTypeResolver >( new ElementTypeResolver() ) )
    , type_( new ElementType( primitive_.GetType(), typeResolver_ ) )
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
// Name: Function::AddParameter
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void Function::AddParameter( boost::shared_ptr< Element_ABC > element )
{
    const PrimitiveParameter* parameter = primitive_.FindParameter( parameters_.size() );
    if( !parameter )
        throw std::exception( tools::translate( "Indicators", "Parameter mismatch in function '%1'." ).arg( primitive_.GetName() ).ascii() );
    if( element->GetType() != parameter->GetType() )
        throw std::exception( tools::translate( "Indicators", "Parameter type mismatch in function '%1': %2 != %3." ).arg( primitive_.GetName() )
                                                .arg( element->GetType().ToString().c_str() )
                                                .arg( parameter->GetType().ToString().c_str() ) );
    parameters_.push_back( std::make_pair( parameter, element ) );
    typeResolver_->AddElement( element->GetType(), parameter->GetType() );
}

// -----------------------------------------------------------------------------
// Name: Function::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
const ElementType& Function::GetType() const
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
    if( parameters_.size() < primitive_.ParameterCount() )
        throw std::exception( tools::translate( "Indicators", "Too few parameters in function '%1'." ).arg( primitive_.GetName() ).ascii() );
    SerializeDeclarations( xos, declarator );
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        parameter.second->Serialize( xos, declarator );
    xos << xml::start( primitive_.GetCategory() )
            << xml::attribute( "function", primitive_.GetName() );
    if( primitive_.GetCategory() != "result" )
        xos << xml::attribute( "id", GetInput() );
    SerializeType( xos );
    SerializeParameters( xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Function::SerializeDeclarations
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void Function::SerializeDeclarations( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const
{
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        if( parameter.first->GetAttribute() == "input" )
            declarator.Declare( parameter.second );
}

// -----------------------------------------------------------------------------
// Name: Function::SerializeType
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void Function::SerializeType( xml::xostream& xos ) const
{
    if( primitive_.GetCategory() == "extract" )
        return;
    const std::string simpleType = ElementTypeResolver::ToSimpleType( GetType().Resolve() );
    xos << xml::attribute( "type", simpleType );
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
        const std::string name = parameter.first->GetAttribute();
        attributes[ name ].push_back( name == "input" ? parameter.second->GetInput() : parameter.second->GetValue() );
    }
    BOOST_FOREACH( const T_Attributes::value_type& attribute, attributes )
        if( attribute.second.size() )
            xos << xml::attribute( attribute.first, boost::algorithm::join( attribute.second, "," ) );
}

// -----------------------------------------------------------------------------
// Name: Function::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Element_ABC& Function::Clone() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented." );
}
