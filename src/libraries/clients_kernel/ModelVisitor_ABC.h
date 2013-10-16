// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelVisitor_ABC_h_
#define __ModelVisitor_ABC_h_

#include <tools/SortedInterfaceContainer.h>

namespace kernel
{
    class EntityBase_ABC;

// =============================================================================
/** @class  ModelVisitor_ABC
    @brief  ModelVisitor_ABC
*/
// Created: AGE 2007-04-12
// =============================================================================
class ModelVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelVisitor_ABC() {};
    virtual ~ModelVisitor_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const EntityBase_ABC& entity ) = 0;
    //@}
};

}

#endif // __ModelVisitor_ABC_h_
