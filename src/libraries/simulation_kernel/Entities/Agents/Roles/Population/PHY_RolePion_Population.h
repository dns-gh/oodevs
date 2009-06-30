// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RolePion_Population.h $
// $Author: Jvt $
// $Modtime: 6/04/05 16:58 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Population.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Population_h_
#define __PHY_RolePion_Population_h_

#include "PHY_RoleInterface_Population.h"

class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Population : public PHY_RoleInterface_Population
{
    MT_COPYNOTALLOWED( PHY_RolePion_Population )

public:
             PHY_RolePion_Population( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Population();
    virtual ~PHY_RolePion_Population();

    //! @name CheckPoints
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
    MT_Float ModifyMaxSpeed         ( MT_Float rSpeed    ) const;
    MT_Float ModifyReloadingDuration( MT_Float rDuration ) const;

    void     EnableSlowDown ();
    void     DisableSlowDown();

    void     EnableReloadingDurationModification ();
    void     DisableReloadingDurationModification();

            void EnableInvulnerability ();
            void DisableInvulnerability();
    virtual bool IsInvulnerable        () const;

    MT_Float GetCollidingPopulationDensity() const;
    //@}

    //! @name Network
    //@{
//    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
//    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private:
    MIL_AgentPion* pPion_;
    bool           bSlowDownEnabled_;
    bool           bReloadingDurationModificationEnabled_;
    bool           bInvulnerabilityEnabled_;
    bool           bHasChanged_;
};

#endif // __PHY_RolePion_Population_h_
