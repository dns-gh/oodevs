// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLevel.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterLevel constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterLevel::ActionParameterLevel( const kernel::OrderParameter& parameter, const kernel::HierarchyLevel_ABC& level )
    : ActionParameter< QString >( parameter, level.GetName() )
    , level_( level )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLevel constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterLevel::ActionParameterLevel( const OrderParameter& parameter, const ASN1T_EnumNatureLevel& asn, const FormationLevels& levels )
    : ActionParameter< QString >( parameter )
    , level_( *levels.Resolve( asn ) )
{
    SetValue( level_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLevel constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterLevel::ActionParameterLevel( const OrderParameter& parameter, xml::xistream& xis, const FormationLevels& levels )
    : ActionParameter< QString >( parameter )
    , level_( *levels.Resolve( attribute< std::string >( xis, "value" ).c_str() ) )
{
    SetValue( level_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLevel destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterLevel::~ActionParameterLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLevel::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterLevel::CommitTo( ASN1T_EnumNatureLevel& asn ) const
{
    asn = ASN1T_EnumNatureLevel( level_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLevel::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void ActionParameterLevel::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", level_.GetName().ascii() );
}
