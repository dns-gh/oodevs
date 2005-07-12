// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Surrender.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Surrender_h_
#define __PHY_RolePion_Surrender_h_

#include "MIL.h"

#include "PHY_RoleInterface_Surrender.h"

class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_Surrender
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Surrender : public PHY_RoleInterface_Surrender
{
    MT_COPYNOTALLOWED( PHY_RolePion_Surrender )

public:
             PHY_RolePion_Surrender( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Surrender();
    virtual ~PHY_RolePion_Surrender();

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

    //! @name Main
    //@{
            bool Surrender                ();
    virtual bool TakePrisoner             ( const MIL_AgentPion& pionTakingPrisoner );
            bool NotifyTakenPrisoner      ();
    virtual void NotifyInsidePrisonerCamp ( const MIL_CampPrisonniers& camp );
    virtual void NotifyOutsidePrisonerCamp( const MIL_CampPrisonniers& camp );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsSurrendered() const;
    virtual bool IsPrisoner   () const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private:
    MIL_AgentPion* pPion_;
    bool           bSurrendered_;
    bool           bPrisoner_;
    bool           bHasChanged_;
};

#include "PHY_RolePion_Surrender.inl"

#endif // __PHY_RolePion_Surrender_h_
