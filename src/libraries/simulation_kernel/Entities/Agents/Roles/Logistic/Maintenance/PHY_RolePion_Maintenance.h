// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.h $
// $Author: Nld $
// $Modtime: 27/04/05 17:11 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Maintenance.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Maintenance_h_
#define __PHY_RolePion_Maintenance_h_

#include "MIL.h"
#include "PHY_RoleInterface_Maintenance.h"

class PHY_MaintenanceComposanteState;
class PHY_ComposantePion;
class MIL_AgentPion;
class PHY_MaintenanceWorkRate;

// =============================================================================
// @class  PHY_RolePion_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Maintenance : public PHY_RoleInterface_Maintenance
{

public:
             PHY_RolePion_Maintenance();
    virtual ~PHY_RolePion_Maintenance();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean         ();
    //@}

    //! @name Main
    //@{
    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual void ChangePriorities( const T_MaintenancePriorityVector& priorities );
    virtual void ChangePriorities( const T_AutomateVector&            priorities );
    virtual void ChangeWorkRate  ( const PHY_MaintenanceWorkRate&     workRate   );

    virtual PHY_MaintenanceComposanteState* HandleComposanteForTransport    ( MIL_AgentPion& pion, PHY_ComposantePion& composante );
    virtual bool                            HandleComposanteForTransport    ( PHY_MaintenanceComposanteState& composanteState );
    virtual int                             GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante );

    virtual bool                            HandleComposanteForRepair    ( PHY_MaintenanceComposanteState& composanteState );
    virtual int                             GetAvailabilityScoreForRepair( PHY_MaintenanceComposanteState& composanteState );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}
};

#endif // __PHY_RolePion_Maintenance_h_
