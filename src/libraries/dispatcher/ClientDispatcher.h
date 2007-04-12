// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientDispatcher_h_
#define __ClientDispatcher_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class SimulationNetworker;

// =============================================================================
/** @class  ClientDispatcher
    @brief  ClientDispatcher
*/
// Created: AGE 2007-04-10
// =============================================================================
class ClientDispatcher
{

public:
    //! @name Constructors/Destructor
    //@{
             ClientDispatcher( SimulationNetworker& simu );
    virtual ~ClientDispatcher();
    //@}

    //! @name Operations
    //@{
    void OnReceive( const ASN1T_MsgsOutClient& asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientDispatcher( const ClientDispatcher& );            //!< Copy constructor
    ClientDispatcher& operator=( const ClientDispatcher& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    SimulationNetworker& simu_;
    //@}
};

}

#endif // __ClientDispatcher_h_
