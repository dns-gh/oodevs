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
#include "game_asn/Simulation.h"

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
    void OnReceive( const ASN1T_MsgsSimToClient& asnMsg );

    virtual void Send( const ASN1T_MsgsDispatcherToSim& asnMsg );
    virtual void Send( const ASN1T_MsgsClientToSim& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignement operator
    //@}

private:
    MessageHandler_ABC& handler_;
    tools::MessageSender_ABC& messageService_;
    std::string endpoint_;
};

}

#endif // __Simulation_h_
