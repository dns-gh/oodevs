// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h $
// $Author: Jvt $
// $Modtime: 12/05/05 17:09 $
// $Revision: 7 $
// $Workfile: PHY_RolePion_Transported.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Transported_h_
#define __PHY_RolePion_Transported_h_

#include "MIL.h"
#include "PHY_RoleInterface_Transported.h"
#include "TransportNotificationHandler_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

class MIL_AgentPion;

namespace moving
{
    class MoveComputer_ABC;
}

namespace transport
{
    class TransportPermissionComputer_ABC;
// =============================================================================
// @class  PHY_RolePion_Transported
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Transported : public PHY_RoleInterface_Transported,
    public tools::AlgorithmModifier_ABC<TransportPermissionComputer_ABC>,
    public tools::AlgorithmModifier_ABC<moving::MoveComputer_ABC>,
    public TransportNotificationHandler_ABC,
    public network::NetworkUnitMessageNotificationHandler_ABC
{
public:
    explicit PHY_RolePion_Transported( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Transported();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive& , const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead );
    virtual void Clean     ();
    virtual void Execute(moving::MoveComputer_ABC& algorithm) const;
    //@}

    //! @name Transport
    //@{
    virtual void LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, bool& bTransportedByAnother );
    virtual void UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable );
    virtual void CancelTransport    ( const MIL_Agent_ABC& transporter );

    virtual bool IsTransported      () const;
    void Execute(TransportPermissionComputer_ABC& alg) const;
    //@}

    //! @name Human transporters ... $$$
    //@{
    virtual bool HasHumanTransportersReady    () const;
    virtual void DisableHumanTransporters     ( const MT_Vector2D& vPosition );
    virtual void RecoverHumanTransporters     ();
    virtual bool HasHumanTransportersToRecover() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Tools
    //@{
    virtual bool HasChanged() const;
    //@}

          MIL_AgentPion& pion_;
          bool           bHasChanged_;
    const MIL_Agent_ABC* pTransporter_;

    MT_Vector2D  vLoadingPosition_;
    MT_Vector2D  vHumanTransporterPosition_; // position des 'VABs' quand seuls les humains sont h�liport�s

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Transported* role, const unsigned int /*version*/ );
      template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Transported* role, const unsigned int /*version*/ );

};

} // namespace transport

BOOST_CLASS_EXPORT_KEY( transport::PHY_RolePion_Transported )

#endif // __PHY_RolePion_Transported_h_
