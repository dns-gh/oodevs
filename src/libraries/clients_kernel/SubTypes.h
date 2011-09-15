// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SubTypes_h_
#define __SubTypes_h_

#include "StrongType.h"

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
    class Karma;
    class LogisticLevel;

    #define DECLARE_SUBTYPE( name, base ) \
    struct name##_{};\
    typedef StrongType< base, name##_ > name;

    DECLARE_SUBTYPE( LogisticBaseSuperior, const Entity_ABC* );

    DECLARE_SUBTYPE( TeamKarma, const Karma* );
    DECLARE_SUBTYPE( IntelligenceKarma, const Karma* );

    DECLARE_SUBTYPE( EntityLogisticLevel, const LogisticLevel* );
}

#endif // __SubTypes_h_
