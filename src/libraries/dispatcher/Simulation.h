// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include "SimulationPublisher_ABC.h"

namespace sword
{
    class SimToClient;
    class ClientToSim;
    class DispatcherToSim;
}

namespace tools
{
    class MessageSender_ABC;
}

namespace dispatcher
{
    class MessageHandler_ABC;

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Simulation : public SimulationPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Simulation( MessageHandler_ABC& handler, tools::MessageSender_ABC& sender, const std::string& endpoint );
    virtual ~Simulation();
    //@}

    //! @name Messages
    //@{
    void OnReceive( const sword::SimToClient& msg );

    virtual void Send( const sword::DispatcherToSim& msg );
    virtual void Send( const sword::ClientToSim& msg );
    //@}

private:
    MessageHandler_ABC& handler_;
    tools::MessageSender_ABC& sender_;
    const std::string endpoint_;
};

}

#endif // __Simulation_h_
