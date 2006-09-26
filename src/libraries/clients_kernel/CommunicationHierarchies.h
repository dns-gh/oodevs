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
class CommunicationHierarchies : public EntityHierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CommunicationHierarchies( Controller& controller );
    virtual ~CommunicationHierarchies();
    //@}

    //! @name Operations
    //@{
    bool IsSubordinateOf( const Entity_ABC& entity ) const;
    virtual void AddSubordinate   (       Entity_ABC& entity );
    virtual void RemoveSubordinate( const Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    //@}
};

};

#endif // __Kernel_CommunicationHierarchies_h_
