// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FiresModel_h_
#define __FiresModel_h_

#include "protocol/Protocol.h"
#include "tools/Resolver.h"

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

    void AddFire        ( const sword::StartUnitFire& message );
    void AddFire        ( const sword::StartCrowdFire& message );
    kernel::Entity_ABC* FindFirer( const sword::StopUnitFire& message );
    kernel::Entity_ABC* FindFirer( const sword::StopCrowdFire& message );
    void RemoveFire     ( const sword::StopUnitFire& message );
    void RemoveFire     ( const sword::StopCrowdFire& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FiresModel( const FiresModel& );            //!< Copy constructor
    FiresModel& operator=( const FiresModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Agent_ABC >& agents_;
    const tools::Resolver_ABC< kernel::Population_ABC >& populations_;
    //@}
};

#endif // __FiresModel_h_
