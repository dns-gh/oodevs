// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:17 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Maintenance.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Maintenance_h_
#define __PHY_RoleInterface_Maintenance_h_

#include "MT_Tools/Role_ABC.h"

class PHY_MaintenanceComposanteState;
class PHY_ComposantePion;
class MIL_Agent_ABC;
class PHY_MaintenanceWorkRate;
class PHY_ComposanteTypePion;
class PHY_Breakdown;
class MIL_AgentPionLOG_ABC;
class MIL_AutomateLOG;
class MIL_Automate;

// =============================================================================
// @class  PHY_RoleInterface_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Maintenance : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Maintenance RoleInterface;

    typedef std::vector< const MIL_Automate* > T_AutomateVector;

    typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;
    //@}

public:
             PHY_RoleInterface_Maintenance() {}
    virtual ~PHY_RoleInterface_Maintenance() {}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void UpdateLogistic( bool bIsDead ) = 0;
    virtual void Clean() = 0;
    //@}

    //! @name Main
    //@{
    virtual void EnableSystem() = 0;
    virtual void DisableSystem() = 0;

    virtual T_MaintenancePriorityVector GetMaintenancePriorities() const = 0;
    virtual void ChangePriorities( const T_MaintenancePriorityVector& priorities ) = 0;
    virtual T_AutomateVector GetAutomatePriorities() const = 0;
    virtual void ChangePriorities( const T_AutomateVector& priorities ) = 0;
    virtual int GetWorkRate() const = 0;
    virtual void ChangeWorkRate( const PHY_MaintenanceWorkRate& workRate ) = 0;

    virtual PHY_MaintenanceComposanteState* HandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante ) = 0;
    virtual bool HandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState ) = 0;
    virtual int GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante, const PHY_ComposanteTypePion* type = 0 ) const = 0;

    virtual bool HandleComposanteForDiagnosis( PHY_MaintenanceComposanteState& composanteState ) = 0;
    virtual int GetAvailabilityScoreForDiagnosis( const PHY_ComposanteTypePion* type = 0 ) const = 0;

    virtual bool HandleComposanteForRepair( PHY_MaintenanceComposanteState& composanteState ) = 0;
    virtual int GetAvailabilityScoreForRepair( const PHY_MaintenanceComposanteState& composanteState, const PHY_ComposanteTypePion* type = 0 ) const = 0;

    virtual bool FinishAllHandlingsSuccessfullyWithoutDelay() = 0;
    //@}

    //! @name Tools
    //@{
    virtual PHY_ComposantePion* GetAvailableHauler( const PHY_ComposanteTypePion& carried, const PHY_ComposanteTypePion* type = 0 ) const = 0;
    virtual PHY_ComposantePion* GetAvailableDiagnoser( const PHY_ComposanteTypePion* type = 0 ) const = 0;
    virtual PHY_ComposantePion* GetAvailableRepairer( const PHY_Breakdown& breakdown, const PHY_ComposanteTypePion* type = 0 ) const = 0;
    virtual bool HasUsableRepairer( const PHY_Breakdown& breakdown ) const = 0;
    virtual bool ConsumePartsForBreakdown( const PHY_Breakdown& breakdown ) = 0;

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante ) = 0;
    virtual void StopUsingForLogistic( PHY_ComposantePion& composante ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_AgentPionLOG_ABC& GetPion() const = 0;
    virtual MIL_AutomateLOG* FindLogisticManager() const = 0;
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_Maintenance_h_
