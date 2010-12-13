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
    AddField( tools::translate( "AtlasNature", "Armored" )        , sword::MsgAtlasNature::blinde );
    AddField( tools::translate( "AtlasNature", "SSA" )            , sword::MsgAtlasNature::ass );
    AddField( tools::translate( "AtlasNature", "Command post" )   , sword::MsgAtlasNature::pc );
    AddField( tools::translate( "AtlasNature", "Logistic" )       , sword::MsgAtlasNature::log );
    AddField( tools::translate( "AtlasNature", "Mortar" )         , sword::MsgAtlasNature::mortier );
    AddField( tools::translate( "AtlasNature", "Rocket launcher" ), sword::MsgAtlasNature::lance_roquette );
    AddField( tools::translate( "AtlasNature", "Vehicle" )        , sword::MsgAtlasNature::vehicule );
    AddField( tools::translate( "AtlasNature", "Footman")         , sword::MsgAtlasNature::personnel_a_pied );
    AddField( tools::translate( "AtlasNature", "Helicopter" )     , sword::MsgAtlasNature::helicoptere );
    AddField( tools::translate( "AtlasNature", "SAA" )            , sword::MsgAtlasNature::asa );
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
