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
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_PionOrderManager : public MIL_OrderManager_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PionOrderManager();
    explicit MIL_PionOrderManager( MIL_AgentPion& pion );
    virtual ~MIL_PionOrderManager();
    //@}

    // @name Events
    //@{
    virtual void OnReceiveFragOrder( const sword::FragOrder& asn );
    void OnReceiveMission( const sword::UnitOrder& asn );
    void OnReceiveMission( const MIL_MissionType_ABC& type );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
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
    virtual MIL_LimaOrder* FindNextScheduledLima() const;
    //@}

private:
    //! @name Memner Data
    //@{
    MIL_AgentPion* pion_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PionOrderManager )

#endif // __MIL_PionOrderManager_h_
