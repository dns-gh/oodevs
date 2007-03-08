// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:12 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Refugee_h_
#define __PHY_RolePion_Refugee_h_

#include "MIL.h"

#include "PHY_RoleInterface_Refugee.h"

class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Refugee : public PHY_RoleInterface_Refugee
{
    MT_COPYNOTALLOWED( PHY_RolePion_Refugee )

public:
             PHY_RolePion_Refugee( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Refugee();
    virtual ~PHY_RolePion_Refugee();

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
    virtual bool Orientate( const MIL_AgentPion& pionManaging );
    virtual bool Release  ();
    virtual bool Release  ( const MIL_CampRefugies& camp );

            bool NotifyManaged  ();
            bool NotifyUnmanaged();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsManaged() const;
    virtual bool IsManaged( const MIL_CampRefugies& camp ) const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private:
          MIL_AgentPion*    pPion_;
          bool              bManaged_;
    const MIL_CampRefugies* pCamp_;
          bool              bHasChanged_;
};

#include "PHY_RolePion_Refugee.inl"

#endif // __PHY_RolePion_Refugee_h_
