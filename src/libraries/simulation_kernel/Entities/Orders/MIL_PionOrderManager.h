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

#include "MIL.h"

#include "MIL_OrderManager_ABC.h"

class MIL_AgentPion;
class MIL_MissionType_ABC;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_PionOrderManager : public MIL_OrderManager_ABC
{
    MT_COPYNOTALLOWED( MIL_PionOrderManager );

public:
    MIL_PionOrderManager( MIL_AgentPion& pion );
    virtual ~MIL_PionOrderManager();

    // @name Events 
    //@{
    virtual void OnReceiveFragOrder( const ASN1T_MsgFragOrder& asn );
            void OnReceiveMission  ( const ASN1T_MsgUnitOrder& asn );
            void OnReceiveMission  ( const MIL_MissionType_ABC& type );
    //@}

    //! @name Main
    //@{
    virtual void Update( bool bDead );
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
    //@}
  
private:
    MIL_AgentPion& pion_;        
};

#endif // __MIL_PionOrderManager_h_
