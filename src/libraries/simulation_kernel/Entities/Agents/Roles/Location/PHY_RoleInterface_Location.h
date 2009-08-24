// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:59 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Location.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Location_h_
#define __PHY_RoleInterface_Location_h_

#include "MT_Tools/Role_ABC.h"
#include "simulation_terrain/TER_Agent_ABC.h"

class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_Object_ABC;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class NET_ASN_MsgUnitAttributes;
class HLA_UpdateFunctor;

// =============================================================================
// @class  PHY_RoleInterface_Location
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Location : public tools::Role_ABC
                                 , public TER_Agent_ABC
                                 , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Location RoleInterface;

public:
             PHY_RoleInterface_Location();
    virtual ~PHY_RoleInterface_Location();

    //! @name Operations
    //@{
    virtual void NotifyPopulationCollision( MIL_PopulationFlow&          population ) = 0;
    virtual void NotifyPopulationCollision( MIL_PopulationConcentration& population ) = 0;
    virtual void NotifyObjectCollision    ( MIL_Object_ABC& object ) = 0;

    virtual void NotifyMovingInsideObject ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object ) = 0;
    virtual void NotifyPutInsideObject    ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyPutOutsideObject   ( MIL_Object_ABC& object ) = 0;

    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean () = 0;

    virtual void Hide     () = 0;
    virtual void Show     ( const MT_Vector2D& vNewPosition ) = 0;
    virtual void MagicMove( const MT_Vector2D& vNewPosition ) = 0;
    virtual void Move     ( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, MT_Float rNewSpeed ) = 0;
    virtual void Follow   ( const MIL_Agent_ABC& agent ) = 0;
    virtual void Fly      ( MT_Float rHeight ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual MT_Float            GetHeight           () const = 0;
    virtual MT_Float            GetAltitude         () const = 0;
    virtual const MT_Vector2D&  GetPosition         () const = 0;
    virtual const MT_Vector2D&  GetDirection        () const = 0;
    virtual MIL_Agent_ABC&      GetAgent            () const = 0;
    virtual MT_Float            GetCurrentSpeed     () const = 0;
    virtual bool                HasDoneMagicMove    () const = 0;
    virtual bool                HasSpeedChanged     () const = 0; // Position or direction or height has changed 
    virtual bool                HasLocationChanged  () const = 0;
    //@}

    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asnMsg ) const = 0;

    virtual void Serialize( HLA_UpdateFunctor& functor ) const = 0;


};

#endif // __PHY_RoleInterface_Location_h_
