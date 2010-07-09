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

#include "protocol/Protocol.h"
#include "tools/Resolver.h"

using namespace Common;


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

    void AddFire        ( const MsgsSimToClient::MsgStartUnitFire& message );
    void AddFire        ( const MsgsSimToClient::MsgStartPopulationFire& message );
    kernel::Entity_ABC* FindFirer( const MsgsSimToClient::MsgStopUnitFire& message );
    kernel::Entity_ABC* FindFirer( const MsgsSimToClient::MsgStopPopulationFire& message );
    void RemoveFire     ( const MsgsSimToClient::MsgStopUnitFire& message );
    void RemoveFire     ( const MsgsSimToClient::MsgStopPopulationFire& message );
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
