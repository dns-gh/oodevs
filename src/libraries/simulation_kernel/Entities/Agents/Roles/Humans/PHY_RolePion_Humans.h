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

#include "PHY_RoleInterface_Humans.h"
#include "HumansActionsNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

class MIL_AgentPion;

namespace human
{

// =============================================================================
// @class  PHY_RolePion_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Humans : public PHY_RoleInterface_Humans
                          , public HumansActionsNotificationHandler_ABC
                          , public network::NetworkUnitMessageNotificationHandler_ABC
{

public:
    explicit PHY_RolePion_Humans( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Humans();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    void WriteODB( xml::xostream& xos ) const;
    //@}
    
    //! @name Accessors
    //@{
    virtual uint GetNbrUsableHumans() const;
    virtual uint GetNbrAliveHumans ( const PHY_HumanRank& rank ) const;
    virtual uint GetNbrHumans      ( const PHY_HumanRank& rank ) const;
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyHumanAdded  ( PHY_Human& human );
    virtual void NotifyHumanRemoved( PHY_Human& human );
    virtual void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState );
    //@}

    //! @name Medical
    //@{
    virtual void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const;
    virtual bool                   HasWoundedHumansToEvacuate      () const;   
    virtual void                   ChangeEvacuationMode            ( E_EvacuationMode nMode );
    virtual void                   NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ); // Imex
    virtual void                   NotifyHumanWaitingForMedical    ( PHY_Human& human );
    virtual void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState );
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead );
    virtual void Clean         ();
    virtual bool HasChanged    () const;

    virtual void HealAllHumans           ();
    virtual void ChangeHumansAvailability( const PHY_HumanRank& rank, uint nNbrAvailable );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asn ) const;
    //@}

private:
    //! @name Tools
    //@{
    void UpdateDataWhenHumanRemoved( const PHY_Human& human );
    void UpdateDataWhenHumanAdded  ( const PHY_Human& human );

    void SendLogisticChangedState() const;
    void SendLogisticFullState   () const;
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
    MIL_AgentPion&    pion_;
    T_HumanDataVector humansData_;
    uint              nNbrUsableHumans_;

    uint              nNbrHumans_;
    uint              nNbrHumansDataChanged_;
    T_HumanSet        humansToUpdate_; // $$$ A virer - Tester perfs avec update sur tous les humains

    // Medical
    T_MedicalHumanStateSet medicalHumanStates_;
    uint                   nTickRcMedicalQuerySent_;
    E_EvacuationMode       nEvacuationMode_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Humans* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Humans* role, const unsigned int /*version*/ );
};

} //namespace human

#endif // __PHY_RolePion_Humans_h_
