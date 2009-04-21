// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorElementFactory.h"
#include "IndicatorConstant.h"
#include "IndicatorString.h"
#include "IndicatorVariables.h"
#include "IndicatorFunction.h"
#include "IndicatorType.h"
#include "IndicatorPrimitives.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElementFactory::IndicatorElementFactory( const IndicatorPrimitives& primitives, const IndicatorVariables& variables )
    : primitives_( primitives )
    , variables_( variables )
    , id_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElementFactory::~IndicatorElementFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateNumber
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< IndicatorElement_ABC > IndicatorElementFactory::CreateNumber( double value ) const
{
    return boost::shared_ptr< IndicatorElement_ABC >( new IndicatorConstant< double >( NextId(), "float", value ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateString
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
boost::shared_ptr< IndicatorElement_ABC > IndicatorElementFactory::CreateString( const std::string& value ) const
{
    return boost::shared_ptr< IndicatorElement_ABC >( new IndicatorString( value ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateVariable
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< IndicatorElement_ABC > IndicatorElementFactory::CreateVariable( const std::string& name ) const
{
    boost::shared_ptr< IndicatorElement_ABC > element( variables_.Find( name ) );
    if( element == 0 )
        throw std::exception( ( "Undefined variable : " + name ).c_str() );
    return element;
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateFunction
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
boost::shared_ptr< IndicatorElement_ABC > IndicatorElementFactory::CreateFunction( const std::string& name ) const
{
    const IndicatorPrimitive* primitive = primitives_.Find( boost::algorithm::to_lower_copy( name ).c_str() );
    if( !primitive )
        throw std::exception( ( "Undefined function : " + name ).c_str() );
    return boost::shared_ptr< IndicatorElement_ABC >( new IndicatorFunction( NextId(), *primitive ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::NextId
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
std::string IndicatorElementFactory::NextId() const
{
    return boost::lexical_cast< std::string, unsigned long >( ++id_ );
}
