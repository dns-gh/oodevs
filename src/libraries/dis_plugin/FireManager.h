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
#include "protocol/Protocol.h"

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
    void Update( const sword::SimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireManager( const FireManager& );            //!< Copy constructor
    FireManager& operator=( const FireManager& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReceiveFire( const sword::StartUnitFire& message );
    void ReceiveFire( const sword::StopUnitFire& message );
    void UpdateFireEffect( const sword::StartFireEffect& message );
    void UpdateDetonations();
    void CreateFire( const sword::MsgCoordLatLong& position );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, sword::MsgCoordLatLong > T_Fires;
    typedef T_Fires::const_iterator                         CIT_Fires;
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
