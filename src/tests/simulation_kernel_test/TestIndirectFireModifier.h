// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TestIndirectFireModifier_h_
#define __TestIndirectFireModifier_h_

#include "DotationComputer_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCapacity.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Weapons/PHY_LauncherType.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "StubPHY_RoleInterface_Dotation.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include <xeumeuleu/xml.h>

class TestIndirectFireModifier : public tools::Role_ABC
    , public tools::AlgorithmModifier_ABC< firing::WeaponAvailabilityComputer_ABC >
    , public tools::AlgorithmModifier_ABC< dotation::DotationComputer_ABC >
    , private boost::noncopyable
{
public:
    typedef TestIndirectFireModifier RoleInterface;
    TestIndirectFireModifier( MIL_Time_ABC& time, MIL_EffectManager& effectManager, MIL_Agent_ABC& pion, const std::string& dotations )
        : time_         ( time )
        , effectManager_( effectManager )
        , pion_         ( pion )
    {
        xml::xistringstream xisProtection( "<protections><protection name='protection1' type='humain'><neutralization average-time='10s' variance='1s'/></protection></protections>" );
        PHY_Protection::Initialize( xisProtection );
        xml::xistringstream xisDotationNature( "<natures><nature type='Solide'/></natures>" );
        PHY_DotationNature::Initialize( xisDotationNature );
        xml::xistringstream xisDotations( dotations );
        PHY_DotationType::Initialize( xisDotations );
        xml::xistringstream xisLauncher( "<launchers><launcher name='gun' indirect-fire='true'/></launchers>" );
        PHY_LauncherType::Initialize( xisLauncher );
        xml::xistringstream xisWeapon( "<weapons><weapon-system launcher='gun' munition='ammo'><burst munition='1' duration='1s'/><reloading munition='1' duration='1s'/><indirect-fire average-speed='100' min-range='0' max-range='1000'/></weapon-system></weapons>" );
        PHY_WeaponType::Initialize( time_, xisWeapon, 1 );
        xml::xistringstream xisVolumes( "<volumes><volume name='Big'/></volumes>");
        PHY_Volume::Initialize( xisVolumes );
        xml::xistringstream xisCapacity( "<category capacity='1' logistic-threshold='50'/>" );
        xisCapacity >> xml::start( "category" );
        pCapacity_.reset( new PHY_DotationCapacity( *PHY_DotationType::FindDotationCategory( "ammo" ), xisCapacity ) );
        dotationGroupContainer_.AddCapacity( *pCapacity_ );
    }
    virtual ~TestIndirectFireModifier()
    {
        PHY_LauncherType::Terminate();
        PHY_WeaponType::Terminate();
        PHY_Protection::Terminate();
        PHY_Volume::Terminate();
        PHY_DotationNature::Terminate();
        PHY_DotationType::Terminate();
    }
    virtual void Execute( firing::WeaponAvailabilityComputer_ABC& algo ) const
    {
        xml::xistringstream xisComposanteTypePion( "<bla name='composanteTypePion' id='1' protection='protection1' weight='1' size='Big'><speeds max='8'></speeds><weapon-systems/><sensors/><transports/><objects/><consumptions/></bla>" );
        xisComposanteTypePion.start( "bla" );
        PHY_ComposanteTypePion type( time_, "composanteTypePion", xisComposanteTypePion );
        role_.reset( new PHY_RolePion_Composantes( pion_, false ) );
        firer_.reset( new PHY_ComposantePion( time_, type, *role_, 1, true, true, true ) );
        weapon_.reset( new PHY_Weapon( time_, *PHY_WeaponType::FindWeaponType( "gun", "ammo" ), true ) );
        algo.ApplyOnWeapon( *firer_, *weapon_ );
    }
    virtual void Execute( dotation::DotationComputer_ABC& algo ) const
    {
        algo.SetDotationContainer( dotationGroupContainer_ );
    }
private:
    MIL_Time_ABC& time_;
    MIL_EffectManager& effectManager_;
    MIL_Agent_ABC& pion_;
    mutable PHY_DotationGroupContainer dotationGroupContainer_;
    std::auto_ptr< PHY_DotationCapacity > pCapacity_;
    mutable std::auto_ptr< PHY_RolePion_Composantes > role_;
    mutable std::auto_ptr< PHY_ComposantePion > firer_;
    mutable std::auto_ptr< PHY_Weapon > weapon_;
};

#endif // __TestIndirectFireModifier_h_
