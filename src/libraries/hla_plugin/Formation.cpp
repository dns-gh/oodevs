// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Formation.h"
#include "SerializationTools.h"
#include "hla/UpdateFunctor_ABC.h"
#include "hla/AttributeIdentifier.h"
#include "pathfind/TerrainData.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
Formation::Formation()
    : onRoad_ ( false )
    , changed_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::Serialize
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void Formation::Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const
{
    if( bUpdateAll || changed_ )
    {
        long value = onRoad_ ? 5 : 1; // column, assembly
        Serializer serializer;
        serializer << value;
        functor.Visit( AttributeIdentifier( "Formation" ), serializer );
        changed_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::IsOnRoad
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
bool Formation::IsOnRoad( const ASN1T_MsgUnitEnvironmentType& message )
{
    const unsigned int mask = TerrainData::motorway_  | TerrainData::largeroad_  | TerrainData::mediumroad_
                            | TerrainData::smallroad_ | TerrainData::bridge_;
    return ( message.linear & mask ) != 0;
}

// -----------------------------------------------------------------------------
// Name: Formation::Update
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void Formation::Update( const ASN1T_MsgUnitEnvironmentType& message )
{
    bool onRoad = IsOnRoad( message );
    changed_ = onRoad != onRoad_;
    onRoad_ = onRoad;
}
