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
{
    xis >> xml::list( "group", *this, &AgentComposition::ReadGroup );
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
// Name: AgentComposition::ReadGroup
// Created: MCO 2013-04-18
// -----------------------------------------------------------------------------
void AgentComposition::ReadGroup( xml::xistream& xis )
{
    groups_.push_back(
        std::make_pair(
            xis.attribute< std::string >( "name" ),
            xis.attribute< unsigned int >( "count" ) ) );
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
// Name: AgentComposition::GetGroups
// Created: MCO 2013-04-18
// -----------------------------------------------------------------------------
const AgentComposition::T_Groups& AgentComposition::GetGroups() const
{
    return groups_;
}
