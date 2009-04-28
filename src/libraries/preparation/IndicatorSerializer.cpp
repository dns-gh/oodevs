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
#include "IndicatorElementDeclarator_ABC.h"
#include "IndicatorVariables.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorSerializer::IndicatorSerializer( const IndicatorElementFactory_ABC& factory, const IndicatorVariables& variables )
    : factory_( factory )
    , variables_( variables )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer destructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorSerializer::~IndicatorSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleNumber
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleNumber( double value )
{
    stack_.push_back( factory_.CreateNumber( value ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleString
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleString( const std::string& value )
{
    stack_.push_back( factory_.CreateString( value ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleVariable
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleVariable( const std::string& name )
{
    stack_.push_back( factory_.CreateVariable( name ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::HandleFunctionCall
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorSerializer::HandleFunctionCall( const std::string& name, unsigned int parameters )
{
    boost::shared_ptr< IndicatorElement_ABC > function( factory_.CreateFunction( name ) );
    if( !stack_.empty() )
        std::reverse( stack_.end() - parameters, stack_.end() );
    for( unsigned int i = 0; i < parameters && !stack_.empty(); ++i )
    {
        function->AddParameter( stack_.back() );
        stack_.pop_back();
    }
    stack_.push_back( function );
}

namespace
{
    class IndicatorElementDeclarator : public IndicatorElementDeclarator_ABC
    {
    public:
        explicit IndicatorElementDeclarator( xml::xostream& xos ) : xos_( &xos ) {}
        virtual ~IndicatorElementDeclarator() {}

        virtual void Declare( boost::shared_ptr< IndicatorElement_ABC > element )
        {
            if( std::find( elements_.begin(), elements_.end(), element ) == elements_.end() )
            {
                element->SerializeDeclaration( *xos_ );
                elements_.push_back( element );
            }
        }
    private:
        xml::xostream* xos_;
        std::vector< boost::shared_ptr< IndicatorElement_ABC > > elements_;
    };
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer::Serialize
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void IndicatorSerializer::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "indicator" );
    if( ! stack_.empty() )
    {
        IndicatorElementDeclarator declarator( xos );
        stack_.front()->Serialize( xos, declarator );
    }
    xos << xml::end();
}
