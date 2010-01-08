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
#include "urban/TerrainObject_ABC.h"
#include "urban/Block.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const kernel::OrderParameter& parameter, unsigned long value )
: Parameter< unsigned long >( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const kernel::OrderParameter& parameter, const ASN1T_UrbanBlock& asn )
: Parameter< unsigned long >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, xml::xistream& xis )
: Parameter< unsigned long >( parameter, attribute< unsigned long >( xis, "value" ) )
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
void UrbanBlock::CommitTo(  ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_urbanBlock;
    if( IsSet() )
        asn.value.u.urbanBlock = GetValue();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Serialize
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
void UrbanBlock::Serialize( xml::xostream& xos ) const
{
    Parameter< unsigned long >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Display
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
void UrbanBlock::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Parameter", "Action" ) ).Display( GetName() )
        .Item( tools::translate( "Parameter", "Value" ) ).Display( GetValue() ); // $$$$ MGD ask to -> SBO 2007-05-24: make a displayer
}
