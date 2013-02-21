// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_RoleAction_DirectFiring_h
#define fire_module_RoleAction_DirectFiring_h

#include "DirectFireData.h"
#include <boost/noncopyable.hpp>
#include <vector>

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
/** @class  RoleAction_DirectFiring
    @brief  Role action direct firing
*/
// Created: MCO 2012-03-19
// =============================================================================
class RoleAction_DirectFiring : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RoleAction_DirectFiring( ModuleFacade& module );
    //@}

    //! @name Operations
    //@{
    int  FirePopulation( const wrapper::View& model, const wrapper::View& entity, const wrapper::View& target, const wrapper::View& parameters ) const;
    void FirePopulationSuspended( const wrapper::View& entity, const wrapper::View& target ) const;

    int  FirePion( const wrapper::View& model, const wrapper::View& entity, const wrapper::View& target, const wrapper::View& parameters, bool mustReport ) const;
    void FirePionSuspended( const wrapper::View& model, const wrapper::View& entity, const wrapper::View& target, bool mustReport ) const;
    //int  IlluminatePion         ( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy );
    //void IlluminatePionSuspended( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy );

    //void FireZone               ( const MIL_Object_ABC& zone, FireResults_Default*& pFireResult );
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
        eImpossible,
        eEnemyDestroyed,    // -> le tir est terminé parceque le pion adverse est détruit
        eNoCapacity,        // -> le tir est terminé car il ne reste aucune composante ayant la capacité de tirer
        eNoAmmo,            // -> le tir est terminé parque qu'il reste des composante capables de tirer mais qu'il ne reste plus de munition adéquates
        eRunning,           // -> le tir est en cours d'exécution
        eFinished,          // Stop tir
        eTemporarilyBlocked // -> Le tir ne peut pas etre effectue tout de suite pour cause d'encombrement en zone urbaine
    };

    typedef std::vector< wrapper::View > T_Components;
    //@}

private:
    //! @name Helpers
    //@{
    void FirePion( DirectFireData& firerWeapons, const wrapper::View& entity, const wrapper::View& target, const T_Components& compTargets ) const;
    T_Components GetComposantesAbleToBeFired( const wrapper::View& components, const wrapper::View& parameters, std::size_t nNbrWeaponsUsable ) const;
    //@}

private:
    //! @name Member data
    //@{
    ModuleFacade& module_;
    //@}
};

}
}

#endif // fire_module_RoleAction_DirectFiring_h
