// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:22 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Reinforcement.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Reinforcement_h_
#define __PHY_RolePion_Reinforcement_h_

#include "PHY_RoleInterface_Reinforcement.h"

class MIL_AgentPion;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RolePion_Reinforcement
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Reinforcement : public PHY_RoleInterface_Reinforcement
{
    MT_COPYNOTALLOWED( PHY_RolePion_Reinforcement )

public:
    //! @name Types
    //@{
    typedef std::set< MIL_AgentPion* > T_PionSet;
    typedef T_PionSet::const_iterator  CIT_PionSet;
    //@}

public:
             PHY_RolePion_Reinforcement( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Reinforcement();
    virtual ~PHY_RolePion_Reinforcement();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Operations
    //@{
    bool Reinforce          ( MIL_AgentPion& pion );
    void CancelReinforcement();
    bool IsReinforcing      () const;
    bool IsReinforced       () const;
    bool IsReinforcedBy     ( MIL_AgentPion& pion ) const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    const T_PionSet& GetReinforcements() const;
          bool       CanReinforce     () const;
          bool       CanBeReinforced  () const;
    //@}

private:
    //! @name Notifications
    //@{
    void NotifyReinforcementAdded  ( MIL_AgentPion& reinforcement );
    void NotifyReinforcementRemoved( MIL_AgentPion& reinforcement );
    //@}

private:
    MIL_AgentPion* pPion_;
    MIL_AgentPion* pPionReinforced_;
    T_PionSet      reinforcements_;
    bool           bHasChanged_;
};

#endif // __PHY_RolePion_Reinforcement_h_
