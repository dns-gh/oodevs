// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchBox_h_
#define __EntitySearchBox_h_

#include "EntitySearchBox_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"

namespace gui
{

// =============================================================================
/** @class  EntitySearchBox
    @brief  EntitySearchBox
*/
// Created: SBO 2006-04-20
// =============================================================================
template< typename Entity >
class EntitySearchBox : public EntitySearchBox_ABC
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< Entity >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntitySearchBox( QWidget* pParent, kernel::Controllers& controllers );
    virtual ~EntitySearchBox();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Entity& );
    virtual void NotifyDeleted( const Entity& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: EntitySearchBox constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
EntitySearchBox< Entity >::EntitySearchBox( QWidget* pParent, kernel::Controllers& controllers )
    : EntitySearchBox_ABC( pParent, controllers.actions_ )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
EntitySearchBox< Entity >::~EntitySearchBox()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::NotifyCreated( const Entity& entity )
{
    AddItem( entity );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::NotifyDeleted( const Entity& entity )
{
    RemoveItem( entity );
}

}

#endif // __EntitySearchBox_h_
