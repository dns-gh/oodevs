// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorSerializer.h"
#include "IndicatorElementFactory_ABC.h"
#include "IndicatorElement_ABC.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

namespace bs = boost::spirit::classic;

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorSerializer::IndicatorSerializer( const IndicatorElementFactory_ABC& factory, xml::xostream& xos )
    : factory_( factory )
    , xos_( xos )
{
    xos_ << xml::start( "indicator" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer destructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorSerializer::~IndicatorSerializer()
{
    // $$$$ SBO 2009-03-17: what if stack not empty...
    xos_ << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleNumber
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleNumber( double value )
{
    boost::shared_ptr< IndicatorElement_ABC > element( factory_.CreateNumber( value ) );
    stack_.push_back( element );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleExtract
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleExtract( const boost::spirit::classic::tree_match< const char* >::const_tree_iterator& it )
{

}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleVariable
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleVariable( const std::string& name )
{
    boost::shared_ptr< IndicatorElement_ABC > element( factory_.CreateVariable( name ) );
    stack_.push_back( element );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleFunctionCall
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleFunctionCall( const std::string& name, unsigned int parameters )
{
    boost::shared_ptr< IndicatorElement_ABC > function( factory_.CreateFunction( name ) );
    std::reverse( stack_.end() - parameters, stack_.end() );
    for( unsigned int i = 0; i < parameters && !stack_.empty(); ++i )
    {
        function->AddParameter( stack_.back() );
        stack_.pop_back();
    }
    function->Serialize( xos_ );
    stack_.push_back( function );
}
