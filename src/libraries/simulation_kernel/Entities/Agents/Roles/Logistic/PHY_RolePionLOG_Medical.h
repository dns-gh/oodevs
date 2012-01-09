// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Medical.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:23 $
// $Revision: 9 $
// $Workfile: PHY_RolePionLOG_Medical.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOG_Medical_h_
#define __PHY_RolePionLOG_Medical_h_

#include "MIL.h"
#include "PHY_RoleInterface_Medical.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkMessageSender_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"

namespace client
{
    class UnitAttributes;
}

class ComposanteUsePredicate_ABC;
class MIL_AgentPionLOG_ABC;
class MIL_AutomateLOG;
class MIL_Automate;
class PHY_ComposantePion;
class PHY_MedicalConsign_ABC;
class PHY_MedicalEvacuationConsign;
class PHY_MedicalCollectionConsign;
class PHY_MedicalEvacuationAmbulance;
class PHY_MedicalCollectionAmbulance;
class PHY_ComposanteUsePredicate;

// =============================================================================
// @class  PHY_RolePionLOG_Medical
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOG_Medical : public PHY_RoleInterface_Medical
                              , public component::ComponentsChangedNotificationHandler_ABC
                              , public network::NetworkMessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePionLOG_Medical( MIL_AgentPionLOG_ABC& pion );
    virtual ~PHY_RolePionLOG_Medical();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean();

    virtual void NotifyComponentHasChanged();

    virtual void EnableSystem();
    virtual void DisableSystem();
    virtual void EnableHealingFunction();
    virtual void DisableHealingFunction();
    virtual void EnableSortingFunction();
    virtual void DisableSortingFunction();

    virtual void ChangePriorities( const T_MedicalPriorityVector& priorities );
    virtual void ChangePriorities( const T_AutomateVector& priorities );

    virtual PHY_MedicalHumanState* HandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human ); // Imex
    virtual PHY_MedicalHumanState* HandleHumanForEvacuation( MIL_AgentPion& pion, Human_ABC& human ); // Releve
    virtual bool                   HandleHumanForEvacuation( PHY_MedicalHumanState& humanState );
    virtual int GetAvailabilityScoreForEvacuation( const Human_ABC& human ) const;
    virtual bool HandleHumanForCollection( PHY_MedicalHumanState& humanState );     // Ramassage
    virtual int GetAvailabilityScoreForCollection( const PHY_MedicalHumanState& humanState ) const;

    virtual void HandleHumanForSorting( PHY_MedicalHumanState& humanState );
    virtual int GetAvailabilityScoreForSorting() const;
    virtual void ReserveForSorting( const PHY_MedicalCollectionAmbulance& ambulance );
    virtual void CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& ambulance );

    virtual int GetAvailabilityScoreForHealing( const PHY_MedicalHumanState& humanState ) const;
    virtual bool HandleHumanForHealing( PHY_MedicalHumanState& humanState );

    PHY_MedicalEvacuationAmbulance* GetAvailableEvacuationAmbulance( PHY_MedicalEvacuationConsign& consign );
    PHY_MedicalCollectionAmbulance* GetAvailableCollectionAmbulance( PHY_MedicalCollectionConsign& consign );
    PHY_ComposantePion* GetAvailableDoctorForDiagnosing() const;
    PHY_ComposantePion* GetAvailableDoctorForSorting() const;
    PHY_ComposantePion* GetAvailableDoctorForHealing( const Human_ABC& human ) const;
    bool HasUsableDoctorForHealing( const Human_ABC& human, bool bBypassPriorities = false ) const;
    //@}

    //! @name Tools
    //@{
    virtual bool CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const;
    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic( PHY_ComposantePion& composante );
    //@}

    //! @name Accessors
    //@{
    virtual       MIL_AutomateLOG*      FindLogisticManager() const;
    virtual const MIL_AgentPionLOG_ABC& GetPion() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   ( unsigned int context = 0 ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::multimap< const MIL_Automate*, PHY_MedicalEvacuationAmbulance* > T_EvacuationAmbulancesMMap;
    typedef T_EvacuationAmbulancesMMap::iterator                                 IT_EvacuationAmbulancesMMap;
    typedef T_EvacuationAmbulancesMMap::const_iterator                          CIT_EvacuationAmbulancesMMap;
    typedef T_EvacuationAmbulancesMMap::_Pairii                                   T_EvacuationAmbulancesMMapRange;

    typedef std::set< const PHY_MedicalCollectionAmbulance* > T_CollectionAmbulancesSet;
    typedef T_CollectionAmbulancesSet::iterator              IT_CollectionAmbulancesSet;
    typedef T_CollectionAmbulancesSet::const_iterator       CIT_CollectionAmbulancesSet;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< PHY_MedicalConsign_ABC* >   T_MedicalConsignList;
    typedef T_MedicalConsignList::iterator        IT_MedicalConsignList;
    typedef T_MedicalConsignList::const_iterator CIT_MedicalConsignList;

    typedef std::list< PHY_MedicalCollectionAmbulance* > T_CollectionAmbulancesList;
    typedef T_CollectionAmbulancesList::iterator        IT_CollectionAmbulancesList;
    typedef T_CollectionAmbulancesList::const_iterator CIT_CollectionAmbulancesList;

    typedef std::vector< std::pair< const MIL_Automate*, T_MedicalConsignList > > T_MedicalConsigns;
    typedef T_MedicalConsigns::iterator                                           IT_MedicalConsigns;
    typedef T_MedicalConsigns::const_iterator                                     CIT_MedicalConsigns;

    typedef std::vector< const MIL_Automate* > T_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;

    typedef std::vector< const PHY_HumanWound* >      T_MedicalPriorityVector;
    typedef T_MedicalPriorityVector::iterator        IT_MedicalPriorityVector;
    typedef T_MedicalPriorityVector::const_iterator CIT_MedicalPriorityVector;
    //@}

private:
    //! @name Tools
    //@{
    void InsertConsign( PHY_MedicalConsign_ABC& );
    void InsertConsigns( const T_MedicalConsigns& );

    bool HasUsableEvacuationAmbulance( const Human_ABC& human ) const;
    bool HasUsableCollectionAmbulance( const Human_ABC& human ) const;
    bool HasUsableDoctorForSorting() const;

    double GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate ) const;
    void ExecuteOnComponentsAndLendedComponents( ComposanteUsePredicate_ABC& predicate, PHY_Composante_ABC::T_ComposanteUseMap& result ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPionLOG_ABC& pion_;
    bool bHasChanged_;
    bool bExternalMustChangeState_;
    bool bSystemEnabled_;
    bool bSortingFunctionEnabled_;
    bool bHealingFunctionEnabled_;
    T_MedicalPriorityVector priorities_;
    T_AutomateVector tacticalPriorities_;
    T_MedicalConsigns consigns_;
    T_EvacuationAmbulancesMMap evacuationAmbulances_;
    T_CollectionAmbulancesList collectionAmbulances_;
    T_CollectionAmbulancesSet reservations_;
    //@}

    template< typename Archive > friend void save_construct_data( Archive& archive, const PHY_RolePionLOG_Medical* role, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, PHY_RolePionLOG_Medical* role, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( PHY_RolePionLOG_Medical )

#endif // __PHY_RolePionLOG_Medical_h_
