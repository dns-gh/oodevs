// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityPublisher_h_
#define __EntityPublisher_h_

#include "EntityPublisher_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  EntityPublisher
    @brief  Entity publisher helper
*/
// Created: AGE 2008-06-20
// =============================================================================
template< typename E >
class EntityPublisher : public EntityPublisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityPublisher( const E& entity );
    virtual ~EntityPublisher();
    //@}

    //! @name Operations
    //@{
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityPublisher( const EntityPublisher& );            //!< Copy constructor
    EntityPublisher& operator=( const EntityPublisher& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const E& entity_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: EntityPublisher constructor
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename E >
EntityPublisher< E >::EntityPublisher( const E& entity )
    : entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityPublisher destructor
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename E >
EntityPublisher< E >::~EntityPublisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityPublisher::SendFullUpdate
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename E >
void EntityPublisher< E >::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    entity_.SendFullUpdate( publisher );
}

// -----------------------------------------------------------------------------
// Name: EntityPublisher::SendCreation
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename E >
void EntityPublisher< E >::SendCreation( ClientPublisher_ABC& publisher ) const
{
    entity_.SendCreation( publisher );
}

// -----------------------------------------------------------------------------
// Name: EntityPublisher::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
template< typename E >
void EntityPublisher< E >::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    entity_.SendDestruction( publisher );
}

}

#endif // __EntityPublisher_h_
