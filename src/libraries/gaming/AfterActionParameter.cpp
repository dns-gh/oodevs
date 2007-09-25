// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionParameter.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: AfterActionParameter constructor
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
AfterActionParameter::AfterActionParameter( xml::xistream& xis )
    : name_ ( xml::attribute< std::string >( xis, "name" ) )
    , type_ ( xml::attribute< std::string >( xis, "type" ) )
    , value_( xml::attribute( xis, "value", std::string() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionParameter destructor
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
AfterActionParameter::~AfterActionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionParameter::GetName
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
QString AfterActionParameter::GetName() const
{
    return name_.c_str();
}

// -----------------------------------------------------------------------------
// Name: AfterActionParameter::Commit
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionParameter::Commit( xml::xostream& xos, const std::string& name ) const
{
    xos << xml::attribute( name, value_ );
}
