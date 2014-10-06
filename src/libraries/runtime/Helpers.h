// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef RUNTIME_HELPERS_H
#define RUNTIME_HELPERS_H

#define COUNT_OF( X ) ( sizeof( X ) / sizeof *( X ) )

#ifdef _MSC_VER
#define ONCE __pragma( warning( push ) )\
             __pragma( warning( disable: 4127 ) )\
             while( 0 ) \
             __pragma( warning( pop ) )
#else
#define ONCE while( 0 )
#endif

#endif // RUNTIME_HELPERS_H
