 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationPublisher_h_
#define __SimulationPublisher_h_

#include "AsnTypes.h"
#include "Publisher_ABC.h"

namespace dispatcher
{
class Simulation;

// =============================================================================
/** @class  SimulationPublisher
    @brief  SimulationPublisher
*/
// Created: NLD 2006-09-19
// =============================================================================
class SimulationPublisher : public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     SimulationPublisher( Simulation& simulation );
    ~SimulationPublisher();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsInSim&    msg );
    virtual void Send( const ASN1T_MsgsInClient& msg );

    virtual void                     Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg );
    virtual DIN::DIN_BufferedMessage GetDinMsg();
    //@}

private:
    Simulation& simulation_;
};

}

#endif // __SimulationPublisher_h_
