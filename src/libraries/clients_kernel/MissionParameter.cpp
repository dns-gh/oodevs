// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MissionParameter.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: MissionParameter constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
MissionParameter::MissionParameter( xml::xistream& xis )
{
    std::string name, type;
    xis >> attribute( "name", name )
        >> attribute( "type", type )
        >> attribute( "optional", optional_ );
    name_ = name.c_str();
    type_ = type.c_str();
}

// -----------------------------------------------------------------------------
// Name: MissionParameter destructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
MissionParameter::~MissionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter::GetName
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
QString MissionParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter::GetType
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
QString MissionParameter::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter::IsOptional
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
bool MissionParameter::IsOptional() const
{
    return optional_;
}
