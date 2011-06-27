// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationNetworker_h_
#define __SimulationNetworker_h_

#include "SimulationPublisher_ABC.h"
#include "tools/ClientNetworker.h"

namespace sword
{
    class SimToClient;
}

namespace dispatcher
{
    class Model;
    class ClientsNetworker;
    class MessageHandler_ABC;
    class Simulation;
    class Config;
    class RotatingLog;

// =============================================================================
/** @class  SimulationNetworker
    @brief  SimulationNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class SimulationNetworker : public tools::ClientNetworker
                          , public SimulationPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationNetworker( Model& model, ClientsNetworker& clients,
                                  MessageHandler_ABC& handler, const Config& config,
                                  RotatingLog& log );
    virtual ~SimulationNetworker();
    //@}

    //! @name Dispatching
    //@{
    virtual void Send( const sword::ClientToSim& asnMsg );
    virtual void Send( const sword::DispatcherToSim& asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimulationNetworker( const SimulationNetworker& );            //!< Copy constructor
    SimulationNetworker& operator=( const SimulationNetworker& ); //!< Assignment operator
    //@}

    //! @name Connection callbacks
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveSimToClient( const std::string& linkFrom, const sword::SimToClient& message );
    //@}

private:
    Model&                      model_;
    ClientsNetworker&           clients_;
    MessageHandler_ABC&         handler_;
    std::auto_ptr< Simulation > simulation_;
};

}

#endif // __SimulationNetworker_h_
