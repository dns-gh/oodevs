// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "BreakdownType.h"
#include "ENT/ENT_Tr.h"
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BreakdownType constructor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
BreakdownType::BreakdownType( xml::xistream& xis, const std::string& category )
    : category_( ENT_Tr::ConvertToBreakdownNTI( category ) )
    , type_    ( ENT_Tr::ConvertToBreakdownType( xis.attribute< std::string >( "type" ) ) )
    , id_      ( xis.attribute< unsigned long >( "id" ) )
    , name_    ( xis.attribute< std::string >( "name" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownType destructor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
BreakdownType::~BreakdownType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetId
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
unsigned long BreakdownType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetName
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
const std::string& BreakdownType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetUnknownName
// Created: SLI 2014-01-20
// -----------------------------------------------------------------------------
std::string BreakdownType::GetUnknownName() const
{
    return ENT_Tr::ConvertFromBreakdownType( type_, ENT_Tr::eToTr ) + " - " + ENT_Tr::ConvertFromBreakdownNTI( category_, ENT_Tr::eToTr );
}
