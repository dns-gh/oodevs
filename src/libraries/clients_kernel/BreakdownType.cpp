// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "BreakdownType.h"
#include "ENT/ENT_Tr.h"
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>

using namespace kernel;

namespace
{
    BreakdownType::T_Parts ReadParts( xml::xistream& xis )
    {
        BreakdownType::T_Parts parts;
        xis >> xml::list( "part", [&]( xml::xistream& xis ){
            BreakdownType::T_Part part;
            xis >> xml::attribute( "quantity", part.quantity )
                >> xml::attribute( "resource", part.resource );
            parts.push_back( part );
        });
        return parts;
    }
}

// -----------------------------------------------------------------------------
// Name: BreakdownType constructor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
BreakdownType::BreakdownType( xml::xistream& xis, const std::string& category )
    : category_( ENT_Tr::ConvertToBreakdownNTI( category ) )
    , type_    ( ENT_Tr::ConvertToBreakdownType( xis.attribute< std::string >( "type" ) ) )
    , id_      ( xis.attribute< unsigned long >( "id" ) )
    , name_    ( xis.attribute< std::string >( "name" ) )
    , parts_   ( ReadParts( xis ) )
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

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetNTI
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
E_BreakdownNTI BreakdownType::GetNTI() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetType
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
E_BreakdownType BreakdownType::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetParts
// Created: BAX 2014-02-10
// -----------------------------------------------------------------------------
const BreakdownType::T_Parts& BreakdownType::GetParts() const
{
    return parts_;
}
