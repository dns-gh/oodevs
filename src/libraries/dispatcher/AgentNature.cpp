// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentNature.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentNature constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
AgentNature::AgentNature( xml::xistream& xis )
{
    xis >> start( "nature" )
            >> attribute( "level", level_ )
            >> attribute( "nature", nature_ )
            >> attribute( "atlas-nature", atlas_ )
        >> end();
}
    
// -----------------------------------------------------------------------------
// Name: AgentNature destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
AgentNature::~AgentNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentNature::GetLevel
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
const std::string& AgentNature::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: AgentNature::GetNature
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
const std::string& AgentNature::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: AgentNature::GetAtlas
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
const std::string& AgentNature::GetAtlas() const
{
    return atlas_;
}
