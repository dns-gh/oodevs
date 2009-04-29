// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "ElementFactory.h"
#include "Constant.h"
#include "ElementType.h"
#include "Function.h"
#include "Primitives.h"
#include "Variables.h"
#include "clients_kernel/Tools.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: ElementFactory constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
ElementFactory::ElementFactory( const Primitives& primitives, const Variables& variables )
    : primitives_( primitives )
    , variables_( variables )
    , id_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementFactory destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
ElementFactory::~ElementFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateNumber
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateNumber( double value ) const
{
    return boost::shared_ptr< Element_ABC >( new Constant< double >( NextId(), "float", value ) );
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateString
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateString( const std::string& value ) const
{
    return boost::shared_ptr< Element_ABC >( new Constant< std::string >( value, "string", value ) );
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateVariable
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateVariable( const std::string& name ) const
{
    boost::shared_ptr< Element_ABC > element( variables_.Find( name ) );
    if( element == 0 )
        throw std::exception( tools::translate( "Indicators", "Undefined variable: %1." ).arg( name.c_str() ).ascii() );
    return element;
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateFunction
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateFunction( const std::string& name ) const
{
    const Primitive* primitive = primitives_.Find( boost::algorithm::to_lower_copy( name ).c_str() );
    if( !primitive )
        throw std::exception( tools::translate( "Indicators", "Undefined function: %1." ).arg( name.c_str() ).ascii() );
    return boost::shared_ptr< Element_ABC >( new Function( NextId(), *primitive ) );
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::NextId
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
std::string ElementFactory::NextId() const
{
    return boost::lexical_cast< std::string, unsigned long >( ++id_ );
}
