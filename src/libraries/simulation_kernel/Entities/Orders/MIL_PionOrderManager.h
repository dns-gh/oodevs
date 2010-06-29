// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PionOrderManager_h_
#define __MIL_PionOrderManager_h_

#include "MIL.h"
#include "MIL_OrderManager_ABC.h"

namespace Common
{
    class MsgUnitOrder;
}

namespace MsgsClientToSim
{
    class MsgFragOrder;
}

class MIL_AgentPion;
class MIL_MissionType_ABC;
struct ASN1T_MsgFragOrder;
struct ASN1T_MsgUnitOrder;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_PionOrderManager : public MIL_OrderManager_ABC
{

public:
    explicit MIL_PionOrderManager( MIL_AgentPion& pion );
    virtual ~MIL_PionOrderManager();

    // @name Events
    //@{
    virtual void OnReceiveFragOrder( const MsgsClientToSim::MsgFragOrder& asn );
            void OnReceiveMission  ( const Common::MsgUnitOrder& asn );
            void OnReceiveMission  ( const MIL_MissionType_ABC& type );
    //@}

    //! @name Relieve
    //@{
    bool RelievePion   ( const MIL_AgentPion& pion );
    bool CanRelievePion( const MIL_AgentPion& pion ) const;
    //@}

    //! @name Accessors
    //@{
          bool        IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const;
    const MIL_Fuseau& GetFuseau         () const;
    virtual MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function ) const;
    virtual MIL_LimaOrder* FindLima             ( uint nID ) const;
    virtual MIL_LimaOrder* FindNextScheduledLima() const;
    //@}

private:
    MIL_AgentPion& pion_;
};

#endif // __MIL_PionOrderManager_h_
