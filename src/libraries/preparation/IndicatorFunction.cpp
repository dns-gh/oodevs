// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorFunction.h"
#include "IndicatorPrimitive.h"
#include "IndicatorPrimitiveParameter.h"
#include "IndicatorType.h"
#include "IndicatorTypeResolver.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorFunction constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorFunction::IndicatorFunction( const std::string& id, const IndicatorPrimitive& primitive )
    : IndicatorElement_ABC( id )
    , primitive_( primitive )
    , typeResolver_( boost::shared_ptr< IndicatorTypeResolver >( new IndicatorTypeResolver() ) )
    , type_( new IndicatorType( primitive_.GetType(), typeResolver_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorFunction::~IndicatorFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::AddParameter
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void IndicatorFunction::AddParameter( boost::shared_ptr< IndicatorElement_ABC > element )
{
    const IndicatorPrimitiveParameter* parameter = primitive_.FindParameter( parameters_.size() );
    if( !parameter )
        throw std::exception( "Parameter mismatch." );
    if( element->GetType() != parameter->GetType() )
    {
        std::stringstream ss;
        ss << "parameter type mismatch : " << element->GetType().ToString() << " != " << parameter->GetType().ToString();
        throw std::exception( ss.str().c_str() );
    }
    parameters_.push_back( std::make_pair( parameter, element ) );
    typeResolver_->AddElement( element->GetType(), parameter->GetType() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
const IndicatorType& IndicatorFunction::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::Serialize
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void IndicatorFunction::Serialize( xml::xostream& xos ) const
{
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        parameter.second->SerializeDeclaration( xos );
    xos << xml::start( primitive_.GetCategory() )
            << xml::attribute( "id", GetId() )
            << xml::attribute( "function", primitive_.GetName() );
    SerializeType( xos );
    SerializeParameters( xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::SerializeType
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void IndicatorFunction::SerializeType( xml::xostream& xos ) const
{
    if( primitive_.GetCategory() == "extract" )
        return;
    const std::string simpleType = IndicatorTypeResolver::ToSimpleType( GetType().Resolve() );
    xos << xml::attribute( "type", simpleType );
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::SerializeParameters
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void IndicatorFunction::SerializeParameters( xml::xostream& xos ) const
{
    typedef std::map< std::string, std::vector< std::string > > T_Attributes;
    T_Attributes attributes;
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        attributes[ parameter.first->GetAttribute() ].push_back( parameter.second->GetId() );
    BOOST_FOREACH( const T_Attributes::value_type& attribute, attributes )
    {
        const std::string value = boost::algorithm::join( attribute.second, "," );
        if( !value.empty() )
            xos << xml::attribute( attribute.first, value );
    }
}
