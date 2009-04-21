// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorPrimitive.h"
#include "IndicatorPrimitiveParameter.h"
#include "IndicatorElement_ABC.h"
#include "IndicatorType.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorPrimitive::IndicatorPrimitive( xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , category_( xml::attribute< std::string >( xis, "category" ) )
    , type_( new IndicatorType( xis ) )
{
    // $$$$ SBO 2009-04-06: Read "comment"
    xis >> xml::optional() >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &IndicatorPrimitive::ReadParameter )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorPrimitive::~IndicatorPrimitive()
{
    for( T_Parameters::iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::ReadParameter
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
void IndicatorPrimitive::ReadParameter( xml::xistream& xis )
{
    parameters_.push_back( new IndicatorPrimitiveParameter( xis ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::GetName
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
QString IndicatorPrimitive::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::GetCategory
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string IndicatorPrimitive::GetCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::GetPrototype
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString IndicatorPrimitive::GetPrototype() const
{
    return QString( "%1(%2)" ).arg( name_ ).arg( BuildParameterList() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
const IndicatorType& IndicatorPrimitive::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::FindParameter
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
const IndicatorPrimitiveParameter* IndicatorPrimitive::FindParameter( unsigned int index ) const
{
    return index < parameters_.size() ? parameters_.at( index ) : 0;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitive::BuildParameterList
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString IndicatorPrimitive::BuildParameterList() const
{
    std::vector< std::string > list;
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        list.push_back( parameter->GetName().ascii() );
    return boost::join( list, "," ).c_str();
}
