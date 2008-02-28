// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AtlasNatures.h"
#include "AtlasNature.h"
#include "game_asn/Simulation.h"
#include "tools.h"

// -----------------------------------------------------------------------------
// Name: AtlasNatures constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNatures::AtlasNatures()
{
    AddField( tools::translate( "AtlasNature", "Armored" )        , BytXblinde          , BitMblinde );
    AddField( tools::translate( "AtlasNature", "SSA" )            , BytXass             , BitMass );
    AddField( tools::translate( "AtlasNature", "Command post" )   , BytXpc              , BitMpc );
    AddField( tools::translate( "AtlasNature", "Logistic" )       , BytXlog             , BitMlog );
    AddField( tools::translate( "AtlasNature", "Mortar" )         , BytXmortier         , BitMmortier );
    AddField( tools::translate( "AtlasNature", "Rocket launcher" ), BytXlance_roquette  , BitMlance_roquette );
    AddField( tools::translate( "AtlasNature", "Vehicle" )        , BytXvehicule        , BitMvehicule );
    AddField( tools::translate( "AtlasNature", "Footman")         , BytXpersonnel_a_pied, BitMpersonnel_a_pied );
    AddField( tools::translate( "AtlasNature", "Helicopter" )     , BytXhelicoptere     , BitMhelicoptere );
    AddField( tools::translate( "AtlasNature", "Undefined" )      , BytXindefini        , BitMindefini );
    AddField( tools::translate( "AtlasNature", "None" )           , BytXnone            , BitMnone );
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
void AtlasNatures::AddField( const QString& name, unsigned char byte, unsigned char mask )
{
    AtlasNature* nature = new AtlasNature( name, byte ? mask : ( mask << 8 ) );
    Register( Count(), *nature );
}

// -----------------------------------------------------------------------------
// Name: AtlasNatures::MakeNature
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AtlasNature AtlasNatures::MakeNature( unsigned short value ) const
{
    AtlasNature nature;
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->IsSet( value ) )
            nature.Toggle( *it->second );
    return nature;

}
