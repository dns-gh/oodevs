// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FiresModel_h_
#define __FiresModel_h_

#include "tools/Resolver.h"

namespace sword
{
    class StartUnitFire;
    class StartCrowdFire;
}

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Population_ABC;
}

// =============================================================================
/** @class  FiresModel
    @brief  FiresModel
*/
// Created: AGE 2006-03-13
// =============================================================================
class FiresModel : public tools::Resolver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             FiresModel( const tools::Resolver_ABC< kernel::Agent_ABC >& agents, const tools::Resolver_ABC< kernel::Population_ABC >& populations );
    virtual ~FiresModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void AddFire( const sword::StartUnitFire& message );
    void AddFire( const sword::StartCrowdFire& message );
    template< typename T >
    void RemoveFire( const T& message );
    template< typename T >
    kernel::Entity_ABC* FindFirer( const T& message );
    template< typename T >
    kernel::Entity_ABC* FindTarget( const T& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FiresModel( const FiresModel& );            //!< Copy constructor
    FiresModel& operator=( const FiresModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddTarget( const sword::StartUnitFire& message );
    //@}

private:
    //! @name Member data
    //@{
    tools::Resolver< kernel::Entity_ABC > targets_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    const tools::Resolver_ABC< kernel::Population_ABC >& populations_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: FiresModel::FindFirer
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
template< typename T >
kernel::Entity_ABC* FiresModel::FindFirer( const T& message )
{
    return Find( message.fire().id() );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::FindTarget
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
template< typename T >
kernel::Entity_ABC* FiresModel::FindTarget( const T& message )
{
    return targets_.Find( message.fire().id() );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::RemoveFire
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
template< typename T >
void FiresModel::RemoveFire( const T& message )
{
    Remove( message.fire().id() );
    targets_.Remove( message.fire().id() );
}

#endif // __FiresModel_h_
