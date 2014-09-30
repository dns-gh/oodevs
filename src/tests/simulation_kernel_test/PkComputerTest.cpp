// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "MockAgent.h"
#include "MockMIL_Time_ABC.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Urban/MIL_UrbanObject.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "simulation_terrain/TER_World.h"
#include <xeumeuleu/xml.hpp>

namespace
{

class TestPK : private boost::noncopyable
{
public:

    TestPK()
    {
        MockMIL_Time_ABC time;
        MOCK_EXPECT( time.GetTickDuration ).returns( 10u );
        MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 10u );

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
            "</material-composition-types><roof-shape-types/></urban-block-types></urban>" );
        PHY_MaterialCompositionType::Initialize( xisUrbanType );
        xml::xistringstream xisProtection( "<protections><protection name='protection1' type='humain'><neutralization average-time='10s' variance='1s'/></protection></protections>" );
        PHY_Protection::Initialize( xisProtection );
        xml::xistringstream xisDotationNature( "<natures><nature type='Solide' id='1'/></natures>" );
        PHY_DotationNature::Initialize( xisDotationNature );
        xml::xistringstream xisLogisticSupplyClasses( "<logistic-supply-classes><logistic-supply-class id='12' type='whatever'/></logistic-supply-classes>" );
        PHY_DotationLogisticType::Initialize( xisLogisticSupplyClasses );
        xml::xistringstream xisDotations( "<resources><resource name='ammo' category='munition' id='1' logistic-supply-class='whatever' nature='Solide' id-nature='1' package-size='1' package-mass='1' package-volume='1'>"
            "<attritions><attrition destruction='0' protection='protection1' repairable-with-evacuation='0.1' repairable-without-evacuation='0.05'/></attritions>"
            "<urban-modifiers><urban-modifier material-type='Brique' value='0.4' /></urban-modifiers>"
            "</resource></resources>" );
        PHY_DotationType::Initialize( xisDotations );
        xml::xistringstream xisLauncher( "<launchers><launcher name='gun' indirect-fire='true'/></launchers>" );
        xml::xistringstream xisVolumes( "<volumes><volume name='Big'/></volumes>");
        PHY_Volume::Initialize( xisVolumes );
    }

private:
    SingletonTerminator terminator_;
};

}  // namespace

// -----------------------------------------------------------------------------
// Name: PkComputerTest
// Created: SLG 2010-04-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( PkComputerUrbanProtectionTest, TestPK )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

    xml::xistringstream xisCity( "<urban-object name='city' id='1'/>" );
    xml::xistringstream xisDistrict( "<urban-object name='district' id='2'/>" );
    xml::xistringstream xisModel(
        "<urban-object id='3' name='School'>"
        "   <footprint>"
        "       <point location='31TCM1058982147'/>"
        "       <point location='31TCM1055881170'/>"
        "       <point location='31TCM1157881118'/>"
        "       <point location='31TCM1158882115'/>"
        "   </footprint>"
        "   <physical>"
        "       <architecture floor-number='30' height='40' material='Brique' roof-shape='flat' occupation ='0.8' trafficability='60'/>"
        "   </physical>"
        "</urban-object>" );

    MIL_ObjectFactory factory;
    {
        xml::xistringstream xis(
            "<objects>"
            "    <object type='urban block'/>"
            "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    MockAgent firer;
    xisCity >> xml::start( "urban-object" );
    std::unique_ptr< MIL_UrbanObject_ABC > city;
    city.reset( factory.CreateUrbanObject( xisCity, 0 ) );
    xisCity >> xml::end;
    xisDistrict >> xml::start( "urban-object" );
    std::unique_ptr< MIL_UrbanObject_ABC > district;
    district.reset( factory.CreateUrbanObject( xisDistrict, city.get() ) );
    xisDistrict >> xml::end;
    xisModel >> xml::start( "urban-object" );
    std::unique_ptr< MIL_UrbanObject_ABC > urbanBlock;
    urbanBlock.reset( factory.CreateUrbanObject( xisModel, district.get() ) );
    xisModel >> xml::end;
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( firer );

    urbanRole->NotifyMovingInsideObject( *urbanBlock, MT_Vector2D(), MT_Vector2D() );
    firer.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    BOOST_CHECK_CLOSE( 0.32, urbanRole->ComputeUrbanProtection( *pCategory ), 1. );

    urbanBlock.reset();
}
