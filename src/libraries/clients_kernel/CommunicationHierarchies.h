// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Kernel_CommunicationHierarchies_h_
#define __Kernel_CommunicationHierarchies_h_

#include "EntityHierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  Communications
    @brief  Communications
*/
// Created: SBO 2006-09-25
// =============================================================================
class CommunicationHierarchies : public Hierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
             CommunicationHierarchies() {};
    virtual ~CommunicationHierarchies() {};
    //@}
};

};

#endif // __Kernel_CommunicationHierarchies_h_
