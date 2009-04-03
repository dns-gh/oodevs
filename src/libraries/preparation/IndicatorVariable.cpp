// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorVariable.h"

// -----------------------------------------------------------------------------
// Name: IndicatorVariable constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorVariable::IndicatorVariable( unsigned long id, const std::string& name )
    : IndicatorConstant< std::string >( id, std::string( "$" ) + name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorVariable::~IndicatorVariable()
{
    // NOTHING
}
