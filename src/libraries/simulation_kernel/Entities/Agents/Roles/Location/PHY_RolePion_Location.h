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

class MIL_AgentPion;

namespace location
{
    class LocationComputerFactory_ABC;
}

// =============================================================================
// @class  PHY_RolePion_Location
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Location : public PHY_RoleInterface_Location
{

public:
    explicit PHY_RolePion_Location( MIL_AgentPion& pion, const location::LocationComputerFactory_ABC& locationComputerFactory );
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

    virtual void NotifyPopulationCollision( MIL_PopulationFlow&          population );
    virtual void NotifyPopulationCollision( MIL_PopulationConcentration& population );
    virtual void NotifyObjectCollision    ( MIL_Object_ABC& object );

    virtual void NotifyMovingInsideObject ( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyPutInsideObject    ( MIL_Object_ABC& object );
    virtual void NotifyPutOutsideObject   ( MIL_Object_ABC& object );
    //@}

    //! @name Accessors
    //@{
    virtual MT_Float            GetHeight         () const;
    virtual MT_Float            GetAltitude       () const;
    virtual const MT_Vector2D&  GetPosition       () const;
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
    MT_Vector2D    vPosition_;
    MT_Float       rHeight_;
    MT_Float       rCurrentSpeed_;
    bool           bHasDoneMagicMove_;
    bool           bHasMove_;

    const location::LocationComputerFactory_ABC& locationComputerFactory_;

    bool bPositionHasChanged_;
    bool bDirectionHasChanged_;
    bool bCurrentSpeedHasChanged_;
    bool bHeightHasChanged_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Location* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Location* role, const unsigned int /*version*/ );

};

#endif // __PHY_RolePion_Location_h_
