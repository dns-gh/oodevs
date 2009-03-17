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
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

namespace bs = boost::spirit::classic;

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorSerializer::IndicatorSerializer( xml::xostream& xos )
    : xos_( xos )
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
    stack_.push_back( boost::lexical_cast< std::string >( value ) );
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
    stack_.push_back( std::string( "$" ) + name );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleFunctionCall
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleFunctionCall( const std::string& name, unsigned int parameters )
{
    xos_ << xml::start( "function" )
            << xml::attribute( "name", name );
    std::reverse( stack_.end() - parameters, stack_.end() );
    for( unsigned int i = 0; i < parameters && !stack_.empty(); ++i )
    {
        const T_Parameter& parameter = stack_.back();
        xos_ << xml::start( "parameter" )
                << xml::attribute( "value", parameter )
             << xml::end();
        stack_.pop_back();
    }
    xos_ << xml::end();
    stack_.push_back( std::string( "Function_" + name ) );
}
