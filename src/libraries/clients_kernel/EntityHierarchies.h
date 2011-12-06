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
#include "tools/Resolver.h"
#include "Creatable.h"

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
class EntityHierarchies : public Interface
                        , public tools::Resolver< Entity_ABC >
                        , public Creatable< Interface >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior );
    virtual ~EntityHierarchies();
    //@}

    //! @name Accessors
    //@{
    virtual const Entity_ABC* GetSuperior() const;
    virtual const Entity_ABC& GetEntity() const;
    virtual tools::Iterator< const Entity_ABC& > CreateSubordinateIterator() const;
	virtual long CountSubordinates() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void RegisterSubordinate  (       Entity_ABC& entity );
    virtual void AddSubordinate       (       Entity_ABC& entity );
    virtual void RemoveSubordinate    ( const Entity_ABC& entity );
    virtual void UnregisterSubordinate( const Entity_ABC& entity );
    virtual void UnregisterParent     ();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const Hierarchies* RetrieveHierarchies( const Entity_ABC& entity ) const;
    Interface* SuperiorHierarchy();
    void ChangeSuperior( Entity_ABC* superior );
    void SetSuperior( Entity_ABC* superior );
    virtual void SetSuperiorInternal( Entity_ABC* superior );
    virtual void CreateDictionary( PropertiesDictionary& dico ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityHierarchies( const EntityHierarchies& );            //!< Copy constructor
    EntityHierarchies& operator=( const EntityHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Entity_ABC& entity_;
    const Entity_ABC* superior_;
    //@}
};

}

#include "EntityHierarchies.inl"

#endif // __kernel_EntityHierarchies_h_
