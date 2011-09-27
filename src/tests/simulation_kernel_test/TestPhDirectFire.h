// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TestPhDirectFire_h_
#define __TestPhDirectFire_h_

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/Weapons/PHY_LauncherType.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"

#include <xeumeuleu/xml.hpp>

class TestPhDirectFire : public tools::Role_ABC
                       , private boost::noncopyable
{
public:
    typedef TestPhDirectFire RoleInterface;
    TestPhDirectFire( MIL_Time_ABC& time )
        : time_         ( time )
    {
        xml::xistringstream xisProtection( "<protections><protection name='protection1' type='humain'><neutralization average-time='10s' variance='1s'/></protection></protections>" );
        PHY_Protection::Initialize( xisProtection );

        PHY_Posture::Initialize();

        xml::xistringstream xisVolumes( "<volumes><volume name='Heavy'/><volume name='Medium'/><volume name='Small'/><volume name='Personal'/></volumes>");
        PHY_Volume::Initialize( xisVolumes );

        xml::xistringstream xisDotationNature( "<natures><nature type='Solide' id='1'/></natures>" );
        PHY_DotationNature::Initialize( xisDotationNature );
        xml::xistringstream xisLogisticSupplyClasses( "<logistic-supply-classes><logistic-supply-class id='12' type='whatever'/></logistic-supply-classes>" );
        PHY_DotationLogisticType::Initialize( xisLogisticSupplyClasses );
        xml::xistringstream xisDotations( "<resources><resource name='ammo' category='munition' id='1' logistic-supply-class='whatever' nature='Solide' id-nature='1' package-size='1' package-mass='1' package-volume='1'><attritions>"
            "<attrition destruction='0' protection='protection1' repairable-with-evacuation='0.1' repairable-without-evacuation='0.05'/></attritions></resource></resources>" );
        PHY_DotationType::Initialize( xisDotations );

        xml::xistringstream xisLauncher( "<launchers><launcher name='gun'><ph-modifiers posture='Mouvement'>"
            "<ph-modifier target-posture='Mouvement' value='0.5'/><ph-modifier target-posture='MouvementDiscret' value='0.5'/><ph-modifier target-posture='Arret' value='0.5'/>"
            "<ph-modifier target-posture='PosteReflexe' value='0.5'/><ph-modifier target-posture='Poste' value='0.5'/><ph-modifier target-posture='PosteAmenage' value='0.5'/>"
            "<ph-modifier target-posture='PostePrepareGenie' value='0.5'/></ph-modifiers></launcher></launchers>" );

        PHY_LauncherType::Initialize( xisLauncher );
        xml::xistringstream xisWeapon( "<weapons><weapon-system launcher='gun' munition='ammo'><burst duration='5s' munition='5'/>"
            "<reloading duration='20s' munition='30'/><direct-fire><hit-probabilities target='Heavy'><hit-probability distance='0' percentage='1'/>"
            "<hit-probability distance='1000' percentage='0.9'/><hit-probability distance='1500' percentage='0.8'/></hit-probabilities></direct-fire>"
            "</weapon-system></weapons>" );
        PHY_WeaponType::Initialize( time_, xisWeapon );
    }

    virtual ~TestPhDirectFire()
    {
        PHY_LauncherType::Terminate();
        PHY_WeaponType::Terminate();
        PHY_Protection::Terminate();
        PHY_Volume::Terminate();
        PHY_DotationNature::Terminate();
        PHY_DotationType::Terminate();
    }
    virtual void Execute() const
    {
        //TO DO
    }

private:
    MIL_Time_ABC& time_;
};

#endif // __TestIndirectFireModifier_h_
