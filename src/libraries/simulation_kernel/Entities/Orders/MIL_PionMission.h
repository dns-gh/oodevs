// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PionMission_h_
#define __MIL_PionMission_h_

#include "MIL_Mission_ABC.h"

class MIL_MissionType_ABC;
class MIL_AgentPion;
class MIL_AutomateMission;

// =============================================================================
/** @class  MIL_PionMission
    @brief  MIL_PionMission
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_PionMission : public MIL_Mission_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, uint32_t id, const boost::shared_ptr< MIL_Mission_ABC >& parent );
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, uint32_t id, const sword::MissionParameters& parameters );
             MIL_PionMission( MIL_AgentPion& pion, const MIL_PionMission& rhs, uint32_t id );
    virtual ~MIL_PionMission();
    //@}

    //! @name Accessors
    //@{
    virtual MIL_AgentPion& GetPion() const;
    virtual unsigned int GetOwnerId() const;
    //@}

    //! @name Operations
    //@{
    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > self, ActionManager& actions );
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;

    virtual void Send( ActionManager& actions ) const;

    boost::shared_ptr< MIL_Mission_ABC > CreateCopy( MIL_AgentPion& target, uint32_t id ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const MIL_PionMission* mission, const unsigned int version );
    template< typename Archive > friend void load_construct_data( Archive& archive, MIL_PionMission* mission, const unsigned int version );
    //@}

private:
    //! @name Network
    //@{
    static void SendNoMission( const MIL_AgentPion& pion );
    //@}

private:
    //! @name Member Data
    //@{
    MIL_AgentPion& pion_;
    bool bDIABehaviorActivated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PionMission )

#endif // __MIL_PionMission_h_
