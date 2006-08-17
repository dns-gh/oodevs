// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Serialization.cpp $
// $Author: Age $
// $Modtime: 10/12/04 10:33 $
// $Revision: 6 $
// $Workfile: HLA_Serialization.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_Serialization.h"
#include "hla/Serializer.h"
#include "hla/Deserializer.h"
#include "hla/ObjectIdentifier.h"
#include "TER/TER_Localisation.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
Serializer& operator<<( Serializer& serializer, const MT_Vector2D& v )
{
    return serializer << v.rX_ << v.rY_;
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
Deserializer& operator>>( Deserializer& deserializer, MT_Vector2D& v )
{
    return deserializer >> v.rX_ >> v.rY_;
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
Serializer& operator<<( Serializer& serializer, const ObjectIdentifier& id )
{
    return serializer << id.ToLong();
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
Deserializer& operator>>( Deserializer& deserializer, ObjectIdentifier& id )
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
Serializer& operator<<( Serializer& serializer, const TER_Localisation& localisation )
{
    serializer << TER_Localisation::ConvertLocalisationType( localisation.GetType() );
    std::vector< std::string > coordinates; coordinates.reserve( localisation.GetPoints().size() );
    for( CIT_PointVector it = localisation.GetPoints().begin(); it != localisation.GetPoints().end(); ++it )
    {
        std::string strCoordinate;
        MIL_Tools::ConvertCoordSimToMos( *it, strCoordinate );
        coordinates.push_back( strCoordinate );
    }
    return serializer << coordinates;
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
Deserializer& operator>>( Deserializer& deserializer, TER_Localisation& localisation )
{
    std::string strLocalisationType;
    deserializer >> strLocalisationType;
    TER_Localisation::E_TypeLocalisation eType = TER_Localisation::ConvertLocalisationType( strLocalisationType );
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
