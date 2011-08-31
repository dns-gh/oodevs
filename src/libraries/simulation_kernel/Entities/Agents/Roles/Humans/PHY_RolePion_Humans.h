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
#include "simulation_kernel/Entities/Agents/Units/Humans/Human_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "simulation_kernel/NetworkMessageSender_ABC.h"

namespace xml
{
    class xostream;
}

class MIL_AgentPion;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class PHY_HumanRank;
class PHY_HumanWound;

namespace human
{

// =============================================================================
// @class  PHY_RolePion_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Humans : public PHY_RoleInterface_Humans
                          , public HumansActionsNotificationHandler_ABC
                          , public network::NetworkUnitAttributesMessageSender_ABC
                          , public network::NetworkMessageSender_ABC
{
public:
    explicit PHY_RolePion_Humans( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Humans();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetNbrUsableHumans() const;
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyHumanAdded  ( Human_ABC& human );
    virtual void NotifyHumanRemoved( Human_ABC& human );
    virtual void NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState );
    //@}

    //! @name Medical
    //@{
    virtual void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const;
    virtual bool HasWoundedHumansToEvacuate() const;
    virtual void ChangeEvacuationMode( E_EvacuationMode nMode );
    virtual void NotifyHumanEvacuatedByThirdParty( Human_ABC& human, MIL_AutomateLOG& destinationTC2 ); // Imex
    virtual void NotifyHumanWaitingForMedical( Human_ABC& human );
    virtual void NotifyHumanBackFromMedical( PHY_MedicalHumanState& humanState );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean();

    virtual void HealAllHumans();
    virtual void ChangeHumansAvailability( const PHY_HumanRank& rank, unsigned int nNbrAvailable );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState ( client::UnitAttributes& asn ) const;
    virtual void SendFullState    ( client::UnitAttributes& asn ) const;
    virtual void SendChangedState () const;
    virtual void SendFullState    ( unsigned int context ) const;
    //@}

private:
    //! @name Tools
    //@{
    unsigned int GetNbrTotal( const PHY_HumanRank& rank ) const;
    unsigned int GetNbrOperational( const PHY_HumanRank& rank ) const;

    void UpdateDataWhenHumanRemoved( const Human_ABC& human );
    void UpdateDataWhenHumanAdded( const Human_ABC& human );
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Humans* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Humans* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Types
    //@{
    class HumanState
    {
    public:
        //! @name Constructors/Destructor
        //@{
        explicit HumanState();
                 HumanState( unsigned int number, const PHY_HumanRank& rank, const PHY_HumanWound& state, Human_ABC::E_Location location = Human_ABC::eBattleField, bool contaminated = false, bool psyop = false );
        virtual ~HumanState();
        //@}

        //! @name Serialization
        //@{
        BOOST_SERIALIZATION_SPLIT_MEMBER();
        void load( MIL_CheckPointInArchive&, const unsigned int );
        void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
        //@}

        //! @name Member data
        //@{
        unsigned int          number_;
        const PHY_HumanRank*  rank_;
        const PHY_HumanWound* state_;
        Human_ABC::E_Location location_;
        bool                  contaminated_;
        bool                  psyop_;
        //@}
    };

    typedef std::vector< HumanState* >                T_HumanStateVector;
    typedef T_HumanStateVector::iterator             IT_HumanStateVector;
    typedef T_HumanStateVector::const_iterator      CIT_HumanStateVector;

    typedef std::set< Human_ABC* >                    T_HumanSet;
    typedef T_HumanSet::const_iterator              CIT_HumanSet;

    typedef std::set< PHY_MedicalHumanState* >        T_MedicalHumanStateSet;
    typedef T_MedicalHumanStateSet::const_iterator  CIT_MedicalHumanStateSet;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&      pion_;
    T_HumanStateVector  humansStates_;
    T_HumanSet          humansToUpdate_; // $$$ A virer - Tester perfs avec update sur tous les humains
    bool                hasChanged_;
    unsigned int        nNbrUsableHumans_;
    // Medical
    T_MedicalHumanStateSet medicalHumanStates_;
    unsigned int           nTickRcMedicalQuerySent_;
    E_EvacuationMode       nEvacuationMode_;
    //@}
};

} //namespace human

BOOST_CLASS_EXPORT_KEY( human::PHY_RolePion_Humans )

#endif // __PHY_RolePion_Humans_h_
