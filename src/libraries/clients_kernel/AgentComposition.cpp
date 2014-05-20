// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentComposition.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentComposition constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
AgentComposition::AgentComposition( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& resolver )
    : type_( resolver.Get( xis.attribute< std::string >( "type" ) ) )
    , count_( xis.attribute< unsigned int >( "count" ) )
    , crew_( xis.attribute< unsigned int >( "crew" ) )
    , major_( xis.attribute< bool >( "major", false ) )
    , convoyer_( xis.attribute< bool >( "convoyer", false ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentComposition destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
AgentComposition::~AgentComposition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::GetType
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
const ComponentType& AgentComposition::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::GetCount
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
unsigned int AgentComposition::GetCount() const
{
    return count_;
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::GetCrew
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
unsigned int AgentComposition::GetCrew() const
{
    return crew_;
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::IsMajor
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
bool AgentComposition::IsMajor() const
{
    return major_;
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::IsConvoyer
// Created: SLI 2014-05-20
// -----------------------------------------------------------------------------
bool AgentComposition::IsConvoyer() const
{
    return convoyer_;
}
