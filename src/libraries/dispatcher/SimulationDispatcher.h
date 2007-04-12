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

#include "game_asn/Asn.h"
#include "Publisher_ABC.h"

namespace dispatcher
{
    class ClientsNetworker;
    class Model;

// =============================================================================
/** @class  SimulationDispatcher
    @brief  SimulationDispatcher
*/
// Created: AGE 2007-04-10
// =============================================================================
class SimulationDispatcher : private Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SimulationDispatcher( ClientsNetworker& clients, Model& model );
    virtual ~SimulationDispatcher();
    //@}

    //! @name Operations
    //@{
    void OnReceive( const ASN1T_MsgsOutSim& asnMsg );
    void OnReceive( const ASN1T_MsgsInClient& asnMsg );

    void StartSynchronisation();
    void EndSynchronisation();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsInClient& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimulationDispatcher( const SimulationDispatcher& );            //!< Copy constructor
    SimulationDispatcher& operator=( const SimulationDispatcher& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ClientsNetworker& clients_;
    Model& model_;
    bool synching_;
    //@}
};

}

#endif // __SimulationDispatcher_h_
