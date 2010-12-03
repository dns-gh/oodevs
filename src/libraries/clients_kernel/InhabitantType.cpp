// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "InhabitantType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InhabitantType constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantType::InhabitantType( xml::xistream& xis, const tools::Resolver_ABC< PopulationType, std::string >& modelResolver )
{
    std::string model;
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "associated-crowd", model );
    type_ = &modelResolver.Get( model );
}

// -----------------------------------------------------------------------------
// Name: InhabitantType destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantType::~InhabitantType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetName
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
std::string InhabitantType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetId
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
unsigned long InhabitantType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantType::GetCrowdType
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
const PopulationType& InhabitantType::GetCrowdType() const
{
    return *type_;
}
