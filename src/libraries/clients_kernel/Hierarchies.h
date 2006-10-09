// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Hierarchies_h_
#define __Hierarchies_h_

#include "Iterator.h"
#include "Extension_ABC.h"

namespace kernel
{
    class Entity_ABC;

// =============================================================================
/** @class  Hierarchies
    @brief  Hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
class Hierarchies : public Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Hierarchies() {};
    virtual ~Hierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetSuperior() const = 0;
    virtual const Entity_ABC& GetEntity() const = 0;
    virtual Iterator< const Entity_ABC& > CreateSubordinateIterator() const = 0; 

    virtual void AddSubordinate   (       Entity_ABC& entity ) = 0;
    virtual void RemoveSubordinate( const Entity_ABC& entity ) = 0;
    virtual void UnregisterSubordinate( const Entity_ABC& entity ) = 0;

    virtual bool IsSubordinateOf( const Entity_ABC& entity ) const = 0;
    virtual const Entity_ABC& GetTop() const = 0;
    virtual const Entity_ABC& GetUp( unsigned int nLevel = 1 ) const = 0;
    //@}
};

}

#endif // __Hierarchies_h_
