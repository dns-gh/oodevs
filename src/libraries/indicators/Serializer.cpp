// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Serializer.h"
#include "Element_ABC.h"
#include "ElementDeclarator_ABC.h"
#include "ElementFactory_ABC.h"
#include "Variables.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Serializer constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
Serializer::Serializer( const ElementFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Serializer destructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
Serializer::~Serializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Serializer::HandleNumber
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void Serializer::HandleNumber( double value )
{
    stack_.push_back( factory_.CreateNumber( value ) );
}

// -----------------------------------------------------------------------------
// Name: Serializer::HandleString
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void Serializer::HandleString( const std::string& value )
{
    stack_.push_back( factory_.CreateString( value ) );
}

// -----------------------------------------------------------------------------
// Name: Serializer::HandleVariable
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void Serializer::HandleVariable( const std::string& name )
{
    stack_.push_back( factory_.CreateVariable( name ) );
}

// -----------------------------------------------------------------------------
// Name: Serializer::HandleFunctionCall
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void Serializer::HandleFunctionCall( const std::string& name, unsigned int parameters )
{
    boost::shared_ptr< Element_ABC > function( factory_.CreateFunction( name ) );
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
    class ElementDeclarator : public ElementDeclarator_ABC
    {
    public:
        explicit ElementDeclarator( xml::xostream& xos ) : xos_( &xos ) {}
        virtual ~ElementDeclarator() {}

        virtual void Declare( boost::shared_ptr< Element_ABC > element )
        {
            if( std::find( elements_.begin(), elements_.end(), element ) == elements_.end() )
            {
                element->SerializeDeclaration( *xos_ );
                elements_.push_back( element );
            }
        }
    private:
        xml::xostream* xos_;
        std::vector< boost::shared_ptr< Element_ABC > > elements_;
    };
}

// -----------------------------------------------------------------------------
// Name: Serializer::Serialize
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void Serializer::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "indicator" );
    if( ! stack_.empty() )
    {
        ElementDeclarator declarator( xos );
        stack_.front()->Serialize( xos, declarator );
    }
    xos << xml::end();
}
