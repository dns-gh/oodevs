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
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
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
    class SpeedStrategy_ABC;

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
             PHY_RoleAction_Moving();
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
    virtual void SendRC( const MIL_DecisionalReport& reportId ) const;
    virtual void SendRC( const MIL_DecisionalReport& reportId, const std::string& name ) const;
    virtual int Move( boost::shared_ptr< DEC_PathResult > pPath );

    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double rWalkedDistance );
    //@}

    //! @name Accessors
    //@{
    virtual double GetSpeed( const TerrainData& environment ) const;
    virtual double GetSpeed( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    virtual double GetMaxSpeed() const;
    virtual double GetMaxSpeed( const TerrainData& environment ) const;
    virtual double GetTheoricSpeed( const TerrainData& environment ) const;
    virtual double GetTheoricMaxSpeed() const;
    virtual void SetSpeedModificator( double rFactor );
    virtual void SetMaxSpeedModificator( double rFactor );
    virtual double GetMaxSpeedModificator() const;
    virtual double GetMaxSlope() const;
    virtual double GetSlopeDeceleration() const;
    virtual void ApplyTrafficModifier();
    virtual bool HasResources();
    virtual bool CanMove() const;
    virtual bool IsReady() const;
    virtual bool IsUnderground() const;

    virtual const MT_Vector2D& GetPosition () const;
    virtual const MT_Vector2D& GetDirection() const;
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
    virtual bool HasKnowledgeObject   ( const MIL_Object_ABC& object ) const;
    virtual double GetObjectCost      ( const MIL_ObjectType_ABC& objectType, const DEC_PathType& pathType ) const;
    //@}

private:
    //! @name Speed management
    //@{
    double ApplyMaxSpeedModificators( double rSpeed ) const;
    double ApplySpeedModificators( double rSpeed ) const;
    double ComputeSpeed( const SpeedStrategy_ABC& strategy ) const;
    //@}

    //! @name Network
    //@{
    void SendCurrentPath( unsigned int context = 0 ) const;
    void SendEnvironmentType( unsigned int context = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion*              owner_;
    double                      rSpeed_;
    double                      rSpeedModificator_;
    double                      rMaxSpeedModificator_;
    double                      rTrafficModificator_;
    // Network
    bool                        bCurrentPathHasChanged_;
    bool                        bEnvironmentHasChanged_;
    bool                        bHasMove_;
    bool                        bIntentToMove_;
    //@}
};

} // namespace moving

BOOST_CLASS_EXPORT_KEY( moving::PHY_RoleAction_Moving )

#endif // __PHY_RoleAction_Moving_h_
