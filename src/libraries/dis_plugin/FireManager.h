// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireManager_h_
#define __FireManager_h_

#include "hla_plugin/EntityIdentifier.h"
#include "protocol/protocol.h"

using namespace Common;
using namespace MsgsSimToClient;

namespace plugins
{
namespace dis
{
    class UdpNetwork;
    class Time_ABC;

// =============================================================================
/** @class  FireManager
    @brief  FireManager
*/
// Created: AGE 2008-04-08
// =============================================================================
class FireManager
{

public:
    //! @name Constructors/Destructor
    //@{
             FireManager( UdpNetwork& network, const Time_ABC& time, unsigned char exercise );
    virtual ~FireManager();
    //@}

    //! @name Operations
    //@{
    void Update       ( const MsgSimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireManager( const FireManager& );            //!< Copy constructor
    FireManager& operator=( const FireManager& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReceiveFire( const MsgStartUnitFire& fire );
    void ReceiveFire( const MsgStopUnitFire& fire );
    void UpdateFireEffect( const MsgStartFireEffect& fire );
    void UpdateDetonations();
    void CreateFire( const MsgCoordLatLong& position );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, MsgCoordLatLong >  T_Fires;
    typedef T_Fires::const_iterator                     CIT_Fires;
    //@}

private:
    //! @name Member data
    //@{
    UdpNetwork& network_;
    const Time_ABC& time_;
    unsigned char exercise_;
    T_Fires activeFires_;
    //@}
};

}
}

#endif // __FireManager_h_
