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

#include "MT_Tools/MT_Role_ABC.h"

#include "Entities/Actions/PHY_MovingEntity_ABC.h"

class PHY_RolePion_Location;
class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// @class  PHY_RoleAction_Moving
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Moving : public MT_Role_ABC
                            , public PHY_MovingEntity_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Moving )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Moving RoleInterface;
    //@}

public:
             PHY_RoleAction_Moving( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RoleAction_Moving();
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
    //@}

    //! @name Operations
    //@{
            MT_Float GetMaxSlope                 () const;
            MT_Float GetMaxSpeedWithReinforcement() const;
    virtual MT_Float GetSpeedWithReinforcement   ( const TerrainData& environment ) const;
    virtual MT_Float GetMaxSpeed                 () const;
            void     SetSpeedModificator         ( MT_Float rFactor );
            void     SetMaxSpeedModificator      ( MT_Float rFactor );
    //@}

    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name Tools
    //@{
    MT_Vector2D ExtrapolatePosition( const MT_Float rTime, const bool bBoundOnPath ) const;
    //@}
                
private:
    //! @name 
    //@{
    virtual const MT_Vector2D& GetPosition () const;
    virtual const MT_Vector2D& GetDirection() const;
    
    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed, MT_Float rWalkedDistance );
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
            MT_Float GetMaxSpeed              ( const TerrainData& environment ) const;
            MT_Float GetMaxSpeed              ( const MIL_RealObject_ABC& object ) const;
            MT_Float ApplyMaxSpeedModificators( MT_Float rSpeed ) const;
            MT_Float ApplySpeedModificators   ( MT_Float rSpeed ) const;

    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    //@}

    //! @name Network
    //@{
    void SendCurrentPath    () const;
    void SendEnvironmentType() const;
    //@}

private:
    MIL_AgentPion*         pPion_;
    PHY_RolePion_Location* pRoleLocation_;
    MT_Float               rSpeedModificator_;
    MT_Float               rMaxSpeedModificator_;

    // Network
    bool bCurrentPathHasChanged_;
    bool bEnvironmentHasChanged_;
};

#include "PHY_RoleAction_Moving.inl"

#endif // __PHY_RoleAction_Moving_h_
