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

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class InstanciationComplete;
}

// =============================================================================
/** @class  EntityHierarchies
    @brief  Entity hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
class EntityHierarchies : public kernel::EntityHierarchies
                        , public kernel::Updatable_ABC< kernel::InstanciationComplete >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~EntityHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual const kernel::Entity_ABC& GetEntity() const;
    void ChangeSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityHierarchies( const EntityHierarchies& );            //!< Copy constructor
    EntityHierarchies& operator=( const EntityHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    void RegisterToSuperior();
    void RemoveFromSuperior();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& holder_;
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __EntityHierarchies_h_
