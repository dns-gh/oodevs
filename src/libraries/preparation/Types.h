// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Preparation_Types_h_
#define __Preparation_Types_h_

#include "clients_kernel/StrongType.h"

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
    class Karma;
    class LogisticLevel;
}

#define DECLARE_SUBTYPE( name, base ) \
struct name##_{};\
typedef kernel::StrongType< base, name##_ > name;

DECLARE_SUBTYPE( LogisticBaseSuperior, const kernel::Entity_ABC* );

DECLARE_SUBTYPE( TeamKarma, const kernel::Karma* );
DECLARE_SUBTYPE( IntelligenceKarma, const kernel::Karma* );

DECLARE_SUBTYPE( EntityLogisticLevel, const kernel::LogisticLevel* );

#endif // __Preparation_Types_h_
