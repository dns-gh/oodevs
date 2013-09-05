// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h $
// $Author: Jvt $
// $Modtime: 30/03/05 15:39 $
// $Revision: 3 $
// $Workfile: PHY_RoleAction_IndirectFiring.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_IndirectFiring_h_
#define __PHY_RoleAction_IndirectFiring_h_

#include "MIL.h"
#include "MT_Tools/Role_ABC.h"

class PHY_IndirectFireDotationClass;
class PHY_DotationCategory;
class MIL_Agent_ABC;
class MIL_Effect_IndirectFire;
class MT_Vector2D;

namespace firing
{
// =============================================================================
// @class  PHY_RoleAction_IndirectFiring
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_IndirectFiring : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_IndirectFiring RoleInterface;
    //@}

public:
             PHY_RoleAction_IndirectFiring();
    explicit PHY_RoleAction_IndirectFiring( MIL_Agent_ABC& pion );
    virtual ~PHY_RoleAction_IndirectFiring();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean ();
    //@}

    //! @name Operations
    //@{
    int  Fire( MIL_Effect_IndirectFire* pEffect );
    void FireSuspended();

    const PHY_DotationCategory* GetMunitionForIndirectFire( const PHY_IndirectFireDotationClass& indirectWeaponCategory, const MT_Vector2D* vTargetPosition );
    double ThrowSmoke( const MT_Vector2D& vTargetPosition );
    //@}

    //! @name Return codes
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;
    //@}

    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,    // -> le tir est en cours d'ex�cution
        eNoCapacity, // -> le tir n'a pas lieu car aucune composante n'a la capacit� de tirer
        eFinished,    // -> le tir est termin� par un DEC_StopTir ou parce que les obus sont arriv�s sur la cible
        eImpossible,
        eNoAmmo
    };
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC* owner_;
    //@}
};

} //namespace firing

BOOST_CLASS_EXPORT_KEY( firing::PHY_RoleAction_IndirectFiring )

#endif // __PHY_RoleAction_IndirectFiring_h_
