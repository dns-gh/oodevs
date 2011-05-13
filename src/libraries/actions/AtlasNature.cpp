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
#include <xeumeuleu/xml.hpp>

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
AtlasNature::AtlasNature( const kernel::OrderParameter& parameter, const sword::Nature& message, const kernel::AtlasNatures& natures )
    : Parameter< kernel::AtlasNature >( parameter, natures.MakeNature( static_cast< unsigned short >( message.flags() ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::AtlasNatures& natures )
    : Parameter< kernel::AtlasNature >( parameter, natures.MakeNature( xis.attribute< unsigned short >( "value" ) ) )
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
    xos << xml::attribute( "value", GetValue().GetValue() );
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::CommitTo
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void AtlasNature::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.add_value()->mutable_nature()->set_flags( GetValue().GetValue() );
}
// -----------------------------------------------------------------------------
// Name: AtlasNature::CommitTo
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void AtlasNature::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.mutable_nature()->set_flags( GetValue().GetValue() );
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

// -----------------------------------------------------------------------------
// Name: AtlasNature::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string AtlasNature::SerializeType() const
{
    return "natureatlas";
}
