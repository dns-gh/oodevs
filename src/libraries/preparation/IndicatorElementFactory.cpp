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

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElementFactory::IndicatorElementFactory()
    : id_( 0 )
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
    return new IndicatorConstant< double >( NextId(), value );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::CreateVariable
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorElement_ABC* IndicatorElementFactory::CreateVariable( const std::string& name ) const
{
    return new IndicatorVariable( NextId(), name );
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
    return new IndicatorFunction( NextId(), name );
}

// -----------------------------------------------------------------------------
// Name: IndicatorElementFactory::NextId
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
unsigned long IndicatorElementFactory::NextId() const
{
    return ++id_;
}
