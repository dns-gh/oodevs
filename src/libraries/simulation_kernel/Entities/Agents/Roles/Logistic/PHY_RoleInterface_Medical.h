// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:40 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Medical.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Medical_h_
#define __PHY_RoleInterface_Medical_h_

#include "MT_Tools/Role_ABC.h"

class PHY_MedicalHumanState;
class Human_ABC;
class MIL_AgentPion;
class MIL_AgentPionLOG_ABC;
class MIL_AutomateLOG;
class MIL_Automate;
class PHY_ComposantePion;
class PHY_MedicalEvacuationConsign;
class PHY_MedicalCollectionConsign;
class PHY_MedicalEvacuationAmbulance;
class PHY_MedicalCollectionAmbulance;
class PHY_HumanWound;

// =============================================================================
// @class  PHY_RoleInterface_Medical
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Medical : public tools::Role_ABC
                                , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Medical RoleInterface;
    typedef std::vector< const MIL_Automate* > T_AutomateVector;
    typedef std::vector< const PHY_HumanWound* > T_MedicalPriorityVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Medical() {}
    virtual ~PHY_RoleInterface_Medical() {}
    //@}

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
    virtual void EnableHealingFunction() = 0;
    virtual void DisableHealingFunction() = 0;
    virtual void EnableSortingFunction() = 0;
    virtual void DisableSortingFunction() = 0;

    virtual void ChangePriorities( const T_MedicalPriorityVector& priorities ) = 0;
    virtual void ChangePriorities( const T_AutomateVector& priorities ) = 0;

    virtual PHY_MedicalHumanState* HandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human ) = 0; // Imex
    virtual PHY_MedicalHumanState* HandleHumanForEvacuation( MIL_AgentPion& pion, Human_ABC& human ) = 0; // Releve
    virtual bool                   HandleHumanForEvacuation( PHY_MedicalHumanState& humanState ) = 0;
    virtual int GetAvailabilityScoreForEvacuation( const Human_ABC& human ) const = 0;
    virtual bool HandleHumanForCollection( PHY_MedicalHumanState& humanState ) = 0; // Ramassage
    virtual int GetAvailabilityScoreForCollection( const PHY_MedicalHumanState& humanState ) const = 0;

    virtual void HandleHumanForSorting( PHY_MedicalHumanState& humanState ) = 0;
    virtual int GetAvailabilityScoreForSorting() const = 0;
    virtual void ReserveForSorting( const PHY_MedicalCollectionAmbulance& ambulance ) = 0;
    virtual void CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& ambulance ) = 0;

    virtual int GetAvailabilityScoreForHealing( const PHY_MedicalHumanState& humanState ) const = 0;
    virtual bool HandleHumanForHealing( PHY_MedicalHumanState& humanState ) = 0;

    virtual PHY_MedicalEvacuationAmbulance* GetAvailableEvacuationAmbulance( PHY_MedicalEvacuationConsign& consign ) = 0;
    virtual PHY_MedicalCollectionAmbulance* GetAvailableCollectionAmbulance( PHY_MedicalCollectionConsign& consign ) = 0;
    virtual PHY_ComposantePion* GetAvailableDoctorForDiagnosing() const = 0;
    virtual PHY_ComposantePion* GetAvailableDoctorForSorting() const = 0;
    virtual PHY_ComposantePion* GetAvailableDoctorForHealing( const Human_ABC& human ) const = 0;
    virtual bool HasUsableDoctorForHealing( const Human_ABC& human, bool bBypassPriorities = false ) const = 0;
    //@}

    //! @name Tools
    //@{
    virtual bool CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const = 0;
    //@}

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante ) = 0;
    virtual void StopUsingForLogistic( PHY_ComposantePion& composante ) = 0;

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

#endif // __PHY_RoleInterface_Medical_h_
