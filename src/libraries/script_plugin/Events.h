// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Events_h_
#define __Events_h_

namespace plugins
{
namespace script
{

#define DECLARE_EVENT( name, type, member )         \
    struct name                                     \
    {                                               \
        name( type data )                           \
            : member( data ) {}                     \
        type member;                                \
    private:                                        \
        name& operator=( const name& );             \
    }

#define DECLARE_EVENT2( name, type1, member1, type2, member2 )         \
    struct name                                                        \
    {                                                                  \
        name( type1 data1, type2 data2 )                               \
            : member1( data1 )                                         \
            , member2( data2 ) {}                                      \
        type1 member1;                                                 \
        type2 member2;                                                 \
    private:                                                           \
        name& operator=( const name& );                                \
    }

}
}

#endif // __Events_h_
