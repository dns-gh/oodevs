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
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Level constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Level::Level( const kernel::OrderParameter& parameter, E_NatureLevel level )
    : Parameter< QString >( parameter, ENT_Tr::ConvertFromNatureLevel( level ).c_str() )
    , level_( level )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Level constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Level::Level( const kernel::OrderParameter& parameter, const sword::EnumNatureLevel& message )
    : Parameter< QString >( parameter )
    , level_( static_cast< E_NatureLevel >( message ) )
{
    SetValue( ENT_Tr::ConvertFromNatureLevel( level_ ).c_str() );
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
void Level::CommitTo( T_Setter setter ) const
{
    setter( sword::EnumNatureLevel( level_ ) );
}

// -----------------------------------------------------------------------------
// Name: Level::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void Level::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << xml::attribute( "value", ENT_Tr::ConvertFromNatureLevel( level_ ) );
}

// -----------------------------------------------------------------------------
// Name: Level::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Level::SerializeType() const
{
    return "level";
}
