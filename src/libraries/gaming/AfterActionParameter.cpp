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
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>

// -----------------------------------------------------------------------------
// Name: AfterActionParameter constructor
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
AfterActionParameter::AfterActionParameter( xml::xistream& xis )
    : name_ ( xml::attribute< std::string >( xis, "name" ) )
    , type_ ( xml::attribute< std::string >( xis, "type" ) )
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
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void AfterActionParameter::Commit( std::string& content, const std::string& value ) const
{
    const std::string parameter = "${" + name_ + "}";
    boost::algorithm::replace_all( content, parameter, value );
}

// -----------------------------------------------------------------------------
// Name: AfterActionParameter::GetType
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
std::string AfterActionParameter::GetType() const
{
    return type_;
}
