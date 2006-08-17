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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Serialization.h $
// $Author: Age $
// $Modtime: 10/12/04 11:32 $
// $Revision: 6 $
// $Workfile: HLA_Serialization.h $
//
// *****************************************************************************

#ifndef __HLA_Serialization_h_
#define __HLA_Serialization_h_

class Serializer;
class Deserializer;
class ObjectIdentifier;
class TER_Localisation;

// =============================================================================
/** @brief  HLA serialization helpers
*/
// Created: AGE 2004-11-10
// =============================================================================

Serializer&   operator<<( Serializer&   serializer,   const MT_Vector2D& v );
Deserializer& operator>>( Deserializer& deserializer,       MT_Vector2D& v );

Serializer&   operator<<( Serializer&   serializer,   const ObjectIdentifier& id );
Deserializer& operator>>( Deserializer& deserializer,       ObjectIdentifier& id );

Serializer&   operator<<( Serializer&   serializer,   const TER_Localisation& localisation );
Deserializer& operator>>( Deserializer& deserializer,       TER_Localisation& localisation );

template< typename Type >
Serializer& operator<<( Serializer& serializer, const std::vector< Type >& v )
{
    serializer << uint32( v.size() );
    for( std::vector< Type >::const_iterator it = v.begin(); it != v.end(); ++it )
        serializer << *it;
    return serializer;
}

template< typename Type >
Deserializer& operator>>( Deserializer& deserializer, std::vector< Type >& v )
{
    uint32 nSize;
    deserializer >> nSize;
    v.reserve( nSize );
    while( nSize-- > 0 )
    {
        v.push_back( Type() );
        deserializer >> v.back();
    }
    return deserializer;
}

template< typename Type1, typename Type2 >
Serializer& operator<<( Serializer& serializer, const std::pair< Type1, Type2 >& p )
{
    serializer << p.first;
    serializer << p.second;
    return serializer;
}

template< typename Type1, typename Type2 >
Deserializer& operator>>( Deserializer& deserializer, std::pair< Type1, Type2 >& p )
{
    deserializer >> p.first;
    deserializer >> p.second;
    return deserializer;
}

#endif // __HLA_Serialization_h_
