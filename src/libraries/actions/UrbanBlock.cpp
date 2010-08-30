// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "UrbanBlock.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const kernel::OrderParameter& parameter, unsigned long id )
    : Parameter< unsigned long >( parameter, id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const kernel::OrderParameter& parameter, const Common::MsgUrbanBlock& /*message*/ )
    : Parameter< unsigned long >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< unsigned long >( parameter, xis.attribute< unsigned long >( "value" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock destructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::~UrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        message.mutable_value()->mutable_urbanblock()->set_id( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Serialize
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
void UrbanBlock::Serialize( xml::xostream& xos ) const
{
    Parameter< unsigned long >::Serialize( xos );
    xos << xml::attribute( "value", GetValue() );
}
