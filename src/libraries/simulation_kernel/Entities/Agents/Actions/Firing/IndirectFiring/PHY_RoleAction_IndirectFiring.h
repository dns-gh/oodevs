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

#include "MT_Tools/Role_ABC.h"

class PHY_IndirectFireDotationClass;
class PHY_DotationCategory;
class MIL_AgentPion;
class MIL_Effect_IndirectFire;

namespace firing
{
// =============================================================================
// @class  PHY_RoleAction_IndirectFiring
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_IndirectFiring : public tools::Role_ABC
                                    , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_IndirectFiring RoleInterface;
    //@}

public:
    explicit PHY_RoleAction_IndirectFiring( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_IndirectFiring();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean ();
    //@}

    //! @name Operations
    //@{
    int  Fire         ( MIL_Effect_IndirectFire* pEffect );
    void FireSuspended();

    const PHY_DotationCategory* GetMunitionForIndirectFire( const PHY_IndirectFireDotationClass& indirectWeaponCategory, const MT_Vector2D& vTargetPosition );
          int                   ThrowSmoke                ( const MT_Vector2D& vTargetPosition, uint nNbrAmmo );
    //@}

    //! @name Return codes
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;
    //@}

private:
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

    //! @name Tools
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RoleAction_IndirectFiring* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RoleAction_IndirectFiring* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion& pion_;
    //@}
};

} //namespace firing

#endif // __PHY_RoleAction_IndirectFiring_h_
