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
#include "DataTypeFactory.h"
#include "ElementTypeResolver.h"
#include "Primitive.h"
#include "Primitives.h"
#include "Variables.h"
#include "clients_kernel/Tools.h"
#pragma warning( push )
#pragma warning( disable : 4512 4702 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: ElementFactory constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
ElementFactory::ElementFactory( const Primitives& primitives, const Variables& variables )
    : primitives_( primitives )
    , variables_( variables )
    , types_( new DataTypeFactory() )
    , resolver_( new ElementTypeResolver() )
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
    return boost::shared_ptr< Element_ABC >( new Constant< double >( NextId(), types_->Instanciate( "float", resolver_ ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateString
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateString( const std::string& value ) const
{
    return boost::shared_ptr< Element_ABC >( new Constant< std::string >( value, types_->Instanciate( "string", resolver_ ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateVariable
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateVariable( const std::string& name ) const
{
    boost::shared_ptr< Element_ABC > element( variables_.Find( name ) );
    if( element == 0 )
        throw MASA_EXCEPTION( tools::translate( "Indicators", "Undefined variable: %1." ).arg( name.c_str() ).toStdString() );
    return element;
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::CreateFunction
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > ElementFactory::CreateFunction( const std::string& name ) const
{
    if( const Primitive* primitive = primitives_.Find( boost::algorithm::to_lower_copy( name ).c_str() ) )
        return primitive->Instanciate( NextId() );
    throw MASA_EXCEPTION( tools::translate( "Indicators", "Undefined function: %1." ).arg( name.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ElementFactory::NextId
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
std::string ElementFactory::NextId() const
{
    return boost::lexical_cast< std::string, unsigned long >( ++id_ );
}
