// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h $
// $Author: Jvt $
// $Modtime: 30/03/05 15:43 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_InterfaceFlying.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_InterfaceFlying_h_
#define __PHY_RoleAction_InterfaceFlying_h_

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleAction_InterfaceFlying
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_InterfaceFlying : public MT_Role_ABC
                                     , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_InterfaceFlying RoleInterface;
    //@}

public:
             PHY_RoleAction_InterfaceFlying();
    explicit PHY_RoleAction_InterfaceFlying( MT_RoleContainer& role );
    virtual ~PHY_RoleAction_InterfaceFlying();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();
    virtual bool CanMove() const;
    //@}

    //! @name Action interface
    //@{
    virtual void Fly();
    virtual void SetFlyingHeight( MT_Float rHeight );
    virtual bool IsFlying() const;
    //@}

    //! @name Effect interface
    //@{
    virtual void Apply( MT_Float rHeight );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleAction_InterfaceFlying::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleAction_InterfaceFlying_h_
