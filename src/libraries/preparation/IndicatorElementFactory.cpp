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
#include "IndicatorVariable.h"
#include "IndicatorFunction.h"
#include "IndicatorType.h"
#include "IndicatorPrimitives.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElementFactory::IndicatorElementFactory( const IndicatorPrimitives& primitives )
    : primitives_( primitives )
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
IndicatorElement_ABC* IndicatorElementFactory::CreateNumber( double value ) const
{
    static IndicatorType type( "float" );
    return new IndicatorConstant< double >( NextId(), value, type );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateVariable
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElement_ABC* IndicatorElementFactory::CreateVariable( const std::string& name ) const
{
    static IndicatorType type( "variable" ); // $$$$ SBO 2009-04-09: 
    return new IndicatorVariable( NextId(), name, type );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateExtractor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElement_ABC* IndicatorElementFactory::CreateExtractor( const std::string& name ) const
{
    throw std::runtime_error( "Not implemented" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateFunction
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElement_ABC* IndicatorElementFactory::CreateFunction( const std::string& name ) const
{
    return new IndicatorFunction( NextId(), primitives_.Get( boost::algorithm::to_lower_copy( name ).c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::NextId
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
unsigned long IndicatorElementFactory::NextId() const
{
    return ++id_;
}
