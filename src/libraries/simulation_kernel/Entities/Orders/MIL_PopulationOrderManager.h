// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationOrderManager_h_
#define __MIL_PopulationOrderManager_h_

#include "MIL.h"
#include "MIL_OrderManager_ABC.h"

namespace Common
{
    class MsgPopulationOrder;
}

namespace MsgsClientToSim
{
    class MsgFragOrder;
}

class MIL_Population;
class MIL_MissionType_ABC;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_PopulationOrderManager : public MIL_OrderManager_ABC
{
public:
    explicit MIL_PopulationOrderManager( MIL_Population& population );
    virtual ~MIL_PopulationOrderManager();

    // @name Events
    //@{
    virtual void OnReceiveFragOrder( const MsgsClientToSim::MsgFragOrder& asn );
            void OnReceiveMission  ( const Common::MsgPopulationOrder& asn );
    //@}

    //! @name Operations
    //@{
    bool IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const;
    //@}

private:
    MIL_Population& population_;
};

#endif // __MIL_PopulationOrderManager_h_
