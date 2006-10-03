// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_EntityHierarchies_h_
#define __kernel_EntityHierarchies_h_

#include "Hierarchies.h"
#include "Resolver.h"

namespace kernel
{
    class Controller;

// =============================================================================
/** @class  EntityHierarchies
    @brief  Entity hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
class EntityHierarchies : public Hierarchies, public Resolver< Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityHierarchies( Controller& controller );
    virtual ~EntityHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetSuperior() const = 0;
    virtual const Entity_ABC& GetEntity() const = 0;

    virtual Iterator< const Entity_ABC& > CreateSubordinateIterator() const; 

    bool IsSubordinateOf( const Entity_ABC& entity ) const;

    virtual void AddSubordinate   (       Entity_ABC& entity );
    virtual void RemoveSubordinate( const Entity_ABC& entity );
    virtual void UnregisterSubordinate( const Entity_ABC& entity );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityHierarchies( const EntityHierarchies& );            //!< Copy constructor
    EntityHierarchies& operator=( const EntityHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    //@}
};

}

#endif // __kernel_EntityHierarchies_h_
