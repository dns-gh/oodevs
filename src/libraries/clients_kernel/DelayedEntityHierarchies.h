// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DelayedEntityHierarchies_h_
#define __DelayedEntityHierarchies_h_

#include "EntityHierarchies.h"
#include "Updatable_ABC.h"

namespace kernel
{
    class InstanciationComplete;

// =============================================================================
/** @class  DelayedEntityHierarchies
    @brief  DelayedEntityHierarchies
*/
// Created: AGE 2006-10-19
// =============================================================================
template< typename I >
class DelayedEntityHierarchies : public EntityHierarchies< I >
                               , public Updatable_ABC< InstanciationComplete >
{

public:
    //! @name Constructors/Destructor
    //@{
             DelayedEntityHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
                 : EntityHierarchies< I >( controller, entity )
                 , superior_( superior ) {}
    virtual ~DelayedEntityHierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const InstanciationComplete& ) {
        SetSuperior( superior_ );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DelayedEntityHierarchies( const DelayedEntityHierarchies& );            //!< Copy constructor
    DelayedEntityHierarchies& operator=( const DelayedEntityHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Entity_ABC* superior_;
    //@}
};

}

#endif // __DelayedEntityHierarchies_h_
