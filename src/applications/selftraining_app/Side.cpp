// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Side.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Side::Side( xml::xistream& xis )
    : id_ ( xis.attribute< unsigned int >( "id" ) )
{
    xis >> xml::optional >> xml::attribute<std::string>( "name", name_ ) ;
}

// -----------------------------------------------------------------------------
// Name: Side destructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Side::~Side()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Side::GetId
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int Side::GetId() const
{
    return id_ ;
}

// -----------------------------------------------------------------------------
// Name: Side::GetName
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
const std::string Side::GetName() const
{
    return name_ ;
}

