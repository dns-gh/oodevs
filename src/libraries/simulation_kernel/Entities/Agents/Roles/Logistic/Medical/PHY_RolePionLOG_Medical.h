// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:23 $
// $Revision: 9 $
// $Workfile: PHY_RolePionLOG_Medical.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOG_Medical_h_
#define __PHY_RolePionLOG_Medical_h_

#include "MIL.h"

#include "PHY_RolePion_Medical.h"

class MIL_AgentPionLOG_ABC;
class MIL_AutomateLOG;
class MIL_Automate;
class PHY_ComposantePion;
class PHY_MedicalComposanteState;
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
class PHY_RolePionLOG_Medical : public PHY_RolePion_Medical
{
    MT_COPYNOTALLOWED( PHY_RolePionLOG_Medical )

public:
             PHY_RolePionLOG_Medical( MT_RoleContainer& role, MIL_AgentPionLOG_ABC& pion );
             PHY_RolePionLOG_Medical();
    virtual ~PHY_RolePionLOG_Medical();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean         ();

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
    virtual bool                   HandleHumanForCollection         ( PHY_MedicalHumanState& humanState );     // Ramassage
    virtual int                    GetAvailabilityScoreForCollection( const PHY_MedicalHumanState& humanState ) const;

    virtual void                   HandleHumanForSorting            ( const PHY_MedicalCollectionAmbulance& ambulance, PHY_MedicalHumanState& humanState );
    virtual int                    GetAvailabilityScoreForSorting   ( const PHY_MedicalCollectionAmbulance& ambulance ) const;
    virtual void                   ReserveForSorting                ( const PHY_MedicalCollectionAmbulance& ambulance );
    virtual void                   CancelReservationForSorting      ( const PHY_MedicalCollectionAmbulance& ambulance );
    
    virtual int                    GetAvailabilityScoreForHealing   ( const PHY_MedicalHumanState& humanState );
    virtual bool                   HandleHumanForHealing            ( PHY_MedicalHumanState& humanState );

    PHY_MedicalEvacuationAmbulance* GetAvailableEvacuationAmbulance( PHY_MedicalEvacuationConsign& consign );
    PHY_MedicalCollectionAmbulance* GetAvailableCollectionAmbulance( PHY_MedicalCollectionConsign& consign );
    PHY_ComposantePion*             GetAvailableDoctorForDiagnosing() const;
    PHY_ComposantePion*             GetAvailableDoctorForSorting   () const;
    PHY_ComposantePion*             GetAvailableDoctorForHealing   ( const PHY_Human& human ) const;
    bool                            HasUsableDoctorForHealing      ( const PHY_Human& human, bool bBypassPriorities = false ) const;
    //@}

    //! @name Tools
    //@{
    virtual bool CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const;
            void StartUsingForLogistic   ( PHY_ComposantePion& composante );
            void StopUsingForLogistic    ( PHY_ComposantePion& composante );
    //@}

    //! @name Accessors
    //@{
          MIL_AutomateLOG&      GetAutomate() const;
    const MIL_AgentPionLOG_ABC& GetPion    () const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}

    //! @name Types
    //@{
private:
    typedef std::list< PHY_MedicalConsign_ABC* >  T_MedicalConsignList;
    typedef T_MedicalConsignList::iterator        IT_MedicalConsignList;
    typedef T_MedicalConsignList::const_iterator  CIT_MedicalConsignList;

    typedef std::list< PHY_MedicalCollectionAmbulance* > T_CollectionAmbulancesList;
    typedef T_CollectionAmbulancesList::iterator         IT_CollectionAmbulancesList;
    typedef T_CollectionAmbulancesList::const_iterator   CIT_CollectionAmbulancesList;

    typedef std::vector< std::pair< const MIL_Automate*, T_MedicalConsignList > > T_MedicalConsigns;
    typedef T_MedicalConsigns::iterator                                           IT_MedicalConsigns;
    typedef T_MedicalConsigns::const_iterator                                     CIT_MedicalConsigns;

public:
    typedef std::multimap< const MIL_Automate*, PHY_MedicalEvacuationAmbulance* > T_EvacuationAmbulancesMMap;
    typedef T_EvacuationAmbulancesMMap::iterator                                  IT_EvacuationAmbulancesMMap;
    typedef T_EvacuationAmbulancesMMap::const_iterator                            CIT_EvacuationAmbulancesMMap;
    typedef T_EvacuationAmbulancesMMap::_Pairii                                   T_EvacuationAmbulancesMMapRange;

    typedef std::set< const PHY_MedicalCollectionAmbulance* > T_CollectionAmbulancesSet;
    typedef T_CollectionAmbulancesSet::iterator               IT_CollectionAmbulancesSet;
    typedef T_CollectionAmbulancesSet::const_iterator         CIT_CollectionAmbulancesSet;
    //@}

private:
    //! @name Tools
    //@{
    void InsertConsign ( PHY_MedicalConsign_ABC&  );
    void InsertConsigns( const T_MedicalConsigns& );

    bool HasUsableEvacuationAmbulance( const PHY_Human& human ) const;
    bool HasUsableCollectionAmbulance( const PHY_Human& human ) const;
    bool HasUsableDoctorForSorting   () const;

    MT_Float GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate ) const;
    //@}    

private:
    MIL_AgentPionLOG_ABC*   pPion_;
    bool                    bHasChanged_;
    bool                    bSystemEnabled_;
    bool                    bSortingFunctionEnabled_;
    bool                    bHealingFunctionEnabled_;

    T_MedicalPriorityVector priorities_;
    T_AutomateVector        tacticalPriorities_;

    T_MedicalConsigns          consigns_;
    T_EvacuationAmbulancesMMap evacuationAmbulances_;
    T_CollectionAmbulancesList collectionAmbulances_;

    T_CollectionAmbulancesSet  reservations_;
};

#include "PHY_RolePionLOG_Medical.inl"

#endif // __PHY_RolePionLOG_Medical_h_
