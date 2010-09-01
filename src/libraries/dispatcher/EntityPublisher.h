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
    explicit EntityPublisher( const E& entity )
        : entity_( entity )
    {
        // NOTHING
    }
    virtual ~EntityPublisher()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const
    {
        entity_.SendFullUpdate( publisher );
    }
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const
    {
        entity_.SendCreation( publisher );
    }
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const
    {
        entity_.SendDestruction( publisher );
    }
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

}

#endif // __EntityPublisher_h_
