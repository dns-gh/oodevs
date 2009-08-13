// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RolePion_Medical.h $
// $Author: Nld $
// $Modtime: 27/04/05 17:15 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Medical.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Medical_h_
#define __PHY_RolePion_Medical_h_

#include "MIL.h"
#include "PHY_RoleInterface_Medical.h"

class PHY_MedicalHumanState;
class PHY_Human;
class MIL_AgentPion;
class PHY_MedicalCollectionAmbulance;

// =============================================================================
// @class  PHY_RolePion_Medical
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Medical : public PHY_RoleInterface_Medical
{

public:
            PHY_RolePion_Medical();
            PHY_RolePion_Medical( MIL_AgentPion& );
    virtual ~PHY_RolePion_Medical();

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
    virtual void EnableSystem          ();
    virtual void DisableSystem         ();
    virtual void EnableHealingFunction ();
    virtual void DisableHealingFunction();
    virtual void EnableSortingFunction ();
    virtual void DisableSortingFunction();

    virtual void ChangePriorities( const T_MedicalPriorityVector& priorities );
    virtual void ChangePriorities( const T_AutomateVector& priorities );

    virtual PHY_MedicalHumanState* HandleHumanEvacuatedByThirdParty ( MIL_AgentPion& pion, PHY_Human& human ); // Imex
    virtual PHY_MedicalHumanState* HandleHumanForEvacuation         ( MIL_AgentPion& pion, PHY_Human& human ); // Releve
    virtual int                    GetAvailabilityScoreForEvacuation( const PHY_Human& human ) const;
    virtual bool                   HandleHumanForCollection         ( PHY_MedicalHumanState& humanState ); // Ramassage
    virtual int                    GetAvailabilityScoreForCollection( const PHY_MedicalHumanState& humanState ) const;

    virtual void                   HandleHumanForSorting           ( const PHY_MedicalCollectionAmbulance& ambulance, PHY_MedicalHumanState& humanState         );
    virtual int                    GetAvailabilityScoreForSorting  ( const PHY_MedicalCollectionAmbulance& ambulance ) const;
    virtual void                   ReserveForSorting               ( const PHY_MedicalCollectionAmbulance& ambulance );
    virtual void                   CancelReservationForSorting     ( const PHY_MedicalCollectionAmbulance& ambulance );

    virtual int                    GetAvailabilityScoreForHealing  ( const PHY_MedicalHumanState& humanState );
    virtual bool                   HandleHumanForHealing           ( PHY_MedicalHumanState& humanState );
    //@}

    //! @name Tools
    //@{
    virtual bool CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const;

    const MT_Vector2D& GetPosition() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}

private:
    MIL_AgentPion* pPion_;
};

#endif // __PHY_RolePion_Medical_h_
