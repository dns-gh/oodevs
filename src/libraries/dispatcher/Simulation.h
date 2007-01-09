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

#include "AsnTypes.h"
#include "DIN/DIN_UserData_ABC.h"
#include "DIN/DIN_Link.h"

namespace DIN
{
    class DIN_MessageService_ABC;
    class DIN_Input;
    class DIN_BufferedMessage;
}

namespace dispatcher
{
    class Dispatcher;

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Simulation : public DIN::DIN_UserData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Simulation( Dispatcher& dispatcher, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link );
    virtual ~Simulation();
    //@}

    //! @name Messages
    //@{
    void OnReceive( const ASN1T_MsgsOutSim& asnMsg );
    void OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg );
    void Send     ( const ASN1T_MsgsInSim& asnMsg );
    void Send     ( const ASN1T_MsgsInSim& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );
    void Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg );

    DIN::DIN_BufferedMessage GetDinMsg();
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

private:
    Dispatcher&                  dispatcher_;
    DIN::DIN_MessageService_ABC& messageService_;
    DIN::DIN_Link&               link_;
};

}

#include "Simulation.inl"

#endif // __Simulation_h_
