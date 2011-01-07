// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef hla_SerializationTools_h
#define hla_SerializationTools_h

#include <hla/Serializer.h>

namespace hla
{
    template< unsigned N >
    Serializer& operator<<( Serializer& serializer, const unsigned char (&buffer)[N] )
    {
        for( unsigned i = 0; i < N; ++i )
            serializer << buffer[i];
        return serializer;
    }
}

#endif // hla_SerializationTools_h
