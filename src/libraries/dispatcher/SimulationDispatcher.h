// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationDispatcher_h_
#define __SimulationDispatcher_h_

#include "MessageHandler_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
    class ReplayModel_ABC;

// =============================================================================
/** @class  SimulationDispatcher
    @brief  Simulation dispatcher
*/
// Created: AGE 2007-04-10
// =============================================================================
class SimulationDispatcher : public MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationDispatcher( ClientPublisher_ABC& publisher, const ReplayModel_ABC& synchronizer );
    virtual ~SimulationDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& asnMsg );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsDestruction( const sword::SimToClient& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    ClientPublisher_ABC& publisher_;
    const ReplayModel_ABC& synchronizer_;
    //@}
};

}

#endif // __SimulationDispatcher_h_
