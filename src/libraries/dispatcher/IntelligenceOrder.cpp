// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "IntelligenceOrder.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: IntelligenceOrder constructor
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
IntelligenceOrder::IntelligenceOrder( Model& /*model*/, const ASN1T_Intelligence& asn )
    : name_     ( asn.name )
    , nature_   ( asn.nature )
    , level_    ( asn.level )
    , embarked_ ( asn.embarked ? true : false )
    , position_ ( asn.location )
    , diplomacy_( asn.diplomacy )
    , formation_( asn.formation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceOrder destructor
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
IntelligenceOrder::~IntelligenceOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceOrder::Send
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
void IntelligenceOrder::Send( ASN1T_Intelligence& asn ) const
{
    asn.name      = name_.c_str();
    asn.nature    = nature_.c_str();
    asn.level     = level_;
    asn.embarked  = embarked_ ? 1 : 0;
    asn.location  = position_;
    asn.diplomacy = diplomacy_;
    asn.formation = formation_;
}
