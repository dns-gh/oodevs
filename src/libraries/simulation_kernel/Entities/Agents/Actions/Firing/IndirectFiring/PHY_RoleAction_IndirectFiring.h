// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h $
// $Author: Jvt $
// $Modtime: 30/03/05 15:39 $
// $Revision: 3 $
// $Workfile: PHY_RoleAction_IndirectFiring.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_IndirectFiring_h_
#define __PHY_RoleAction_IndirectFiring_h_

#include "MT_Tools/MT_Role_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

class PHY_RoleInterface_FireTarget;
class MIL_AgentPion;
class MIL_Effect_IndirectFire;

// =============================================================================
// @class  PHY_RoleAction_IndirectFiring
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_IndirectFiring : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_IndirectFiring )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_IndirectFiring RoleInterface;
    //@}

public:
             PHY_RoleAction_IndirectFiring( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RoleAction_IndirectFiring();
    virtual ~PHY_RoleAction_IndirectFiring();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean ();
    //@}

    //! @name Operations
    //@{
    int  Fire         ( MIL_Effect_IndirectFire* pEffect );
    void FireSuspended();

    const PHY_DotationCategory* GetMunitionForIndirectFire( const PHY_IndirectFireDotationClass& indirectWeaponCategory, const MT_Vector2D& vTargetPosition );
          int                   ThrowSmoke                ( const MT_Vector2D& vTargetPosition, uint nNbrAmmo );
    //@}

    //! @name Return codes
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,    // -> le tir est en cours d'exécution
        eNoCapacity, // -> le tir n'a pas lieu car aucune composante n'a la capacité de tirer
        eFinished,    // -> le tir est terminé par un DEC_StopTir ou parce que les obus sont arrivés sur la cible
        eImpossible,
        eNoAmmo
    };
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion* pPion_;
    //@}
};

#endif // __PHY_RoleAction_IndirectFiring_h_
