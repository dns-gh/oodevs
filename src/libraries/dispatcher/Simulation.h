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
}

namespace MsgsDispatcherToSim
{
    class MsgDispatcherToSim;
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
    // $$$$ AGE 2007-08-24: au final, sert juste a positionner un flag sur SimulationNetworker...
*/
// Created: NLD 2006-09-19
// =============================================================================
class Simulation : public SimulationPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Simulation( MessageHandler_ABC& handler, tools::MessageSender_ABC& messageService, const std::string& endpoint );
    virtual ~Simulation();
    //@}

    //! @name Messages
    //@{
    void OnReceive( const sword::SimToClient& asnMsg );

    virtual void Send( const MsgsDispatcherToSim::MsgDispatcherToSim& asnMsg );
    virtual void Send( const sword::ClientToSim& asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignment operator
    //@}

private:
    MessageHandler_ABC& handler_;
    tools::MessageSender_ABC& messageService_;
    std::string endpoint_;
};

}

#endif // __Simulation_h_
