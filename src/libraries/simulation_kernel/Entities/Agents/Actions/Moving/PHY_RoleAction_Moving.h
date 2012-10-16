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

#include "MIL.h"
#include "PHY_RoleAction_InterfaceMoving.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/NetworkMessageSender_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_AgentPion;
class MIL_Object_ABC;

namespace client
{
    class UnitAttributes;
}

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
class PHY_RoleAction_Moving : public PHY_RoleAction_InterfaceMoving
                            , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                            , public tools::AlgorithmModifier_ABC<moving::SpeedComputer_ABC>
                            , public network::NetworkMessageSender_ABC
                            , public PHY_MovingEntity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoleAction_Moving( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_Moving();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean();
    bool HasChanged() const;
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    virtual void SendRC( int nReportID ) const;
    virtual void SendRC( int nReportID, const std::string& name ) const;
    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double rWalkedDistance );
    //@}

    //! @name Accessors
    //@{
    virtual double GetSpeedWithReinforcement( const TerrainData& environment ) const;
    virtual double GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    virtual double GetMaxSpeedWithReinforcement() const;
    virtual double GetMaxSpeed() const;
    virtual double GetMaxSpeed( const TerrainData& environment ) const;
    virtual double GetMaxSlope() const;
    virtual double GetTheoricMaxSpeed( bool loaded ) const;
    virtual double GetTheoricMaxSpeedWithReinforcement();
    virtual void SetSpeedModificator( double rFactor );
    virtual void SetMaxSpeedModificator( double rFactor );
    virtual double GetMaxSpeedModificator() const;
    virtual bool HasResources();
    virtual bool CanMove() const;

    virtual const MT_Vector2D& GetPosition () const;
    virtual const MT_Vector2D& GetDirection() const;
    /** Whether to use theoric speed (for pathfinding) or actual speed (to compute real speed when moving) */
    virtual void SetTheoricSpeed( bool ) const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   ( unsigned int context ) const;
    //@}

    //! @name Tools
    //@{
    virtual MT_Vector2D ExtrapolatePosition( const double rTime, const bool bBoundOnPath ) const;
    //@}
    //! @name Notifications
    //@{
    virtual void NotifyMovingOnPathPoint( const MT_Vector2D& point );
    virtual void NotifyMovingOnSpecialPoint( boost::shared_ptr< DEC_PathPoint > point );
    virtual void NotifyMovingInsideObject( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyEnvironmentChanged();
    virtual void NotifyCurrentPathChanged();
    //@}
    //! @name
    //@{
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const;
    //@}

private:
    //! @name Speed management
    //@{
    double GetMaxSpeed( const MIL_Object_ABC& object ) const;
    double ApplyMaxSpeedModificators( double rSpeed ) const;
    double ApplySpeedModificators( double rSpeed ) const;
    //@}

    //! @name Network
    //@{
    void SendCurrentPath( unsigned int context = 0 ) const;
    void SendEnvironmentType( unsigned int context = 0 ) const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_Moving )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&              owner_;
    double                      rSpeed_;
    double                      rSpeedModificator_;
    double                      rMaxSpeedModificator_;
    // Network
    bool                        bCurrentPathHasChanged_;
    bool                        bEnvironmentHasChanged_;
    bool                        bHasMove_;
    mutable bool bTheoricMaxSpeed_;
    //@}
};

} // namespace moving

BOOST_CLASS_EXPORT_KEY( moving::PHY_RoleAction_Moving )
namespace moving
{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_Moving, MIL_AgentPion )
}
#endif // __PHY_RoleAction_Moving_h_
