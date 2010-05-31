// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:22 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Reinforcement.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Reinforcement_h_
#define __PHY_RolePion_Reinforcement_h_

#include "PHY_RoleInterface_Reinforcement.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/TransportNotificationHandler_ABC.h"
#include "simulation_kernel/ConsumptionChangeRequestHandler_ABC.h"
#include "simulation_kernel/ObjectCollisionNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"
#include "simulation_kernel/TransportChangeNotificationHandler_ABC.h"

class MIL_AgentPion;

namespace moving
{
    class SpeedComputer_ABC;
    class MoveComputer_ABC;
}

// =============================================================================
// @class  PHY_RolePion_Reinforcement
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Reinforcement : public PHY_RoleInterface_Reinforcement
                                 , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC>
                                 , public tools::AlgorithmModifier_ABC< moving::MoveComputer_ABC>
                                 , public dotation::ConsumptionChangeRequestHandler_ABC
                                 , public transport::TransportNotificationHandler_ABC
                                 , public terrain::ObjectCollisionNotificationHandler_ABC
                                 , public network::NetworkUnitMessageNotificationHandler_ABC
                                 , public transport::TransportChangeNotificationHandler_ABC
{

public:
    //! @name Types
    //@{
    typedef std::set< MIL_AgentPion* > T_PionSet;
    typedef T_PionSet::const_iterator  CIT_PionSet;
    //@}

public:
    explicit PHY_RolePion_Reinforcement( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Reinforcement();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    virtual void Execute(moving::MoveComputer_ABC& algorithm) const;
    //@}

    //! @name Operations
    //@{
    bool Reinforce          ( MIL_AgentPion& pion );
    void CancelReinforcement();
    bool IsReinforcing      () const;
    bool IsReinforced       () const;
    bool IsReinforcedBy     ( MIL_AgentPion& pion ) const;
    void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    void ChangeConsumptionMode(dotation::ConsumptionModeChangeRequest_ABC& request);
    //@}

    //! @name Event handlers
    //@{
    void LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, bool& bTransportedByAnother );
    void NotifyMovingInsideObject ( MIL_Object_ABC& object );
    void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    void NotifyPutInsideObject( MIL_Object_ABC& object );
    void NotifyPutOutsideObject( MIL_Object_ABC& object );

    virtual void NotifyIsLoadedForTransport();
    virtual void NotifyIsUnLoadedForTransport();
    //@}

    //! @name Network
    //@{

    void SendChangedState( client::UnitAttributes& msg ) const;
    void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    const T_PionSet& GetReinforcements() const;
          bool       CanReinforce     () const;
          bool       CanBeReinforced  () const;
    //@}

private:
    //! @name Notifications
    //@{
    bool HasChanged() const;
    //@}

    //! @name Notifications
    //@{
    void NotifyReinforcementAdded  ( MIL_AgentPion& reinforcement );
    void NotifyReinforcementRemoved( MIL_AgentPion& reinforcement );
    //@}

private:
    MIL_AgentPion& pion_;
    MIL_AgentPion* pPionReinforced_;
    T_PionSet      reinforcements_;
    bool           bHasChanged_;
    bool           bExternalCanReinforce_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Reinforcement* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Reinforcement* role, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Reinforcement )

#endif // __PHY_RolePion_Reinforcement_h_
