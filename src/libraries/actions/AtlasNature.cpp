// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AtlasNature.h"
#include "clients_kernel/AtlasNatures.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature( const kernel::OrderParameter& parameter, const kernel::AtlasNature& nature )
    : Parameter< kernel::AtlasNature >( parameter, nature )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature( const kernel::OrderParameter& parameter, const Common::MsgAtlasNature& message, const kernel::AtlasNatures& natures )
    : Parameter< kernel::AtlasNature >( parameter, natures.MakeNature( message.nature() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::AtlasNatures& natures )
    : Parameter< kernel::AtlasNature >( parameter, natures.MakeNature( attribute< unsigned short >( xis, "value" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::~AtlasNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AtlasNature::Serialize( xml::xostream& xos ) const
{
    Parameter< kernel::AtlasNature >::Serialize( xos );
    xos << attribute( "value", GetValue().GetValue() );
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::CommitTo
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void AtlasNature::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    message.mutable_value()->mutable_atlasnature(); // enforce initialisation of parameter to force his type
    if( IsSet() )
        message.mutable_value()->mutable_atlasnature()->set_nature( GetValue().GetValue() );
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::Clean
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void AtlasNature::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_atlasnature() )
        message.mutable_value()->clear_atlasnature();
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::Display
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AtlasNature::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
             .Item( tools::translate( "Parameter", "Value" ) ).Display( GetValue().GetName() ); // $$$$ SBO 2007-05-24: make a displayer
}
