// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityHierarchies_h_
#define __EntityHierarchies_h_

#include "Hierarchies.h"
#include "Resolver.h"

namespace kernel
{

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
             EntityHierarchies();
    virtual ~EntityHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetSuperior() const;
    virtual Iterator< const Entity_ABC& > CreateSubordinateIterator() const; 

    bool IsSubordinateOf( const Entity_ABC& entity ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityHierarchies( const EntityHierarchies& );            //!< Copy constructor
    EntityHierarchies& operator=( const EntityHierarchies& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    const Entity_ABC* superior_;
    //@}
};

}

#endif // __EntityHierarchies_h_
