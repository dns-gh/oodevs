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

#include "Publisher_ABC.h"
#include "tools/ClientNetworker_ABC.h"

struct ASN1T_MsgsInSim;

namespace DIN
{
    class DIN_Input;
}

namespace dispatcher 
{
class Dispatcher;
class Simulation;
class Config;

// =============================================================================
/** @class  SimulationNetworker
    @brief  SimulationNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class SimulationNetworker : public tools::ClientNetworker_ABC
                          , public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationNetworker( Dispatcher& dispatcher, const Config& config );
    virtual ~SimulationNetworker();
    //@}

    //! @name Dispatching
    //@{
    virtual void Send( const ASN1T_MsgsClientToSim& asnMsg );
    virtual void Send( const ASN1T_MsgsMiddleToSim& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SimulationNetworker( const SimulationNetworker& );            //!< Copy constructor
    SimulationNetworker& operator=( const SimulationNetworker& ); //!< Assignement operator
    //@}

    //! @name Connection callbacks
    //@{
    virtual void OnConnected        ( DIN::DIN_Link& link );
    virtual void OnConnectionFailed ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    virtual void OnConnectionLost   ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgSimToClient( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );    
    void OnReceiveMsgSimToMiddle( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );        
    //@}

private:
    const Config&     config_;
          Dispatcher& dispatcher_;    
          Simulation* pSimulation_;
};

}

#endif // __SimulationNetworker_h_
