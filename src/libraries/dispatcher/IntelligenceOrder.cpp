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
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: IntelligenceOrder constructor
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
IntelligenceOrder::IntelligenceOrder( const Common::MsgIntelligence& asn )
    : name_     ( asn.name() )
    , nature_   ( asn.nature() )
    , level_    ( asn.level() )
    , embarked_ ( asn.embarked() )
    , position_ ( asn.location() )
    , diplomacy_( asn.diplomacy() )
    , formation_( asn.formation().oid() )
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
void IntelligenceOrder::Send( Common::MsgIntelligence& message ) const
{
    message.set_diplomacy( diplomacy_ );
    message.set_embarked( embarked_ );
    message.set_level( level_ );
    message.mutable_formation()->set_oid( formation_ );
    message.set_name( name_ );
    message.set_nature( nature_ );
    *message.mutable_location() = position_;
}
