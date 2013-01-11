// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/RolePion_Location.h $
// $Author: Jvt $
// $Modtime: 5/04/05 16:38 $
// $Revision: 12 $
// $Workfile: RolePion_Location.h $
//
// *****************************************************************************

#ifndef SWORD_ROLE_PION_LOCATION_H
#define SWORD_ROLE_PION_LOCATION_H

#include "MIL.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "PopulationCollisionNotificationHandler_ABC.h"
#include "ObjectCollisionNotificationHandler_ABC.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "LocationActionNotificationHandler_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "NetworkMessageSender_ABC.h"
#include <vector>

namespace urbanLocation
{
    class UrbanLocationComputer_ABC;
}

class MIL_AgentPion;

namespace core
{
    class Model;
}

namespace sword
{
    class RoleListener_ABC;
    class Sink;

// =============================================================================
/** @class  RolePion_Location
    @brief  Role pion location
*/
// Created: MCO 2012-02-27
// =============================================================================
class RolePion_Location : public PHY_RoleInterface_Location
                        , public PopulationCollisionNotificationHandler_ABC
                        , public terrain::ObjectCollisionNotificationHandler_ABC
                        , public network::NetworkUnitAttributesMessageSender_ABC
                        , public location::LocationActionNotificationHandler_ABC
                        , public tools::AlgorithmModifier_ABC< urbanLocation::UrbanLocationComputer_ABC >
{
public:
             RolePion_Location();
             RolePion_Location( Sink& sink, MIL_AgentPion& pion, core::Model& entity, const MT_Vector2D& position );
    virtual ~RolePion_Location();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Finalize();
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
    virtual boost::shared_ptr<MT_Vector2D> GetSharedPosition() const;
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
    //! @name Member data
    //@{
    Sink*                            sink_;
    MIL_AgentPion*                   owner_;
    core::Model*                     entity_;
    MT_Vector2D                      vDirection_;
    boost::shared_ptr< MT_Vector2D > pvPosition_;
    bool                             bHasDoneMagicMove_;
    std::vector< boost::shared_ptr< RoleListener_ABC > > listeners_;
    //@}
};
}

BOOST_CLASS_EXPORT_KEY( sword::RolePion_Location )

#endif // SWORD_ROLE_PION_LOCATION_H
