// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorActionTypeParameter.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypeParameter constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionTypeParameter::SuccessFactorActionTypeParameter( xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , default_( xml::attribute< std::string >( xis, "default", "" ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypeParameter destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionTypeParameter::~SuccessFactorActionTypeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypeParameter::GetName
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QString SuccessFactorActionTypeParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypeParameter::GetDefault
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QString SuccessFactorActionTypeParameter::GetDefault() const
{
    return default_;
}
