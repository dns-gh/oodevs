// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PionOrderManager_h_
#define __MIL_PionOrderManager_h_

#include "MIL_OrderManager_ABC.h"

namespace sword
{
    class UnitOrder;
    class FragOrder;
}

class MIL_AgentPion;
class MIL_MissionType_ABC;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_PionOrderManager : public MIL_OrderManager_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PionOrderManager( MissionController_ABC& controller, MIL_AgentPion& pion );
    virtual ~MIL_PionOrderManager();
    //@}

    // @name Events
    //@{
    void     OnReceiveFragOrder( const sword::FragOrder& asn, const std::function< void( uint32_t ) >& sendAck );
    uint32_t OnReceiveMission  ( const sword::UnitOrder& asn );
    void     OnReceiveMission  ( const MIL_MissionType_ABC& type );
    //@}

    //! @name Relieve
    //@{
    bool RelievePion( const MIL_AgentPion& pion );
    bool CanRelievePion( const MIL_AgentPion& pion ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const;
    const MIL_Fuseau& GetFuseau() const;
    virtual MIL_LimaOrder* FindLima( const MIL_LimaFunction& function ) const;
    virtual MIL_LimaOrder* FindLima( unsigned int nID ) const;
    virtual std::vector< MIL_LimaOrder* > FindAllLimas( const MIL_LimaFunction& func ) const;
    virtual MIL_LimaOrder* FindNextScheduledLima() const;
    //@}

private:
    //! @name Memner Data
    //@{
    MIL_AgentPion& pion_;
    //@}
};

#endif // __MIL_PionOrderManager_h_
