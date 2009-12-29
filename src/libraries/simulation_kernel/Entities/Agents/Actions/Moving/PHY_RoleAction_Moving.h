// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h $
// $Author: Age $
// $Modtime: 13/04/05 18:25 $
// $Revision: 14 $
// $Workfile: PHY_RoleAction_Moving.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Moving_h_
#define __PHY_RoleAction_Moving_h_

#include "MT_Tools/Role_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

class PHY_RoleInterface_Location;
class MIL_AgentPion;
class MIL_Object_ABC;

namespace posture
{
    class PostureComputer_ABC;
}

namespace moving
{

// =============================================================================
// @class  PHY_RoleAction_Moving
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Moving : public tools::Role_ABC
                            , public PHY_MovingEntity_ABC
                            , private boost::noncopyable
                            , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                            , public network::NetworkUnitMessageNotificationHandler_ABC
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Moving RoleInterface;
    //@}

public:
             PHY_RoleAction_Moving( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_Moving();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    //@}

    //! @name Operations
    //@{
            double GetMaxSlope                 () const;
            double GetMaxSpeedWithReinforcement() const;
    virtual double GetSpeedWithReinforcement   ( const TerrainData& environment ) const;
    virtual double GetMaxSpeed                 () const;
            void     SetSpeedModificator         ( double rFactor );
            void     SetMaxSpeedModificator      ( double rFactor );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    virtual void SendFullState( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    //@}

    //! @name Tools
    //@{
    MT_Vector2D ExtrapolatePosition( const double rTime, const bool bBoundOnPath ) const;
    //@}
                
private:
    //! @name 
    //@{
    virtual const MT_Vector2D& GetPosition () const;
    virtual const MT_Vector2D& GetDirection() const;
    
    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double rWalkedDistance );
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyMovingOnPathPoint   ( const DEC_PathPoint& point );
    virtual void NotifyMovingOnSpecialPoint( const DEC_PathPoint& point );
    virtual void NotifyMovingInsideObject  ( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject ( MIL_Object_ABC& object );
    virtual void NotifyEnvironmentChanged  ();
    virtual void NotifyCurrentPathChanged  ();
    //@}

    //! @name 
    //@{
    virtual bool CanMove              () const;
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const;
    virtual bool HasResources         ();
    virtual void SendRC               ( int nReportID ) const;
    //@}

    //! @name Speed management
    //@{
            double GetMaxSpeed              ( const TerrainData& environment ) const;
            double GetMaxSpeed              ( const MIL_Object_ABC& object ) const;
            double ApplyMaxSpeedModificators( double rSpeed ) const;
            double ApplySpeedModificators   ( double rSpeed ) const;

    virtual double GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    //@}

    //! @name Network
    //@{
    void SendCurrentPath    () const;
    void SendEnvironmentType() const;
    //@}

    //! @name Tools
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RoleAction_Moving* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RoleAction_Moving* role, const unsigned int /*version*/ );
    //@}

private:
    MIL_AgentPion&              pion_;
    PHY_RoleInterface_Location* pRoleLocation_;
    double                      rSpeed_;
    double                    rSpeedModificator_;
    double                    rMaxSpeedModificator_;

    // Network
    bool bCurrentPathHasChanged_;
    bool bEnvironmentHasChanged_;
    bool bHasMove_;
};

} // namespace moving

#endif // __PHY_RoleAction_Moving_h_
