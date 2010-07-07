// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __DEC_FunctionsTools_h_
#define __DEC_FunctionsTools_h_

#include "MIL.h"

class MIL_Object_ABC;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_FunctionsTools
{
public:
    //! @name Functions
    //@{
    static MIL_Object_ABC* GetPopulationKnowledgeObjectFromDia( unsigned int nID );
    //@}
};

#endif // __DEC_FunctionsTools_h_
