// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_EntityHierarchies_h_
#define __gui_EntityHierarchies_h_

#include "PropertiesDictionary.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Creatable.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
}

namespace gui
{
// =============================================================================
/** @class  EntityHierarchies
    @brief  Entity hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
template< typename Interface >
class EntityHierarchies : public Interface
                        , public tools::Resolver< kernel::Entity_ABC >
                        , public kernel::Creatable< Interface >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::Entity_ABC* superior );
    virtual ~EntityHierarchies();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual const kernel::Entity_ABC& GetEntity() const;
    virtual tools::Iterator< const kernel::Entity_ABC& > CreateSubordinateIterator() const;
    virtual long CountSubordinates() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void RegisterSubordinate  (       kernel::Entity_ABC& entity );
    virtual void AddSubordinate       (       kernel::Entity_ABC& entity );
    virtual void RemoveSubordinate    ( const kernel::Entity_ABC& entity );
    virtual void UnregisterSubordinate( const kernel::Entity_ABC& entity );
    virtual void UnregisterParent     ();
    virtual void NotifySuperiorChanged( const kernel::Entity_ABC* /*newSuperior*/ ) {}
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const kernel::Hierarchies* RetrieveHierarchies( const kernel::Entity_ABC& entity ) const;
    Interface* SuperiorHierarchy();
    void ChangeSuperior( kernel::Entity_ABC* superior );
    void SetSuperior( kernel::Entity_ABC* superior );
    virtual void SetSuperiorInternal( kernel::Entity_ABC* superior );
    virtual void CreateDictionary( PropertiesDictionary& dico ) const;
    //@}

    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& entity_;
    const kernel::Entity_ABC* superior_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::CreateDictionary( PropertiesDictionary& dictionary ) const
{
    dictionary.RegisterExtension( entity_, (const Interface*)this, tools::translate( "EntityHierarchies", "Hierarchies/Superior" ), superior_ );
}

}

#include "EntityHierarchies.inl"

#endif // __gui_EntityHierarchies_h_
