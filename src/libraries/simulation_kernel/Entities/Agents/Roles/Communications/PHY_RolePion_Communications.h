// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h $
// $Author: Jvt $
// $Modtime: 30/03/05 16:59 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Communications.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Communications_h_
#define __PHY_RolePion_Communications_h_

#include "PHY_RoleInterface_Communications.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_CommunicationsAgentType;
class NET_ASN_MsgUnitAttributes;
class MIL_Entity_ABC;

namespace firing
{
    class WeaponReloadingComputer_ABC;
}

namespace moving
{
	class SpeedComputer_ABC;
}
// =============================================================================
// @class  PHY_RolePion_Communications
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Communications : public PHY_RoleInterface_Communications
                                  , public tools::AlgorithmModifier_ABC< firing::WeaponReloadingComputer_ABC >
                                  , public tools::AlgorithmModifier_ABC<moving::SpeedComputer_ABC>
                                  , public network::NetworkUnitMessageNotificationHandler_ABC
{

public:
    //! @name Static
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

             PHY_RolePion_Communications( MIL_Entity_ABC& entity, const bool bIsAutonomous );
    virtual ~PHY_RolePion_Communications();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    //@}

    //! @name Main @TODO MGD REMOVE
    //@{
    virtual bool CanCommunicate() const;
    //@}

    //! @name Notifications
    //@{
    virtual void Jam           ( const MIL_Object_ABC& jammer );
    virtual void Unjam         ( const MIL_Object_ABC& jammer );

    virtual void ActivateBlackout  ();
    virtual void DeactivateBlackout();
    //@}

    //! @name Algoritms visitors
    //@{
    virtual void Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const;
    virtual void Execute(moving::SpeedComputer_ABC& algorithm) const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_Object_ABC* > T_JammerSet;
    typedef T_JammerSet::const_iterator           CIT_JammerSet;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    //@}

    MIL_Entity_ABC& entity_;
    T_JammerSet     jammers_;
    bool            bBlackoutActivated_;
    bool            bHasChanged_;

    const bool      bIsAutonomous_;

private:
    static MT_Float rCoefSpeedModificator_;
    static MT_Float rCoefReloadingTimeModificator_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Communications* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Communications* role, const unsigned int /*version*/ );
};

#endif // __PHY_RolePion_Communications_h_
