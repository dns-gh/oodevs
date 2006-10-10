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

#include "Hierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  CommunicationHierarchies
    @brief  Communication hierarchies
            agent -> automat -> knowledge group -> team
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
