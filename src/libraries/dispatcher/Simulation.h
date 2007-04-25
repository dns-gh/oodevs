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

#include "Publisher_ABC.h"
#include "game_asn/Asn.h"
#include "tools/Server_ABC.h"

namespace DIN
{
    class DIN_Input;
}

namespace dispatcher
{
    class Dispatcher;
    class SaverFacade;
    class SimulationDispatcher;

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Simulation : public tools::Server_ABC
                 , public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Simulation( Dispatcher& dispatcher, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link, const std::string& configFile );
    virtual ~Simulation();
    //@}

    //! @name Messages
    //@{
            void OnReceive( const ASN1T_MsgsSimToClient& asnMsg );
            void OnReceive( const ASN1T_MsgsSimToMiddle& asnMsg );
            void OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg );

    virtual void Send     ( const ASN1T_MsgsMiddleToSim& asnMsg );
            void Send     ( const ASN1T_MsgsMiddleToSim& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );    
    virtual void Send     ( const ASN1T_MsgsClientToSim& asnMsg );
            void Send     ( const ASN1T_MsgsClientToSim& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );
            void Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg );
    //@}

    //! @name Tools
    //@{
    static Simulation& GetSimulationFromLink( const DIN::DIN_Link& link );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateSaver( const std::string& configFile );
    //@}

private:
    Dispatcher&           dispatcher_;
    SaverFacade*          saver_;
    SimulationDispatcher* simDispatch_;
};

}

#endif // __Simulation_h_
