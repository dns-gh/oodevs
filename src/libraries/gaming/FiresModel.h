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

#include <boost/noncopyable.hpp>
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
    class PopulationPart_ABC;
    class Profile_ABC;
}

// =============================================================================
/** @class  FiresModel
    @brief  FiresModel
*/
// Created: AGE 2006-03-13
// =============================================================================
class FiresModel : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FiresModel( const tools::Resolver_ABC< kernel::Agent_ABC >& agents, const tools::Resolver_ABC< kernel::PopulationPart_ABC >& populations, kernel::Profile_ABC& profile );
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
    //! @name Helpers
    //@{
    void AddTarget( const sword::StartUnitFire& message );
    kernel::Entity_ABC* FindEntity( unsigned long id );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, unsigned long >    T_IDs;
    typedef T_IDs::const_iterator                     CIT_IDs;
    //@}

private:
    //! @name Member data
    //@{
    T_IDs firers_;
    T_IDs targets_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    const tools::Resolver_ABC< kernel::PopulationPart_ABC >& populations_;
    kernel::Profile_ABC& profile_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: FiresModel::FindFirer
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
template< typename T >
kernel::Entity_ABC* FiresModel::FindFirer( const T& message )
{
    CIT_IDs it = firers_.find( message.fire().id() );
    if( it == firers_.end() )
        return 0;
    return FindEntity( it->second );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::FindTarget
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
template< typename T >
kernel::Entity_ABC* FiresModel::FindTarget( const T& message )
{
    CIT_IDs it = targets_.find( message.fire().id() );
    if( it == targets_.end() )
        return 0;
    return FindEntity( it->second );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::RemoveFire
// Created: ABR 2011-02-17
// -----------------------------------------------------------------------------
template< typename T >
void FiresModel::RemoveFire( const T& message )
{
    firers_.erase( message.fire().id() );
    targets_.erase( message.fire().id() );
}

#endif // __FiresModel_h_
