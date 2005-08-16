// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h $
// $Author: Jvt $
// $Modtime: 31/03/05 16:54 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Humans.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Humans_h_
#define __PHY_RolePion_Humans_h_

#include "MIL.h"

#include "PHY_RoleInterface_Humans.h"

class PHY_HumanRank;
class PHY_HumanWound;
class PHY_Human;
class PHY_MedicalHumanState;
class NET_ASN_MsgUnitDotations;
class MIL_AgentPion;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_RolePion_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Humans : public PHY_RoleInterface_Humans
{
    MT_COPYNOTALLOWED( PHY_RolePion_Humans )

public:
    //! @name Types
    //@{
    enum E_EvacuationMode
    {
        eEvacuationMode_Auto,  // TC2 or IMEX
        eEvacuationMode_Manual // IMEX
    };
    //@}

public:
             PHY_RolePion_Humans( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Humans();
    virtual ~PHY_RolePion_Humans();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{

    bool IsUsable              () const;
    uint GetNbrUsableHumans    () const;
    uint GetNbrHumans          () const;
    uint GetNbrFullyAliveHumans() const;
    //@}

    //! @name Notifications
    //@{
    void NotifyHumanAdded  ( PHY_Human& human );
    void NotifyHumanRemoved( PHY_Human& human );
    void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState );
    //@}

    //! @name Medical
    //@{
    void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const;
    bool                   HasWoundedHumansToEvacuate      () const;   
    void                   ChangeEvacuationMode            ( E_EvacuationMode nMode );
    PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ); // Imex
    PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human );
    void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState );
    //@}

    //! @name Operations
    //@{
    void Update        ( bool bIsDead );
    void Clean         ();
    bool HasChanged    () const;

    void HealAllHumans           ();
    void ChangeHumansAvailability( const PHY_HumanRank& rank, uint nNbrAvailable );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitDotations& asn ) const;
    void SendFullState   ( NET_ASN_MsgUnitDotations& asn ) const;

    void SendLogisticChangedState() const;
    void SendLogisticFullState   () const;
    //@}

private:
    //! @name Tools
    //@{
    void UpdateDataWhenHumanRemoved( const PHY_Human& human );
    void UpdateDataWhenHumanAdded  ( const PHY_Human& human );
    //@}

private:
    //! @name Types
    //@{
    struct T_HumanData
    {
        T_HumanData();
        
        template< typename Archive > void serialize( Archive&, const uint );
        
        uint nNbrTotal_;
        uint nNbrOperational_;
        uint nNbrDead_;
        uint nNbrWounded_;
        uint nNbrMentalDiseased_;
        uint nNbrNBC_;
        uint nNbrInLogisticMedical_;
        uint nNbrInLogisticMaintenance_;
        bool bHasChanged_;
    };

    typedef std::vector< T_HumanData >        T_HumanDataVector;
    typedef T_HumanDataVector::iterator       IT_HumanDataVector;
    typedef T_HumanDataVector::const_iterator CIT_HumanDataVector;

    typedef std::set< PHY_Human* >     T_HumanSet;
    typedef T_HumanSet::iterator       IT_HumanSet;
    typedef T_HumanSet::const_iterator CIT_HumanSet;

    typedef std::set< PHY_MedicalHumanState* >     T_MedicalHumanStateSet;
    typedef T_MedicalHumanStateSet::const_iterator CIT_MedicalHumanStateSet;
    //@}

private:
    MIL_AgentPion*    pPion_;
    T_HumanDataVector humansData_;
    uint              nNbrUsableHumans_;

    uint              nNbrHumans_;
    uint              nNbrFullyAliveHumans_; // Not wounded nor contaminated nor mental diseased

    uint              nNbrHumansDataChanged_;
    T_HumanSet        humansToUpdate_; // $$$ A virer - Tester perfs avec update sur tous les humains

    // Medical
    T_MedicalHumanStateSet medicalHumanStates_;
    bool                   bRcMedicalQuerySent_;
    E_EvacuationMode       nEvacuationMode_;
};

#include "PHY_RolePion_Humans.inl"

#endif // __PHY_RolePion_Humans_h_
