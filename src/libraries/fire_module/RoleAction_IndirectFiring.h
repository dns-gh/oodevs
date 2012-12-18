// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_RoleAction_IndirectFiring_h
#define fire_module_RoleAction_IndirectFiring_h

#include <boost/noncopyable.hpp>
#include <string>

class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace fire
{
    class ModuleFacade;

// =============================================================================
/** @class  RoleAction_IndirectFiring
    @brief  Role action INdirect firing
*/
// Created: MCO 2012-06-25
// =============================================================================
class RoleAction_IndirectFiring
{
public:
    ////! @name Operations
    ////@{
    //void Update( bool bIsDead );
    //void Clean ();
    ////@}

    ////! @name Operations
    ////@{
    //int  Fire( MIL_Effect_IndirectFire* pEffect );
    //void FireSuspended();

    int GetAmmunitionForIndirectFire( ModuleFacade& module, const wrapper::View& model, const wrapper::View& firer, const std::string& type, const MT_Vector2D* target );
    //int ThrowSmoke( const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo );
    ////@}

    ////! @name Return codes
    ////@{
    //int GetInitialReturnCode() const;
    //int GetFinalReturnCode  () const;
    ////@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,    // -> le tir est en cours d'exécution
        eNoCapacity, // -> le tir n'a pas lieu car aucune composante n'a la capacité de tirer
        eFinished,    // -> le tir est terminé par un DEC_StopTir ou parce que les obus sont arrivés sur la cible
        eImpossible,
        eNoAmmo
    };
    //@}
};

}
}

#endif // fire_module_RoleAction_IndirectFiring_h
