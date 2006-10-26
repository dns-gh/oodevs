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

    //! @name Accessors
    //@{
    virtual const Entity_ABC* GetSuperior() const;
    virtual const Entity_ABC& GetEntity() const;
    virtual Iterator< const Entity_ABC& > CreateSubordinateIterator() const; 
    //@}

    //! @name Modifiers
    //@{
    virtual void AddSubordinate       (       Entity_ABC& entity );
    virtual void RemoveSubordinate    ( const Entity_ABC& entity );
    virtual void UnregisterSubordinate( const Entity_ABC& entity );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const Hierarchies* RetrieveHierarchies( const Entity_ABC& entity ) const;
    Interface* SuperiorHierarchy();
    void SetSuperior( Entity_ABC* superior );
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
