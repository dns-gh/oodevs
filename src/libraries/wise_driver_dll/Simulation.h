// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include <map>
#include <wiseinterfacetypes.h>

namespace MsgsSimToClient
{
    class MsgControlInformation;
    class MsgControlBeginTick;
    class MsgControlChangeTimeFactorAck;
}

class CWISEDriver;
class SwordMessagePublisher_ABC;

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: SEB 2010-10-14
// =============================================================================
class Simulation
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Simulation( SwordMessagePublisher_ABC& publisher );
    virtual ~Simulation();
    //@}

    //! @name Operations
    //@{
    void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const MsgsSimToClient::MsgControlInformation& message );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const MsgsSimToClient::MsgControlBeginTick& message );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const MsgsSimToClient::MsgControlChangeTimeFactorAck& message );
    void Destroy( CWISEDriver& driver, const WISE_HANDLE& database );
    //@}

    //! @name Event handlers
    //@{
    void OnUpdateState( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle );
    void OnUpdateAccelerationFactor( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    SwordMessagePublisher_ABC& publisher_;
    bool running_;
    WISE_HANDLE handle_;
    std::map< std::wstring, WISE_HANDLE > attributes_;
    //@}
};

#endif // __Simulation_h_
