// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PionMission_h_
#define __MIL_PionMission_h_

#include "MIL_Mission_ABC.h"

namespace sword
{
    class UnitOrder;
}

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
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const sword::UnitOrder& asn );
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const boost::shared_ptr< MIL_Mission_ABC > parent );
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion );
    virtual ~MIL_PionMission();
    //@}

    //! @name Accessors
    //@{
    virtual MIL_AgentPion& GetPion() const;
    //@}

    //! @name Operations
    //@{
    virtual void             Start               ( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual void             Stop                ( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual bool             IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
    boost::shared_ptr< MIL_Mission_ABC > CreateCopy( MIL_AgentPion& target ) const;
    virtual void             AffectDirection     ( const MT_Vector2D& direction );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    MIL_PionMission( MIL_AgentPion& pion, const MIL_PionMission& rhs );
    //@}

    //! @name Network
    //@{
           void Send         () const;
    static void SendNoMission( const MIL_AgentPion& pion );
    //@}

private:
    MIL_AgentPion& pion_;
    bool           bDIABehaviorActivated_;
};

#endif // __MIL_PionMission_h_
