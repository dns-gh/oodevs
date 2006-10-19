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
    class PropertiesDictionary;

// =============================================================================
/** @class  EntityHierarchies
    @brief  Entity hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
template< typename Interface >
class EntityHierarchies : public Interface, public Resolver< Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityHierarchies( Controller& controller, Entity_ABC& entity );
    virtual ~EntityHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetSuperior() const;
    virtual const Entity_ABC& GetEntity() const;
    virtual Iterator< const Entity_ABC& > CreateSubordinateIterator() const; 

    virtual void AddSubordinate       (       Entity_ABC& entity );
    virtual void RemoveSubordinate    ( const Entity_ABC& entity );
    virtual void UnregisterSubordinate( const Entity_ABC& entity );

    bool IsSubordinateOf( const Entity_ABC& entity ) const;
    virtual const Entity_ABC& GetTop() const;
    virtual const Entity_ABC& GetUp( unsigned int nLevel = 1 ) const;
    //@}

protected:
    //! @name Modifiers
    //@{
    Interface* SuperiorHierarchy();
    void SetSuperior( Entity_ABC* superior );
    Entity_ABC* GetSuperior();
    Entity_ABC& GetEntity();
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
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
    Entity_ABC& entity_;
    Entity_ABC* superior_;
    //@}
};

}

#include "EntityHierarchies.inl"

#endif // __kernel_EntityHierarchies_h_
