// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h $
// $Author: Jvt $
// $Modtime: 31/03/05 16:42 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Humans.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Humans_h_
#define __PHY_RoleInterface_Humans_h_

#include "MT_Tools/Role_ABC.h"

class PHY_HumanRank;
class PHY_HumanWound;
class PHY_Human;
class PHY_MedicalHumanState;
class MIL_AgentPion;
class MIL_AutomateLOG;
class NET_ASN_MsgUnitAttributes;

namespace human
{
// =============================================================================
// @class  PHY_RoleInterface_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Humans : public tools::Role_ABC
                               , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Humans RoleInterface;
    //! @name Types
    //@{
    enum E_EvacuationMode
    {
        eEvacuationMode_Auto,  // TC2 or IMEX
        eEvacuationMode_Manual // IMEX
    };
    //@}
    //@}

public:
             PHY_RoleInterface_Humans();
    virtual ~PHY_RoleInterface_Humans();

    virtual uint GetNbrUsableHumans() const = 0;
    virtual uint GetNbrAliveHumans ( const PHY_HumanRank& rank ) const = 0;
    virtual uint GetNbrHumans      ( const PHY_HumanRank& rank ) const = 0;
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyHumanAdded  ( PHY_Human& human ) = 0;
    virtual void NotifyHumanRemoved( PHY_Human& human ) = 0;
    virtual void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState ) = 0;
    //@}

    //! @name Medical
    //@{
    virtual void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const = 0;
    virtual bool                   HasWoundedHumansToEvacuate      () const = 0;   
    virtual void                   ChangeEvacuationMode            ( E_EvacuationMode nMode ) = 0;
    virtual PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) = 0; // Imex
    virtual PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human ) = 0;
    virtual void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead ) = 0;
    virtual void Clean         () = 0;
    virtual bool HasChanged    () const = 0;

    virtual void HealAllHumans           () = 0;
    virtual void ChangeHumansAvailability( const PHY_HumanRank& rank, uint nNbrAvailable ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asn ) const = 0;

    virtual void SendLogisticChangedState() const = 0;
    virtual void SendLogisticFullState   () const = 0;
    //@}
private:
    //! @name Serialization
	//@{
	friend class boost::serialization::access;
	template< typename Archive > void serialize( Archive& ar, const uint )
	{
		ar & boost::serialization::base_object< tools::Role_ABC >( *this );
	}
	//@}
};

} //namespace human

#endif // __PHY_RoleInterface_Humans_h_
