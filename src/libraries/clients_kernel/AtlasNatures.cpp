// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AtlasNatures.h"
#include "AtlasNature.h"
#include "Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AtlasNatures constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNatures::AtlasNatures()
{
    AddField( tools::translate( "AtlasNature", "Armored" )        , sword::Nature::tank );
    AddField( tools::translate( "AtlasNature", "SSA" )            , sword::Nature::missile_launcher );
    AddField( tools::translate( "AtlasNature", "Command post" )   , sword::Nature::command_post );
    AddField( tools::translate( "AtlasNature", "Logistic" )       , sword::Nature::logistics );
    AddField( tools::translate( "AtlasNature", "Mortar" )         , sword::Nature::mortar );
    AddField( tools::translate( "AtlasNature", "Rocket launcher" ), sword::Nature::rocket_launcher );
    AddField( tools::translate( "AtlasNature", "Vehicle" )        , sword::Nature::vehicle );
    AddField( tools::translate( "AtlasNature", "Footman")         , sword::Nature::infantry );
    AddField( tools::translate( "AtlasNature", "Helicopter" )     , sword::Nature::helicopter );
    AddField( tools::translate( "AtlasNature", "SAA" )            , sword::Nature::anti_aircraft );
}

// -----------------------------------------------------------------------------
// Name: AtlasNatures destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNatures::~AtlasNatures()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AtlasNatures::AddField
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AtlasNatures::AddField( const QString& name, unsigned short value )
{
    AtlasNature* nature = new AtlasNature( name, value );
    Register( Count(), *nature );
}

// -----------------------------------------------------------------------------
// Name: AtlasNatures::MakeNature
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature AtlasNatures::MakeNature( unsigned short value ) const
{
    AtlasNature nature;
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->IsSet( value ) )
            nature.Toggle( *it->second );
    return nature;

}
