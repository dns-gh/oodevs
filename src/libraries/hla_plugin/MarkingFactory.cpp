// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "MarkingFactory.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: MarkingFactory constructor
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
MarkingFactory::MarkingFactory( xml::xisubstream xis )
    : addIdentifier_( xis.attribute< bool >( "marking-with-identifier", true ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MarkingFactory destructor
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
MarkingFactory::~MarkingFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MarkingFactory::CreateAggregateMarking
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
AggregateMarking MarkingFactory::CreateAggregateMarking( const std::string& name, unsigned int identifier ) const
{
    return addIdentifier_ ? AggregateMarking( name, identifier ) : AggregateMarking( name );
}

// -----------------------------------------------------------------------------
// Name: MarkingFactory::CreateMarking
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
Marking MarkingFactory::CreateMarking( const std::string& name, unsigned int identifier ) const
{
    return addIdentifier_ ? Marking( name, identifier ) : Marking( name );
}
