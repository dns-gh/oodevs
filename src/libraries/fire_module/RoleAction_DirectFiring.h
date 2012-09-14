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
    //int  FirePopulation         ( unsigned int nTargetKnowledgeID, const PHY_AmmoDotationClass* dotationClass );
    //void FirePopulationSuspended( unsigned int nTargetKnowledgeID );

    int  FirePion( const wrapper::View& model, const wrapper::View& entity, const wrapper::View& target, DirectFireData::E_FiringMode nFiringMode, double rPercentageComposantesToUse, DirectFireData::E_ComposanteFiringType nComposanteFiringType, bool bFireOnlyOnMajorComposantes, bool mustReport, int ammoDotationClass ) const;
    void FirePionSuspended( const wrapper::View& entity, const wrapper::View& target, bool mustReport ) const;
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
        eEnemyDestroyed,    // -> le tir est termin� parceque le pion adverse est d�truit
        eNoCapacity,        // -> le tir est termin� car il ne reste aucune composante ayant la capacit� de tirer
        eNoAmmo,            // -> le tir est termin� parque qu'il reste des composante capables de tirer mais qu'il ne reste plus de munition ad�quates
        eRunning,           // -> le tir est en cours d'ex�cution
        eFinished,          // Stop tir
        eTemporarilyBlocked // -> Le tir ne peut pas etre effectue tout de suite pour cause d'encombrement en zone urbaine
    };

    typedef std::vector< wrapper::View >         T_ComposanteVector;
    typedef T_ComposanteVector::const_iterator CIT_ComposanteVector;
    //@}

private:
    //! @name Helpers
    //@{
    //MIL_Population* GetPopulationTarget( unsigned int nTargetKnowledgeID );
    void FirePion( DirectFireData& firerWeapons, const wrapper::View& entity, const wrapper::View& target, const T_ComposanteVector& compTargets ) const;
    T_ComposanteVector GetComposantesAbleToBeFired( const wrapper::View& components, bool bFireOnlyOnMajorComposantes, unsigned int nNbrWeaponsUsable ) const;
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
