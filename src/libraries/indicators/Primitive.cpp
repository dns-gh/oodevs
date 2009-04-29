// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Primitive.h"
#include "PrimitiveParameter.h"
#include "Element_ABC.h"
#include "ElementType.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.h>

using namespace indicators;

namespace
{
    QString ReadComment( xml::xistream& xis )
    {
        std::string comment;
        xis >> xml::start( "comments" ) >> comment >> xml::end();
        return comment.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Primitive constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
Primitive::Primitive( xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , category_( xml::attribute< std::string >( xis, "category" ) )
    , comment_( ReadComment( xis ) )
    , type_( new ElementType( xis ) )
{
    xis >> xml::optional() >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &Primitive::ReadParameter )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Primitive destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
Primitive::~Primitive()
{
    for( T_Parameters::iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Primitive::ReadParameter
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
void Primitive::ReadParameter( xml::xistream& xis )
{
    parameters_.push_back( new PrimitiveParameter( xis ) );
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetName
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
QString Primitive::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetCategory
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string Primitive::GetCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetComment
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
QString Primitive::GetComment() const
{
    return comment_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetPrototype
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString Primitive::GetPrototype() const
{
    return QString( "%1(%2)" ).arg( name_ ).arg( BuildParameterList() );
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
const ElementType& Primitive::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::FindParameter
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
const PrimitiveParameter* Primitive::FindParameter( unsigned int index ) const
{
    return index < parameters_.size() ? parameters_.at( index ) : 0;
}

// -----------------------------------------------------------------------------
// Name: Primitive::BuildParameterList
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString Primitive::BuildParameterList() const
{
    std::vector< std::string > list;
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        list.push_back( parameter->GetName().ascii() );
    return list.empty() ? "" : ( " " + boost::join( list, ", " ) + " " ).c_str();
}

// -----------------------------------------------------------------------------
// Name: Primitive::ParameterCount
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
unsigned int Primitive::ParameterCount() const
{
    return parameters_.size();
}
