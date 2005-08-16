// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h $
// $Author: Jvt $
// $Modtime: 30/03/05 15:37 $
// $Revision: 4 $
// $Workfile: PHY_RoleAction_DirectFiring.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_DirectFiring_h_
#define __PHY_RoleAction_DirectFiring_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "PHY_DirectFireData.h"

class PHY_RoleInterface_FireTarget;
class PHY_AmmoDotationClass;
class PHY_DirectFireResults;
class PHY_ControlZoneFireResults;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_ControlZone;

// =============================================================================
// @class  PHY_RoleAction_DirectFiring
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_DirectFiring : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_DirectFiring )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_DirectFiring RoleInterface;

    enum E_Mode
    {
        eModeNormal, // Chaque composante fait feu avec le meilleur couple ( lanceur / munition )
        eModeFree    // Chaque composante fait feu avec tous les lanceurs
    };
    //@}

public:
             PHY_RoleAction_DirectFiring( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RoleAction_DirectFiring();
    virtual ~PHY_RoleAction_DirectFiring();

    //! @name Checkpoints
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
    int  Fire         ( uint nTargetKnowledgeID, E_Mode nFireMode, MT_Float rPercentageComposantesToUse, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireResults*& pFireResult, const PHY_AmmoDotationClass* pAmmoDotationClass = 0 );
    void FireSuspended( uint nTargetKnowledgeID );
    void FireZone     ( const MIL_ControlZone& zone, PHY_ControlZoneFireResults*& pFireResult );
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
        eImpossible,
        eEnemyDestroyed , // -> le tir est terminé parceque le pion adverse est détruit
        eNoCapacity     , // -> le tir est terminé car il ne reste aucune composante ayant la capacité de tirer
        eNoAmmo         , // -> le tir est terminé parque qu'il reste des composante capables de tirer mais qu'il ne reste plus de munition adéquates
        eRunning        , // -> le tir est en cours d'exécution
        eFinished         // Stop tir
    };
    //@}

private:
    //! @name Tools
    //@{
    MIL_Agent_ABC* GetTarget( uint nTargetKnowledgeID );
    void           Fire     ( PHY_DirectFireData& firerWeapons, MIL_Agent_ABC& target, const PHY_RoleInterface_Composantes::T_ComposanteVector& compTargets, E_Mode nFireMode, PHY_DirectFireResults& fireResult );
    //@}

private:
    MIL_AgentPion* pPion_;
};

#include "PHY_RoleAction_DirectFiring.inl"

#endif // __PHY_RoleAction_DirectFiring_h_
