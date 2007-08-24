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

#include "ReplayModel_ABC.h"
#include "MessageHandler_ABC.h"

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  SimulationDispatcher
    @brief  Simulation dispatcher
*/
// Created: AGE 2007-04-10
// =============================================================================
class SimulationDispatcher : public ReplayModel_ABC, public MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SimulationDispatcher( ClientPublisher_ABC& clientsPublisher, Model& model );
    virtual ~SimulationDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    virtual void StartSynchronisation();
    virtual void EndSynchronisation();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimulationDispatcher( const SimulationDispatcher& );            //!< Copy constructor
    SimulationDispatcher& operator=( const SimulationDispatcher& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool IsNotDestruction( const ASN1T_MsgsSimToClient& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    ClientPublisher_ABC& clientsPublisher_;
    Model&               model_;
    bool                 synching_;
    //@}
};

}

#endif // __SimulationDispatcher_h_
