// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RolePion_Location.h $
// $Author: Jvt $
// $Modtime: 5/04/05 16:38 $
// $Revision: 12 $
// $Workfile: PHY_RolePion_Location.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Location_h_
#define __PHY_RolePion_Location_h_

#include "MIL.h"
#include "PHY_RoleInterface_Location.h"
#include "PopulationCollisionNotificationHandler_ABC.h"
#include "ObjectCollisionNotificationHandler_ABC.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "LocationActionNotificationHandler_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

namespace urbanLocation
{
    class UrbanLocationComputer_ABC;
}

class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_Location
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Location : public PHY_RoleInterface_Location
                            , public PopulationCollisionNotificationHandler_ABC
                            , public terrain::ObjectCollisionNotificationHandler_ABC
                            , public network::NetworkUnitAttributesMessageSender_ABC
                            , public location::LocationActionNotificationHandler_ABC
                            , public tools::AlgorithmModifier_ABC< urbanLocation::UrbanLocationComputer_ABC >
{
public:
             PHY_RolePion_Location();
    explicit PHY_RolePion_Location( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Location();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();

    virtual void Hide();
    virtual void Show( const MT_Vector2D& vNewPosition );
    virtual void MagicMove( const MT_Vector2D& vNewPosition );
    virtual void Move( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, double rNewSpeed );
    virtual void Follow( const MIL_Agent_ABC& agent );
    void Execute( urbanLocation::UrbanLocationComputer_ABC& algorithm ) const;

    virtual void NotifyTerrainObjectCollision( MIL_Object_ABC& object );
    virtual void NotifyTerrainPutInsideObject( MIL_Object_ABC& object );
    virtual void NotifyTerrainPutOutsideObject( MIL_Object_ABC& object );
    //@}

    //! @name Event handlers
    //@{
    virtual void NotifyMovingInsideObject( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyPutInsideObject( MIL_Object_ABC& object ) ;
    virtual void NotifyPutOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyFlowCollision( MIL_PopulationFlow& population );
    virtual void NotifyConcentrationCollision( MIL_PopulationConcentration& population );
    //@}

    //! @name Accessors
    //@{
    virtual double GetHeight() const;
    virtual double GetAltitude() const;
    virtual const MT_Vector2D& GetPosition() const;
    virtual boost::shared_ptr< MT_Vector2D > GetSharedPosition() const;
    virtual const MT_Vector2D& GetDirection() const;
    virtual MIL_Agent_ABC& GetAgent() const;
    virtual double GetCurrentSpeed() const;
    virtual bool HasDoneMagicMove() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& asnMsg ) const;
    virtual void SendFullState( client::UnitAttributes& asnMsg ) const;
    //@}

private:
    //! @name Tools
    //@{
    bool HasSpeedChanged() const; // Position or direction or height has changed
    bool HasLocationChanged() const;
    void SetDirection( const MT_Vector2D& vDirection );
    void SetPosition( const MT_Vector2D& vPosition  );
    void SetCurrentSpeed( double rSpeed );
    void SetHeight( double rHeight );
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion*                  owner_;
    MT_Vector2D                     vDirection_;
    boost::shared_ptr<MT_Vector2D>  pvPosition_;
    double                          rHeight_;
    double                          rCurrentSpeed_;
    bool                            bHasDoneMagicMove_;
    bool                            bHasMove_;

    bool                            bPositionHasChanged_;
    bool                            bDirectionHasChanged_;
    bool                            bCurrentSpeedHasChanged_;
    bool                            bHeightHasChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Location )

#endif // __PHY_RolePion_Location_h_
