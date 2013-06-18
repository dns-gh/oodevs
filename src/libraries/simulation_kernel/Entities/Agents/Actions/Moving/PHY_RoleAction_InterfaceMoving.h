// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h $
// $Author: Age $
// $Modtime: 13/04/05 18:25 $
// $Revision: 14 $
// $Workfile: PHY_RoleAction_InterfaceMoving.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_InterfaceMoving_h_
#define __PHY_RoleAction_InterfaceMoving_h_

#include "MT_Tools/Role_ABC.h"

class TerrainData;
class MIL_Object_ABC;

namespace moving
{
// =============================================================================
// @class  PHY_RoleAction_InterfaceMoving
// Created: MCO 2012-03-05
// =============================================================================
class PHY_RoleAction_InterfaceMoving : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_InterfaceMoving RoleInterface;
    //@}

public:
    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean() = 0;

    virtual double GetMaxSpeedWithReinforcement() const = 0;
    virtual double GetMaxSpeedWithReinforcement( const TerrainData& environment ) const = 0;
    virtual double GetTheoricMaxSpeed( bool loaded ) const = 0;
    virtual double GetTheoricMaxSpeedWithReinforcement() const = 0;
    virtual double GetSpeedWithReinforcement( const TerrainData& environment ) const = 0;
    virtual double GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const = 0;
    virtual double GetTheoricSpeedWithReinforcement( const TerrainData& environment ) const = 0;
    virtual double GetMaxSpeedModificator() const = 0;
    virtual double GetMaxSlope() const = 0;

    virtual void SetSpeedModificator( double rFactor ) = 0;
    virtual void SetMaxSpeedModificator( double rFactor ) = 0;
    virtual void ApplyTrafficModifier() = 0;
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

} // namespace moving

#endif // __PHY_RoleAction_InterfaceMoving_h_
