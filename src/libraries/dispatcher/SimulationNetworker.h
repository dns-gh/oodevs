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

#include "network/ClientNetworker_ABC.h"

struct ASN1T_MsgsInSim;

namespace DIN
{
    class DIN_Input;
}

namespace dispatcher 
{
class Dispatcher;
class Simulation;

// =============================================================================
/** @class  SimulationNetworker
    @brief  SimulationNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class SimulationNetworker : public network::ClientNetworker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     SimulationNetworker( Dispatcher& dispatcher, const std::string& configFile );
    ~SimulationNetworker();
    //@}

    //! @name Dispatching
    //@{
    void Dispatch( const ASN1T_MsgsInSim& asnMsg );
    void Dispatch( unsigned int nMsgID, const DIN::DIN_Input& dinMsg );
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
    void OnReceiveMsgOutSim                                ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );    

    void OnReceiveMsgInit                                  ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgProfilingValues                       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgTrace                                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitVisionCones                       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitInterVisibility                   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgObjectInterVisibility                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgPopulationFlowInterVisibility         ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDebugDrawPoints                       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgPopulationCollision                   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgEnvironmentType                       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}

private:
    Dispatcher& dispatcher_;    
    Simulation* pSimulation_;
};

}

#endif // __SimulationNetworker_h_
