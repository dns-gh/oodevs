// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentExtensions_h_
#define __AgentExtensions_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"

#define DECLARE_EXTENSION( extension )                                          \
class extension : public kernel::Extension_ABC, public kernel::Displayable_ABC  \
{                                                                               \
public:                                                                         \
             extension() {};                                                    \
    virtual ~extension() {};                                                    \
};

namespace kernel
{
    DECLARE_EXTENSION( CampAttributes_ABC )
    DECLARE_EXTENSION( CrossingSiteAttributes_ABC )
    DECLARE_EXTENSION( LogisticRouteAttributes_ABC )
    DECLARE_EXTENSION( NBCAttributes_ABC )
    DECLARE_EXTENSION( RotaAttributes_ABC )
    DECLARE_EXTENSION( MineAttributes_ABC )
}

#undef DECLARE_EXTENSION

#endif // __AgentExtensions_h_
