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

#include "simulation_kernel/PopulationCollisionNotificationHandler_ABC.h"
#include "simulation_kernel/ObjectCollisionNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"
#include "simulation_kernel/LocationActionNotificationHandler_ABC.h"

class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_Location
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Location : public PHY_RoleInterface_Location
                            , public population::PopulationCollisionNotificationHandler_ABC
                            , public terrain::ObjectCollisionNotificationHandler_ABC
                            , public network::NetworkUnitMessageNotificationHandler_ABC
                            , public location::LocationActionNotificationHandler_ABC
{

public:
    explicit PHY_RolePion_Location( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Location();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();

    virtual void Hide     ();
    virtual void Show     ( const MT_Vector2D& vNewPosition );
    virtual void MagicMove( const MT_Vector2D& vNewPosition );
    virtual void Move     ( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, MT_Float rNewSpeed );
    virtual void Follow   ( const MIL_Agent_ABC& agent );

    virtual void NotifyTerrainObjectCollision    ( MIL_Object_ABC& object );
    virtual void NotifyTerrainPutInsideObject    ( MIL_Object_ABC& object );
    virtual void NotifyTerrainPutOutsideObject   ( MIL_Object_ABC& object );
    //@}

    //! @name Event handlers
    //@{
    virtual void NotifyMovingInsideObject ( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyPutInsideObject( MIL_Object_ABC& object ) ;
    virtual void NotifyPutOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyFlowCollision( MIL_PopulationFlow&          population );
    virtual void NotifyConcentrationCollision( MIL_PopulationConcentration& population );
    //@}

    //! @name Accessors
    //@{
    virtual MT_Float            GetHeight         () const;
    virtual MT_Float            GetAltitude       () const;
    virtual const MT_Vector2D&  GetPosition       () const;
    virtual boost::shared_ptr<MT_Vector2D> GetSharedPosition() const;
    virtual const MT_Vector2D&  GetDirection      () const;
    virtual MIL_Agent_ABC&      GetAgent          () const;
    virtual MT_Float            GetCurrentSpeed   () const;
    virtual bool                HasDoneMagicMove  () const;
    virtual bool                HasSpeedChanged   () const; // Position or direction or height has changed 
    virtual bool                HasLocationChanged() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

private:
    //! @name Tools
    //@{
    void SetDirection   ( const MT_Vector2D& vDirection );
    void SetPosition    ( const MT_Vector2D& vPosition  );
    void SetCurrentSpeed( MT_Float rSpeed               );
    void SetHeight      ( MT_Float rHeight              );
    //@}

private:
    MIL_AgentPion& pion_;
    MT_Vector2D    vDirection_;
    boost::shared_ptr<MT_Vector2D> pvPosition_;
    MT_Float       rHeight_;
    MT_Float       rCurrentSpeed_;
    bool           bHasDoneMagicMove_;
    bool           bHasMove_;

    bool bPositionHasChanged_;
    bool bDirectionHasChanged_;
    bool bCurrentSpeedHasChanged_;
    bool bHeightHasChanged_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Location* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Location* role, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Location )

#endif // __PHY_RolePion_Location_h_
