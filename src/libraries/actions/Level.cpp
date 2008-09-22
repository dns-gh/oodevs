// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Level.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Level constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Level::Level( const kernel::OrderParameter& parameter, const kernel::HierarchyLevel_ABC& level )
    : Parameter< QString >( parameter, level.GetName() )
    , level_( level )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Level constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Level::Level( const kernel::OrderParameter& parameter, const ASN1T_EnumNatureLevel& asn, const kernel::FormationLevels& levels )
    : Parameter< QString >( parameter )
    , level_( *levels.Resolve( asn ) )
{
    SetValue( level_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Level constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
Level::Level( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::FormationLevels& levels )
    : Parameter< QString >( parameter )
    , level_( *levels.Resolve( attribute< std::string >( xis, "value" ).c_str() ) )
{
    SetValue( level_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Level destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
Level::~Level()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Level::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Level::CommitTo( ASN1T_EnumNatureLevel& asn ) const
{
    asn = ASN1T_EnumNatureLevel( level_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Level::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void Level::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << attribute( "value", level_.GetName().ascii() );
}
