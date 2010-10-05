// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_Serialization.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"
#include "tools/MIL_Tools.h"
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <hla/ObjectIdentifier.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
Serializer& hla::operator<<( Serializer& serializer, const MT_Vector2D& v )
{
    return serializer << v.rX_ << v.rY_;
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
Deserializer& hla::operator>>( Deserializer& deserializer, MT_Vector2D& v )
{
    return deserializer >> v.rX_ >> v.rY_;
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
Serializer& hla::operator<<( Serializer& serializer, const ObjectIdentifier& id )
{
    return serializer << id.ToLong();
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
Deserializer& hla::operator>>( Deserializer& deserializer, ObjectIdentifier& id )
{
    unsigned long nId;
    deserializer >> nId;
    id = ObjectIdentifier( nId );
    return deserializer;
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
Serializer& hla::operator<<( Serializer& serializer, const TER_Localisation& localisation )
{
    serializer << TER_Localisation::ConvertLocalisationType( localisation.GetType() );
    std::vector< std::string > coordinates; coordinates.reserve( localisation.GetPoints().size() );
    for( CIT_PointVector it = localisation.GetPoints().begin(); it != localisation.GetPoints().end(); ++it )
        coordinates.push_back( MIL_Tools::ConvertCoordSimToMos( *it ) );
    return serializer << coordinates;
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
Deserializer& hla::operator>>( Deserializer& deserializer, TER_Localisation& localisation )
{
    std::string strLocalisationType;
    deserializer >> strLocalisationType;
    TER_Localisation::E_LocationType eType = TER_Localisation::ConvertLocalisationType( strLocalisationType );
    if( eType == TER_Localisation::eNone )
        return deserializer;

    std::vector< std::string > coordinates;
    deserializer >> coordinates;
    T_PointVector points; points.reserve( coordinates.size() );
    for( std::vector< std::string >::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it )
    {
        MT_Vector2D vPosition;
        MIL_Tools::ConvertCoordMosToSim( *it, vPosition );
        TER_World::GetWorld().ClipPointInsideWorld( vPosition );
        points.push_back( vPosition );
    }
    localisation.Reset( eType, points );
    return deserializer;
}
