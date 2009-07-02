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
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const ASN1T_MsgUnitOrder& asn );
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const MIL_AutomateMission& parent );
             MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion );
    virtual ~MIL_PionMission();
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    //@}

    //! @name Accessors
    //@{
    MIL_AgentPion& GetPion() const;
    //@}

    //! @name Operations
    //@{
    virtual void             Start               ();
    virtual bool             IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
            MIL_PionMission& CreateCopy          ( MIL_AgentPion& target ) const;
    virtual void             AffectDirection     ( const MT_Vector2D& direction );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    MIL_PionMission( MIL_AgentPion& pion, const MIL_PionMission& rhs );
    //@}

    //! @name Tools
    //@{
    void Stop();
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
