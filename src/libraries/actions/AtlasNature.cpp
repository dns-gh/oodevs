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

namespace
{
    unsigned short Shortify( const unsigned char data[2] )
    {
        return ( data[0] << 8 ) + data[1];
    }
}

// -----------------------------------------------------------------------------
// Name: AtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature::AtlasNature( const kernel::OrderParameter& parameter, const ASN1T_AtlasNature& asn, const kernel::AtlasNatures& natures )
    : Parameter< kernel::AtlasNature >( parameter, natures.MakeNature( Shortify( asn.data ) ) )
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
void AtlasNature::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_atlasNature;
    asn.value.u.atlasNature = new ASN1T_AtlasNature();
    asn.value.u.atlasNature->numbits = 11;
    asn.value.u.atlasNature->data[0] = 0;
    asn.value.u.atlasNature->data[1] = 0;
    if( IsSet() )
        GetValue().CommitTo( asn.value.u.atlasNature->numbits, asn.value.u.atlasNature->data );
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::Clean
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void AtlasNature::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.atlasNature;
}

// -----------------------------------------------------------------------------
// Name: AtlasNature::Display
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AtlasNature::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Parameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "Parameter", "Value" ) ).Display( GetValue().GetName() ); // $$$$ SBO 2007-05-24: make a displayer
}
