// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TestPK_h_
#define __TestPK_h_

#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "UrbanType.h"
#include <xeumeuleu/xml.hpp>

class TestPK : private boost::noncopyable
{
public:

    TestPK()
    {
        xml::xistringstream xisUrbanType( "<urban><urban-block-types><material-composition-types><material-composition-type name='Bois'/>"
            "<material-composition-type name='Beton'/><material-composition-type name='Brique'/><material-composition-type name='Metal'/>"
            "<material-composition-type name='Torchis'/><material-composition-type name='Prefabrique'/><material-composition-type name='Vegetation'/>"
            "</material-composition-types><facade-types/><roof-shape-types/></urban-block-types></urban>" );
        UrbanType::Initialize( xisUrbanType );
        xml::xistringstream xisProtection( "<protections><protection name='protection1' type='humain'><neutralization average-time='10s' variance='1s'/></protection></protections>" );
        PHY_Protection::Initialize( xisProtection );
        xml::xistringstream xisDotationNature( "<natures><nature type='Solide'/></natures>" );
        PHY_DotationNature::Initialize( xisDotationNature );
        xml::xistringstream xisDotations( "<dotations><dotation category='ammo' name='munition' id='1' nature='Solide' package-size='1' package-mass='1' package-volume='1'>"
            "<attritions><attrition destruction='0' protection='protection1' repairable-with-evacuation='0.1' repairable-without-evacuation='0.05'/></attritions>"
            "<urbanModifiers><urbanModifier material-type='Brique' value='0.4' /></urbanModifiers>"
            "</dotation></dotations>" );
        PHY_DotationType::Initialize( xisDotations );
        xml::xistringstream xisLauncher( "<launchers><launcher name='gun' indirect-fire='true'/></launchers>" );
        xml::xistringstream xisVolumes( "<volumes><volume name='Big'/></volumes>");
        PHY_Volume::Initialize( xisVolumes );
    }

    virtual ~TestPK()
    {
        UrbanType::Terminate();
        PHY_Protection::Terminate();
        PHY_Volume::Terminate();
        PHY_DotationNature::Terminate();
        PHY_DotationType::Terminate();
    }
};

#endif // __TestPK_h_
