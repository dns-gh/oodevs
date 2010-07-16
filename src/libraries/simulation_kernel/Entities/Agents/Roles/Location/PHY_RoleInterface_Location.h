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
#include <boost/shared_ptr.hpp>

namespace client
{
    class UnitAttributes;
}

class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
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
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Location RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Location() {}
    virtual ~PHY_RoleInterface_Location() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyTerrainObjectCollision( MIL_Object_ABC& object ) = 0;

    virtual void NotifyTerrainPutInsideObject( MIL_Object_ABC& object ) = 0;
    virtual void NotifyTerrainPutOutsideObject( MIL_Object_ABC& object ) = 0;

    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;

    virtual void MagicMove( const MT_Vector2D& vNewPosition ) = 0;
    virtual void Move( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, MT_Float rNewSpeed ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual MT_Float GetHeight() const = 0;
    virtual MT_Float GetAltitude() const = 0;
    virtual const MT_Vector2D& GetPosition() const = 0;
    virtual boost::shared_ptr<MT_Vector2D> GetSharedPosition() const = 0;
    virtual const MT_Vector2D& GetDirection() const = 0;
    virtual MIL_Agent_ABC& GetAgent() const = 0;
    virtual MT_Float GetCurrentSpeed() const = 0;
    virtual bool HasDoneMagicMove() const = 0;
    virtual bool HasSpeedChanged() const = 0; // Position or direction or height has changed
    virtual bool HasLocationChanged() const = 0;
    //@}

//    virtual void SendChangedState( client::UnitAttributes& asnMsg ) const = 0;
//    virtual void SendFullState( client::UnitAttributes& asnMsg ) const = 0;

    virtual void Serialize( HLA_UpdateFunctor& functor ) const = 0;
private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const uint )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}

};

#endif // __PHY_RoleInterface_Location_h_
