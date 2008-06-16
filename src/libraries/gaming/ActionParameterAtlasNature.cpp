// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAtlasNature.h"
#include "AtlasNatures.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAtlasNature::ActionParameterAtlasNature( const OrderParameter& parameter, const AtlasNature& nature )
    : ActionParameter< AtlasNature >( parameter, nature )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAtlasNature::ActionParameterAtlasNature( const OrderParameter& parameter, const ASN1T_AtlasNature& asn, const AtlasNatures& natures )
    : ActionParameter< AtlasNature >( parameter, natures.MakeNature( *(unsigned short*)( asn.data ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAtlasNature::ActionParameterAtlasNature( const OrderParameter& parameter, xml::xistream& xis, const AtlasNatures& natures )
    : ActionParameter< AtlasNature >( parameter, natures.MakeNature( attribute< unsigned short >( xis, "value" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAtlasNature::~ActionParameterAtlasNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAtlasNature::Serialize( xml::xostream& xos ) const
{
    ActionParameter< AtlasNature >::Serialize( xos );
    xos << attribute( "value", GetValue().GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature::CommitTo
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void ActionParameterAtlasNature::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_atlasNature;
    asn.value.u.atlasNature = new ASN1T_AtlasNature();
    if( IsSet() )
        GetValue().CommitTo( *asn.value.u.atlasNature );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature::Clean
// Created: SBO 2008-06-16
// -----------------------------------------------------------------------------
void ActionParameterAtlasNature::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.atlasNature;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAtlasNature::Display
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAtlasNature::Display( Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ActionParameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "ActionParameter", "Value" ) ).Display( GetValue().GetName() ); // $$$$ SBO 2007-05-24: make a displayer
}
