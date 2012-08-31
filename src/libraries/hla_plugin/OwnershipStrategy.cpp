// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "OwnershipStrategy.h"
#include "OwnershipController_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    OwnershipStrategy::AcceptKind readStrategy( xml::xistream& xis )
    {
        std::string tmp( xis.attribute< std::string >( "accept-external-transfer", "negotiated" ) );
        if( tmp == "always" )
            return OwnershipStrategy::ALWAYS;
        if( tmp == "never" )
            return OwnershipStrategy::NEVER;

        return OwnershipStrategy::NEGOTIATED;
    }
}

// -----------------------------------------------------------------------------
// Name: OwnershipStrategy constructor
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
OwnershipStrategy::OwnershipStrategy( xml::xistream& xis )
    : doNegotiation_( xis.attribute< bool >( "attribute-ownership-negotiation", true ) )
    , accept_( readStrategy( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OwnershipStrategy destructor
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
OwnershipStrategy::~OwnershipStrategy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OwnershipStrategy::AcceptAcquisition
// Created: AHC 2012-02-23
// -----------------------------------------------------------------------------
bool OwnershipStrategy::AcceptAcquisition( const std::string& /*objectID*/, bool negotiated )
{
    return accept_ == OwnershipStrategy::ALWAYS ||
            ( negotiated && accept_ == OwnershipStrategy::NEGOTIATED);
}

// -----------------------------------------------------------------------------
// Name: OwnershipStrategy::AcceptDivestiture
// Created: AHC 2012-02-23
// -----------------------------------------------------------------------------
bool OwnershipStrategy::AcceptDivestiture( const std::string& /*objectID*/, bool negotiated )
{
    return accept_ == OwnershipStrategy::ALWAYS ||
            ( negotiated && accept_ == OwnershipStrategy::NEGOTIATED);
}

// -----------------------------------------------------------------------------
// Name: OwnershipStrategy::PerformAttributeOwnershipNegotiation
// Created: AHC 2012-04-16
// -----------------------------------------------------------------------------
bool OwnershipStrategy::PerformAttributeOwnershipNegotiation()
{
    return doNegotiation_;
}
