// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PropagationCapacity_ABC_h_
#define __PropagationCapacity_ABC_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

// =============================================================================
/** @class  PropagationCapacity_ABC
    @brief  PropagationCapacity_ABC
*/
// Created: JCR 2008-06-02
// =============================================================================
class PropagationCapacity_ABC : public ObjectCapacity_ABC
                              , public MIL_InteractiveContainer_ABC
{
    public:
    virtual ~PropagationCapacity_ABC() {}
};

#endif // PropagationCapacity_ABC
