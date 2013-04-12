// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentNature.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentNature constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
AgentNature::AgentNature( xml::xistream& xis )
    : depth_( 0.f )
{
    xis >> xml::start( "nature" )
            >> xml::attribute( "level", level_ )
            >> xml::attribute( "nature-app6", nature_ )
            >> xml::attribute( "atlas-nature", atlas_ )
            >> xml::optional >> xml::attribute( "depth", depth_ )
        >> xml::end;
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

// -----------------------------------------------------------------------------
// Name: AgentNature::GetDepth
// Created: LDC 2013-04-12
// -----------------------------------------------------------------------------
const float AgentNature::GetDepth() const
{
    return depth_;
}
