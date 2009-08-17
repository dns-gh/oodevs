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

class PHY_ActionFly;
class MIL_Entity_ABC;

// =============================================================================
// @class  PHY_RoleAction_Flying
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Flying : public PHY_RoleAction_InterfaceFlying
{

public:
    explicit PHY_RoleAction_Flying( MIL_Entity_ABC& entity );
             PHY_RoleAction_Flying();
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
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC* pEntity_;
    MIL_Effect_Fly effectFly_;
    PHY_ActionFly* pActionFly_;
    bool           bForceLanding_;
    //@}
};



#endif // __PHY_RoleAction_Flying_h_
