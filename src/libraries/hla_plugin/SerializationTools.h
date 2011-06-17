// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_SerializationTools_h
#define plugins_hla_SerializationTools_h

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

#endif // plugins_hla_SerializationTools_h
