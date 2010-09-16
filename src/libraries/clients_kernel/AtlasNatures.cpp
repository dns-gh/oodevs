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
    AddField( tools::translate( "AtlasNature", "Armored" )        , Common::MsgAtlasNature::blinde );
    AddField( tools::translate( "AtlasNature", "SSA" )            , Common::MsgAtlasNature::ass );
    AddField( tools::translate( "AtlasNature", "Command post" )   , Common::MsgAtlasNature::pc );
    AddField( tools::translate( "AtlasNature", "Logistic" )       , Common::MsgAtlasNature::log );
    AddField( tools::translate( "AtlasNature", "Mortar" )         , Common::MsgAtlasNature::mortier );
    AddField( tools::translate( "AtlasNature", "Rocket launcher" ), Common::MsgAtlasNature::lance_roquette );
    AddField( tools::translate( "AtlasNature", "Vehicle" )        , Common::MsgAtlasNature::vehicule );
    AddField( tools::translate( "AtlasNature", "Footman")         , Common::MsgAtlasNature::personnel_a_pied );
    AddField( tools::translate( "AtlasNature", "Helicopter" )     , Common::MsgAtlasNature::helicoptere );
    AddField( tools::translate( "AtlasNature", "SAA" )            , Common::MsgAtlasNature::asa );
    AddField( tools::translate( "AtlasNature", "Undefined" )      , Common::MsgAtlasNature::indefini );
    AddField( tools::translate( "AtlasNature", "None" )           , Common::MsgAtlasNature::none );
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
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->IsSet( value ) )
            nature.Toggle( *it->second );
    return nature;

}
