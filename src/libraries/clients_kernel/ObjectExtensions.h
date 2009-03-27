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
    DECLARE_EXTENSION( ConstructionAttribute_ABC )
    DECLARE_EXTENSION( MineAttribute_ABC )
    DECLARE_EXTENSION( ActivityTimeAttribute_ABC )
    DECLARE_EXTENSION( NBCAttribute_ABC )
    DECLARE_EXTENSION( BypassAttribute_ABC )
    DECLARE_EXTENSION( ObstacleAttribute_ABC )
    DECLARE_EXTENSION( LogisticAttribute_ABC )
    DECLARE_EXTENSION( CrossingSiteAttribute_ABC )
    DECLARE_EXTENSION( SupplyRouteAttribute_ABC )
    DECLARE_EXTENSION( ToxicCloudAttribute_ABC )
    DECLARE_EXTENSION( FireAttribute_ABC )
    DECLARE_EXTENSION( MedicalTreatmentAttribute_ABC )    
}

#undef DECLARE_EXTENSION

#endif // __AgentExtensions_h_
