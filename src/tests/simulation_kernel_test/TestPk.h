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

#include "PHY_MaterialCompositionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "MockMIL_Time_ABC.h"
#include <xeumeuleu/xml.hpp>

class TestPK : private boost::noncopyable
{
public:

    TestPK()
    {
        MockMIL_Time_ABC time;
        MOCK_EXPECT( time, GetTickDuration ).returns( 10u );
        MOCK_EXPECT( time, GetCurrentTick ).returns( 10u );

        xml::xistringstream xisUrbanType( "<urban><urban-block-types><material-composition-types><material-composition-type name='Bois'>"
            "<attritions><attrition destruction=\"0\" protection=\"CharLourd\" repairable-with-evacuation=\"0\" repairable-without-evacuation=\"0.05\"/></attritions></material-composition-type>"
            "<material-composition-type name='Beton'>"
            "<attritions><attrition destruction=\"0\" protection='CharLourd' repairable-with-evacuation='0' repairable-without-evacuation='0.05'/></attritions></material-composition-type>"
            "<material-composition-type name='Brique'>"
            "<attritions><attrition destruction=\"0\" protection='CharLourd' repairable-with-evacuation='0' repairable-without-evacuation='0.05'/></attritions></material-composition-type>"
            "<material-composition-type name='Metal'>"
            "<attritions><attrition destruction=\"0\" protection='CharLourd' repairable-with-evacuation='0' repairable-without-evacuation='0.05'/></attritions></material-composition-type>"
            "<material-composition-type name='Torchis'>"
            "<attritions><attrition destruction=\"0\" protection=\"CharLourd\" repairable-with-evacuation=\"0\" repairable-without-evacuation=\"0.05\"/></attritions></material-composition-type>"
            "<material-composition-type name='Prefabrique'>"
            "<attritions><attrition destruction=\"0\" protection=\"CharLourd\" repairable-with-evacuation=\"0\" repairable-without-evacuation=\"0.05\"/></attritions></material-composition-type>"
            "<material-composition-type name='Vegetation'>"
            "<attritions><attrition destruction=\"0\" protection=\"CharLourd\" repairable-with-evacuation=\"0\" repairable-without-evacuation=\"0.05\"/></attritions></material-composition-type>"
            "</material-composition-types><facade-types/><roof-shape-types/></urban-block-types></urban>" );
        PHY_MaterialCompositionType::Initialize( xisUrbanType );
        xml::xistringstream xisProtection( "<protections><protection name='protection1' type='humain'><neutralization average-time='10s' variance='1s'/></protection></protections>" );
        PHY_Protection::Initialize( xisProtection );
        xml::xistringstream xisDotationNature( "<natures><nature type='Solide' id='1'/></natures>" );
        PHY_DotationNature::Initialize( xisDotationNature );
        xml::xistringstream xisDotations( "<resources><resource name='ammo' category='munition' id='1' nature='Solide' id-nature='1' package-size='1' package-mass='1' package-volume='1'>"
            "<attritions><attrition destruction='0' protection='protection1' repairable-with-evacuation='0.1' repairable-without-evacuation='0.05'/></attritions>"
            "<urban-modifiers><urban-modifier material-type='Brique' value='0.4' /></urban-modifiers>"
            "</resource></resources>" );
        PHY_DotationType::Initialize( xisDotations );
        xml::xistringstream xisLauncher( "<launchers><launcher name='gun' indirect-fire='true'/></launchers>" );
        xml::xistringstream xisVolumes( "<volumes><volume name='Big'/></volumes>");
        PHY_Volume::Initialize( xisVolumes );
    }

    virtual ~TestPK()
    {
        PHY_MaterialCompositionType::Terminate();
        PHY_Protection::Terminate();
        PHY_Volume::Terminate();
        PHY_DotationNature::Terminate();
        PHY_DotationType::Terminate();
    }
};

#endif // __TestPK_h_
