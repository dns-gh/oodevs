// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FillRuleFactory.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FillRuleFactory constructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
FillRuleFactory::FillRuleFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FillRuleFactory destructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
FillRuleFactory::~FillRuleFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FillRuleFactory::CreateProperty
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
FillRule* FillRuleFactory::CreateProperty( const std::string& rule ) const
{
    if( rule != "nonzero" && rule != "evenodd" )
        return 0;
    return new FillRule( rule );
}
