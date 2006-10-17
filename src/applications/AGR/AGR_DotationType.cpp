// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_DotationType.h"

// -----------------------------------------------------------------------------
// Name: AGR_DotationType constructor
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
AGR_DotationType::AGR_DotationType()
    : AGR_Type_ABC( "ScipioBase:TypeDotation", "TypeDotation", "TypeDotation", "TypeDotation", true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DotationType destructor
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
AGR_DotationType::~AGR_DotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DotationType::MOS2RCCode
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
std::string AGR_DotationType::MOS2RCCode( const std::string& strAsnParam ) const
{
    std::stringstream strTmp;
    strTmp << "DotationLink( "
           << strAsnParam
           << ", dotationResolver"
           << " )";
    return strTmp.str();
}
