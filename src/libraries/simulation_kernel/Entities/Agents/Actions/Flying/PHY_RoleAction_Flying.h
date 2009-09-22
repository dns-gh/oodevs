// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Flying/PHY_RoleAction_Flying.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 7 $
// $Workfile: PHY_RoleAction_Flying.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Flying_h_
#define __PHY_RoleAction_Flying_h_

#include "PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Effects/MIL_Effect_Fly.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

class PHY_ActionFly;
class MIL_Entity_ABC;

namespace location
{
    class LocationComputer_ABC;
}
namespace posture
{
    class PostureComputer_ABC;
}
namespace moving
{
    class MoveComputerFactory_ABC;
}
// =============================================================================
// @class  PHY_RoleAction_Flying
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Flying : public PHY_RoleAction_InterfaceFlying
                            , public tools::AlgorithmModifier_ABC< location::LocationComputer_ABC >
                            , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
{

public:
    explicit PHY_RoleAction_Flying( MIL_Entity_ABC& entity, const moving::MoveComputerFactory_ABC& moveComputerFactory );
    virtual ~PHY_RoleAction_Flying();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Update ( bool bIsDead );
    virtual void Clean  ();
    virtual bool CanMove() const;
    //@}

    //! @name Action interface
    //@{
    virtual void Fly            ();
    virtual void SetFlyingHeight( MT_Float rHeight );
    virtual bool IsFlying       () const;
    //@}

    //! @name Effect interface
    //@{
    virtual void Apply( MT_Float rHeight );
    //@}

private:
    //! @name Tools
    //@{
    bool TakeOff();
    bool Land   ();
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RoleAction_Flying* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RoleAction_Flying* role, const unsigned int /*version*/ );
    virtual void Execute( location::LocationComputer_ABC& algorithm ) const;
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC& entity_;
    MIL_Effect_Fly effectFly_;
    PHY_ActionFly* pActionFly_;
    double         rHeight_;

    const moving::MoveComputerFactory_ABC& moveComputerFactory_;
    //@}
};



#endif // __PHY_RoleAction_Flying_h_
