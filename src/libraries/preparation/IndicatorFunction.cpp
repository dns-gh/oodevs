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
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorFunction constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorFunction::IndicatorFunction( unsigned long id, const std::string& name )
    : IndicatorElement_ABC( id )
    , name_( name )
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
    parameters_.push_back( element );
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::Serialize
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void IndicatorFunction::Serialize( xml::xostream& xos ) const
{
    for( std::vector< boost::shared_ptr< IndicatorElement_ABC > >::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->SerializeDeclaration( xos );
    xos << xml::start( "function" )
            << xml::attribute( "id", GetId() )
            << xml::attribute( "name", name_ );
    std::vector< std::string > ss;
    std::transform( parameters_.begin(), parameters_.end(), std::back_inserter( ss )
        , boost::bind( boost::lexical_cast< std::string, unsigned long >, boost::bind( &IndicatorElement_ABC::GetId, _1 ) ) );
    const std::string result = boost::algorithm::join( ss, "," );
    if( ! result.empty() )
        xos << xml::attribute( "input", result );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::SerializeDeclaration
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
void IndicatorFunction::SerializeDeclaration( xml::xostream& xos ) const
{
    // $$$$ SBO 2009-04-03: NOTHING
}
