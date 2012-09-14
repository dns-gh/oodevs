// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FireHooks.h"
#include "Hook.h"
#include "MIL_AgentServer.h"
#include "DotationComputer_ABC.h"
#include "AlgorithmsFactories.h"
#include "DotationComputerFactory_ABC.h"
#include "ComposantesAbleToBeFiredComputer_ABC.h"
#include "ComposantesAbleToBeFiredComputerFactory_ABC.h"
#include "WeaponReloadingComputer_ABC.h"
#include "WeaponReloadingComputerFactory_ABC.h"
#include "DefaultDotationComputer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Weapons/PHY_LauncherType.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
#include <core/Facade.h>
#include <core/UserData.h>
#include <core/Convert.h>
#include <module_api/Hook.h>
#include <module_api/Model.h>

using namespace sword;

#define GET_ROLE( node, role ) (*core::Convert( node ))[ "pion" ].GetUserData< MIL_AgentPion >().GetRole< role >()
#define GET_PION( node ) (*core::Convert( node ))[ "pion" ].GetUserData< MIL_AgentPion >()

namespace
{
    const PHY_DotationCategory* GetDotationCategory( const char* dotation )
    {
        if( ! dotation )
            return 0;
        return PHY_DotationType::FindDotationCategory( dotation );
    }
    DEFINE_HOOK( GetFireRandomInteger, size_t, ( size_t min, size_t max ) )
    {
        return MIL_Random::rand32_io( min, max, MIL_Random::eFire );
    }
    DEFINE_HOOK( IsTemporarilyBlocked, bool, ( const SWORD_Model* entity, std::size_t nUrbanCoefficient ) )
    {
        if( ! GET_ROLE( entity, PHY_RoleInterface_UrbanLocation ).IsInCity() )
            return false;
        return GET_HOOK( GetFireRandomInteger )( 0u, 100u ) >= nUrbanCoefficient; // $$$$ MCO 2012-08-28: move nUrbanCoefficient out of fire module and into model
    }
    DEFINE_HOOK( GetFireRandomNumber, double, ( double min, double max ) )
    {
        return MIL_Random::rand_io( min, max, MIL_Random::eFire );
    }
    DEFINE_HOOK( HasDotation, bool, ( const SWORD_Model* entity, const char* dotation ) )
    {
        const PHY_DotationCategory* category = GetDotationCategory( dotation );
        if( ! category )
        {
            MT_LOG_ERROR_MSG( "Unknown dotation category in HasDotation hook implementation : " << dotation );
            return false;
        }
        MIL_AgentPion& pion = GET_PION( entity );
        std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( pion.GetAlgorithms().dotationComputerFactory_->Create() );
        pion.Execute( *dotationComputer );
        return dotationComputer->HasDotation( *category );
    }
    DEFINE_HOOK( GetDotationValue, double, ( const SWORD_Model* entity, const char* dotation ) )
    {
        const PHY_DotationCategory* category = GetDotationCategory( dotation );
        if( ! category )
        {
            MT_LOG_ERROR_MSG( "Unknown dotation category in HasDotation hook implementation : " << dotation );
            return false;
        }
        MIL_AgentPion& pion = GET_PION( entity );
        std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( pion.GetAlgorithms().dotationComputerFactory_->Create() );
        pion.Execute( *dotationComputer );
        return dotationComputer->GetDotationValue( *category );
    }
    DEFINE_HOOK( CanFire, bool, ( const SWORD_Model* component, const char* dotation, int firingType, int ammoDotationClass ) )
    {
        const PHY_ComposantePion& compFirer = (*core::Convert( component ))[ "component" ].GetUserData< PHY_ComposantePion >();
        if( !compFirer.CanFire() )
            return false;
        if( nComposanteFiringType == 1 && !compFirer.CanBeLoaded() ) // $$$$ MCO 2012-04-30: eFireUsingOnlyComposantesLoadable
            return false;
        if( nComposanteFiringType == 2 && !compFirer.CanTransportHumans() ) // $$$$ MCO 2012-04-30: eFireUsingOnlyComposantesCarrier
            return false;
        const PHY_DotationCategory* category = GetDotationCategory( dotation );
        if( ! category )
        {
            MT_LOG_ERROR_MSG( "Unknown dotation category in CanFire hook implementation : " << dotation );
            return false;
        }
        const PHY_AmmoDotationClass* pAmmoDotationClass = 0;
        if( ammoDotationClass != -1 ) // $$$$ LDC FIXME Varargs hidden here...
           pAmmoDotationClass = PHY_AmmoDotationClass::Find( ammoDotationClass );
        if( pAmmoDotationClass && ( ! category->GetAmmoDotationClass() || *category->GetAmmoDotationClass() != *pAmmoDotationClass ) )
            return false;
        return true;
    }
    DEFINE_HOOK( CanComponentFire, bool, ( const SWORD_Model* component ) )
    {
        return (*core::Convert( component ))[ "component" ].GetUserData< PHY_ComposantePion >().CanFire();
    }
    DEFINE_HOOK( CanComponentBeFiredAt, bool, ( const SWORD_Model* component ) )
    {
        return (*core::Convert( component ))[ "component" ].GetUserData< PHY_ComposantePion >().CanBeFired();
    }
    DEFINE_HOOK( GetWeaponReloadingDuration, double, ( const SWORD_Model* firer, double rDuration ) )
    {
        MIL_AgentPion& pion = GET_PION( firer );
        std::auto_ptr< firing::WeaponReloadingComputer_ABC > computer( pion.GetAlgorithms().weaponReloadingComputerFactory_->Create( rDuration ) );
        pion.Execute( *computer );
        return computer->GetDuration();
    }
    DEFINE_HOOK( ReserveAmmunition, unsigned int, ( const SWORD_Model* firer, const char* dotation, double nNbrAmmoToFire ) )
    {
        const PHY_DotationCategory* category = GetDotationCategory( dotation );
        if( ! category )
        {
            MT_LOG_ERROR_MSG( "Unknown dotation category in ReserveAmmunition hook implementation : " << dotation );
            return 0;
        }
        using namespace dotation;
        return (unsigned int)GET_ROLE( firer, PHY_RoleInterface_Dotations ).AddFireReservation( *category, nNbrAmmoToFire );
    }
    DEFINE_HOOK( GetVolumeId, size_t, ( const char* type ) )
    {
        const PHY_Volume::T_VolumeMap& volumes = PHY_Volume::GetVolumes();
        PHY_Volume::CIT_VolumeMap it = volumes.find( type );
        const PHY_Volume& volume = *it->second;
        return volume.GetID();
    }
    DEFINE_HOOK( GetDistance, double, ( const SWORD_Model* firer, const SWORD_Model* target ) )
    {
        const MIL_AgentPion& pion = GET_PION( firer );
        const MIL_Agent_ABC& knowledge = (*core::Convert( target ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >()->GetAgentKnown();
        return pion.Distance( knowledge );
    }
    DEFINE_HOOK( ModifyPh, double, ( const SWORD_Model* firer, const SWORD_Model* target, const char* dotation, double rPh ) )
    {
        const PHY_DotationCategory* category = GetDotationCategory( dotation );
        if( ! category )
        {
            MT_LOG_ERROR_MSG( "Unknown dotation category in ModifyPh hook implementation : " << dotation );
            return false;
        }
        const MIL_Agent_ABC& enemy = (*core::Convert( target ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >()->GetAgentKnown();
        const double protection = enemy.GetRole< PHY_RoleInterface_ActiveProtection >().GetPHModifier( *category );
        return GET_ROLE( firer, PHY_RoleInterface_HumanFactors ).ModifyPH( rPh * protection );
    }
    DEFINE_HOOK( GetPhModificator, double, ( const SWORD_Model* firer, const SWORD_Model* target, const char* launcher ) )
    {
        const PHY_LauncherType* type = PHY_LauncherType::FindLauncherType( launcher );
        if( ! type )
        {
            MT_LOG_ERROR_MSG( "Unknown launcher type in GetPhModificator hook implementation : " << launcher );
            return 0;
        }
        const MIL_Agent_ABC& enemy = (*core::Convert( target ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >()->GetAgentKnown();
        const PHY_RoleInterface_Posture& firerPosture  = GET_ROLE( firer, PHY_RoleInterface_Posture );
        const PHY_RoleInterface_Posture& targetPosture = enemy.GetRole< PHY_RoleInterface_Posture >();
        return type->GetPHModificator( firerPosture, targetPosture ) * firerPosture.GetElongationFactor();
    }
    DEFINE_HOOK( GetPhModificator2, double, ( const char* launcher ) )
    {
        const PHY_LauncherType* type = PHY_LauncherType::FindLauncherType( launcher );
        if( ! type )
        {
            MT_LOG_ERROR_MSG( "Unknown launcher type in GetPhModificator hook implementation : " << launcher );
            return 0;
        }
        return type->GetPHModificator();
    }
    DEFINE_HOOK( ModifyDangerosity, double, ( const SWORD_Model* compTarget, const char* dotation ) )
    {
        const PHY_DotationCategory* category = GetDotationCategory( dotation );
        if( ! category )
        {
            MT_LOG_ERROR_MSG( "Unknown dotation category in ModifyDangerosity hook implementation : " << dotation );
            return 0;
        }
        return category->GetAttritionScore( (*core::Convert( compTarget ))[ "component" ].GetUserData< PHY_ComposantePion >().GetType().GetProtection() );
    }
    DEFINE_HOOK( EvaluateDangerosity, double, ( const SWORD_Model* agent, const SWORD_Model* target ) )
    {
        const MIL_AgentPion& pion = GET_PION( target );
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = (*core::Convert( agent ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >();
        return knowledge->GetDangerosity( pion, false ) * knowledge->GetOperationalState(); // $$$$ MCO 2012-05-16: use fire module GetDangerosity
    }
    DEFINE_HOOK( EvaluateDangerosity2, double, ( const SWORD_Model* agent, const SWORD_Model* target ) )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge1 = (*core::Convert( agent ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >();
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge2 = (*core::Convert( target ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >();
        return knowledge1->GetDangerosity( *knowledge2, true ) * knowledge2->GetOperationalState(); // $$$$ MCO 2012-05-16: use fire module GetDangerosity
    }
}

// -----------------------------------------------------------------------------
// Name: FireHooks::Initialize
// Created: MCO 2012-04-30
// -----------------------------------------------------------------------------
void FireHooks::Initialize( core::Facade& facade )
{
    REGISTER_HOOK( IsTemporarilyBlocked, facade );
    REGISTER_HOOK( GetFireRandomInteger, facade );
    REGISTER_HOOK( GetFireRandomNumber, facade );
    REGISTER_HOOK( HasDotation, facade );
    REGISTER_HOOK( GetDotationValue, facade );
    REGISTER_HOOK( CanFire, facade );
    REGISTER_HOOK( CanComponentFire, facade );
    REGISTER_HOOK( CanComponentBeFiredAt, facade );
    REGISTER_HOOK( GetWeaponReloadingDuration, facade );
    REGISTER_HOOK( ReserveAmmunition, facade );
    REGISTER_HOOK( GetVolumeId, facade );
    REGISTER_HOOK( GetDistance, facade );
    REGISTER_HOOK( ModifyPh, facade );
    REGISTER_HOOK( GetPhModificator, facade );
    REGISTER_HOOK( GetPhModificator2, facade );
    REGISTER_HOOK( ModifyDangerosity, facade );
    REGISTER_HOOK( EvaluateDangerosity, facade );
    REGISTER_HOOK( EvaluateDangerosity2, facade );
}
